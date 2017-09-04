local Packet = require 'util.packet'
--使用物品入口
--[[
@player:玩家对象
@item_id:物品id
@count:使用物品数量
]]
function UseItem(player, item_guid, count)
	if count <= 0 then return end		--数量不能小于等于0
	local itemMgr = player:getItemMgr()	
	local item = itemMgr:getItemByGuid(item_guid, BAG_TYPE_MAIN_BAG)		--只有主包裹的物品才能使用
	if not item then
		outFmtError("UseItem: not find item_id %s ", item_guid)
		--没找到这个物品
		player:CallOptResult(OPRATE_TYPE_BAG,BAG_RESULT_ITEM_NOT_EXIST)
		return
	end
	--使用的数量不够
	if count > item:getCount() then
		outFmtError("UseItem: item entry %d use count %d > has count %d", item:getEntry(), count, item:getCount())
		player:CallOptResult(OPRATE_TYPE_BAG,BAG_RESULT_LACK_USER)
		return
	end
	
	local item_entry = item:getEntry()
	local item_template = tb_item_template[item_entry]
	if not item_template then
		--模板配置没加
		outFmtError("UseItem: tb_item_template not find %d ! ",item_entry)
		return
	end
	--是否不可被使用物品
	if item_template.use_result == ITEM_USE_RESULT_UN_USE then
		outFmtError("UseItem: item %d cant be used!", item_entry)
		return
	end
	--校验下使用等级
	if player:GetLevel() < item_template.level then
		outFmtError("UseItem: player %s level %d is too low to use item %d %d", player:GetGuid(), player:GetLevel(), item_entry, item_template.level)
		return
	end
	
	--校验物品cd
	local cd = itemMgr:getCoolDown(item_entry)
	if cd > 0 then
		outFmtDebug("UseItem: item %d has cd %d", item_entry, cd)
		return 
	end
	--处理cd
	itemMgr:handleCoolDown(item_entry)
	
	player.cur_use_item_guid = item_guid 	--保存当前正在使用的物品id
	--使用接口
	UseItemScripts.Go(UseItemScripts, player, item, count)
end

function UseHpItem(player, entry, count)
	
	return true
end


-- 购买自动复活道具
function useRespawn(player)
	
	return true
end



--场景服返回使用物品结果
function ScenedUseItemResult(player_guid, item_entry, count, result)
	if result == 0 then return end
	local player = app.objMgr:getObj(player_guid)
	if not player then
		outFmtDebug("use_item_result:player %s not online item_entry %d count %d", player_guid, item_entry, count)
		return
	end
	local itemMgr = player:getItemMgr()
	local item = itemMgr:getItemByGuid(player.cur_use_item_guid)
	if not item then 
		outFmtDebug("use_item_result: player %s not find item id %s", player_guid, player.cur_use_item_guid)
		return 
	end
	if item_entry ~= item:getEntry() then
		outFmtDebug("use_item_result: player %s useitem item_entry %d ~= %d", player_guid, item_entry, item:getEntry())
		return
	end

	local item_template = tb_item_template[item_entry]
	local use_type = item_template.use_result
	if use_type == ITEM_USE_RESULT_DISAPPEAR then
		local stack_count = item:getCount()
		if stack_count > count then
			item:setCount(stack_count - count)
			itemMgr.itemMgr:SavePtr(item.item)	
			WriteItemLog(player, LOG_ITEM_OPER_TYPE_FORGE_DECOMPOSE, item_entry, count, item:isBind())
		elseif stack_count == count then
			itemMgr:delItemObj(item)
			WriteItemLog(player, LOG_ITEM_OPER_TYPE_FORGE_DECOMPOSE, item_entry, count, item:isBind())
		end		
	end
	
	local questMgr = player:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_USE_ITEM, {item_entry})
end


---------------------------------------------------------
--使用物品:
--	@self:		自身table
--	@player:	使用物品的玩家
--	@item_entry:	使用的物品模板ID
--	@item_type:	物品模板里面的物品类别
--	@item_data:	物品模板中的保留字段

UseItemScripts = {
	--需要发送到场景服的物品模板(复活丹也写这里)
	scened_use_items_array = {
		50003,191,171,201,202,203,204
	},
	
	-- 仙府宝箱的id
	xianfu_items = {50006},
	
	scened_use_items_set = {},
	__init__= function(self)
		table.foreach(self.scened_use_items_array, function(i,v)
			self.scened_use_items_set[v] = 1
		end)
	end,
	
	Send2ScenedUseItem = function(self, player, item_entry, count)
		local pkt = Packet.new(INTERNAL_OPT_USER_ITEM)
		pkt:writeUTF(player:GetGuid())
		pkt:writeU32(item_entry)
		pkt:writeU32(count)
		app:sendToConnection(player:GetScenedFD(), pkt)
		pkt:delete()
	end,
	
	--使用物品脚本
	Go = function(self,player,item,count)
		local itemMgr = player:getItemMgr()
		local gender = player:GetGender()
		local item_entry = item:getEntry()
		local config = tb_item_template[item_entry]
		if not config then return end
		local item_type = config.type	
		local func = DoUseItem[item_entry]
		if(func ~= nil)then
			if not func(DoUseItem,player,item,count) then
				return
			end
		else
			---发送到场景服使用
			--装备类型的物品不可以使用
			if item_type == ITEM_TYPE_EQUIP then
				outFmtError("item %d is equip, cant use!", item_entry)
				return
			end
			
			if(self.scened_use_items_set[item_entry]) then
				--需要发送的参数：玩家guid、物品模板、数量
				self:Send2ScenedUseItem(player, item_entry, count)		
				return
			end	
			--走使用物品流程
			if item_type == ITEM_TYPE_BOX then
				if config.is_slather == 0 and count > 1 then
					--不能批量使用，使用数量却大于1
					outFmtError("item cant batch use item %d count %d", item_entry, count)
					return
				end
				
				--print("================in use count = ", count)

				--宝箱类（使用获得道具类型）
				local  box_config = tb_box[item_entry]
				if not box_config then return end
				--是否装得下物品
				--[[
				if (itemMgr:getEmptyCount() < (box_config.bag_num or 1) ) then	
					player:CallOptResult(OPRATE_TYPE_BAG,BAG_RESULT_BAG_FULL)
					return
				end
				--]]
				local consum_config = box_config.cost
				--先看下够不够消耗
				for _, consum_info in pairs(consum_config) do
					local consum_type = consum_info[ 1 ]
					local consum_val = consum_info[ 2 ] * count
					if consum_type == Item_Loot_Silver then		--消耗铜钱
						if player:GetMoney(MONEY_TYPE_SILVER) < consum_val then return end
					elseif consum_type == Item_Loot_Bind_Gold then	--消耗绑定元宝
						if player:GetMoney(MONEY_TYPE_BIND_GOLD) < consum_val then return end
					elseif consum_type == Item_Loot_Gold then	--消耗元宝
						if player:GetMoney(MONEY_TYPE_GOLD_INGOT) < consum_val then return end
					else
						--消耗道具
						if player:CountItem(consum_type) < consum_val then return end
					end					
				end
				--可以处理消耗了
				for _, consum_info in pairs(consum_config) do
					local consum_type = consum_info[ 1 ]
					local consum_val = consum_info[ 2 ] * count
					if consum_type == Item_Loot_Silver then		--消耗铜钱
						if(self:SubMoney(MONEY_TYPE_SILVER, MONEY_CHANGE_USE_BOX, consum_val) == false)then return end
					elseif consum_type == Item_Loot_Bind_Gold then	--消耗绑定元宝
						if(self:SubMoney(MONEY_TYPE_BIND_GOLD, MONEY_CHANGE_USE_BOX, consum_val) == false)then return end
					elseif consum_type == Item_Loot_Gold then	--消耗元宝
						if(self:SubMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_USE_BOX, consum_val) == false)then return end
					else
						--消耗道具
						if(itemMgr:delItem(consum_type, consum_val) == SUB_ITEM_FAIL)then return end
					end					
				end
				--把物品先删掉
				if not itemMgr:delItemObj(item, count) then
					return
				end
				
				local dropId = GetRewardIfGenderSensitive(box_config.dropids, gender)
				
				local dict = {}
				for i = 1, count do
					DoRandomDrop(dropId, dict)
				end
				
				local rewardDict = {}
				for itemId, value in pairs(dict) do
					table.insert(rewardDict, {itemId, value})
				end
				
				player:AppdAddItems(rewardDict, MONEY_CHANGE_BOX_RANDOM, LOG_ITEM_OPER_TYPE_OPEN_BOX)
				
				-- 判断是否是仙府夺宝中的宝箱
				if table.find(self.xianfu_items, item_entry) then
					for _, rewardInfo in pairs(rewardDict) do
						local entry = rewardInfo[ 1 ]
						local count = rewardInfo[ 2 ]
						
						local config = tb_item_template[entry]
						for _, record in pairs(config.records) do
							-- 仙府宝箱
							if record == ITEM_RECORD_XIANFU then
								local str = string.format("%s|%d|%d|%d", player:GetName(), item_entry, entry, count)
								globalValue:AddXianfuRecord(str)
							end
						end
					end
				end
				
				--[[
				if config.is_slather == 0 then
					--给随机奖励
					local result = ExtractRandomReward( gender, box_config.reward_random_id )
					for _, config in ipairs(result) do
						--抽中了则给随机奖励
						player:AddItemByEntry(config[1], config[2] , MONEY_CHANGE_BOX_RANDOM, LOG_ITEM_OPER_TYPE_OPEN_BOX, config[3])
					end
				end
				]]
			elseif item_type == ITEM_TYPE_MEDICINE or item_type == ITEM_TYPE_BUFF or item_type == ITEM_TYPE_PK_MEDICINE or item_type == ITEM_TYPE_PET_MEDICINE then
				--药品、获得buff、pk药、宠物药发到场景服处理
				self:Send2ScenedUseItem(player, item_entry, count)	
				return
			end		
		end
		
		local questMgr = player:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_USE_ITEM, {item_entry})
	end,
	
}
--初始化
UseItemScripts:__init__()

-- 是否是大cd物品模版（返回下标偏移值1-20，可使用次数）
function DoIsBigCdItemsEntry(entry)
	
	return 0
end

DoUseItem = {
	[301] = --八仙过海
		function(self,player,item,count)
			local itemMgr = player:getItemMgr()
			if (itemMgr:delItemObj(item, count)) then
				player:DoTitleOpt(TITLE_OPT_TYPE_JIHUO,TITLE_TYPE_BAXIANGUOHAI,os.time()+2592000)
				return true
			end
			return false
		end,
	[193] = --初级潜力丹
		function(self,player,item,count)
			local itemMgr = player:getItemMgr( )
			if (itemMgr:delItemObj(item, count)) then
				player:AddQianLiValue(count)
				player:AddAddQianLiValue(count)
				return true
			end
			return false
		end,		
	[194] = --中级潜力丹
		function(self,player,item,count)			
			local itemMgr = player:getItemMgr( )
			if (itemMgr:delItemObj(item, count)) then
				player:AddQianLiValue(3*count)
				player:AddAddQianLiValue(3*count)
				return true
			end
			return false
		end,		
	[195] = --高级潜力丹
		function(self,player,item,count)
			local itemMgr = player:getItemMgr( )
			if (itemMgr:delItemObj(item, count)) then
				player:AddQianLiValue(10*count)
				player:AddAddQianLiValue(10*count)
				return true
			end
			return false
		end,
}




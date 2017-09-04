
local ActionBase = require('robotd.action.action')
local ActionAppd = class('ActionAppd', ActionBase)

--获取类型
function ActionAppd:GetType()
	return ROBOT_ACTION_TYPE_APPD
end

--初始化变量
function ActionAppd:Initialize(...)
	self:RegOpcodeHandler(SMSG_OPERATION_FAILED, self.HandlerOperationFailed)
	-- AddTimer 参数：1：名字，2：对应业务逻辑的函数名，3:间隔执行时间，4：玩家对象
	-- 每秒发送6个消息
	self:AddTimer("SendProtocol", self.player.SendProtocol, 1000, self.player)
	
		--[[
	-- 装备等使用检测
	self:AddTimer("Send_Bag_Exchange_Pos", self.player.Send_Bag_Exchange_Pos, 10000, self.player)
	-- 加物品
	self:AddTimer("GmAddItem", self.player.GmAddItem, 9000, self.player)
	-- 摧毁物品
	self:AddTimer("Send_Bag_Destroy", self.player.Send_Bag_Destroy, 8000, self.player)
	-- 使用物品
	self:AddTimer("Send_Use_Item", self.player.Send_Use_Item, 7000, self.player)	
	-- 商店买物品
	self:AddTimer("Send_Mall_Buy", self.player.Send_Mall_Buy, 7000, self.player)
	-- 强化装备部位
	self:AddTimer("Send_Strength", self.player.Send_Strength, 7000, self.player)

	-- 创建帮派
	self:AddTimer("Send_Create_Faction", self.player.Send_Create_Faction, 17000, self.player)
	-- 升级帮派
	self:AddTimer("Send_Faction_Upgrade", self.player.Send_Faction_Upgrade, 18000, self.player)
	-- 加入帮派
	self:AddTimer("Send_Faction_Join", self.player.Send_Faction_Join, 19000, self.player)
	--
	self:AddTimer("Send_Raise_Base_Spell", self.player.Send_Raise_Base_Spell, 10000, self.player)
	--
	self:AddTimer("Send_Upgrade_Anger_Spell", self.player.Send_Upgrade_Anger_Spell, 10000, self.player)
	--
	self:AddTimer("Send_Raise_Mount", self.player.Send_Raise_Mount, 10000, self.player)
	--
	self:AddTimer("Send_Upgrade_Mount", self.player.Send_Upgrade_Mount, 10000, self.player)
	--
	self:AddTimer("Send_Upgrade_Mount_One_Step", self.player.Send_Upgrade_Mount_One_Step, 10000, self.player)
	--
	self:AddTimer("Send_Illusion_Mount_Active", self.player.Send_Illusion_Mount_Active, 10000, self.player)
	--
	self:AddTimer("Send_Illusion_Mount", self.player.Send_Illusion_Mount, 10000, self.player)
	--
	self:AddTimer("Send_Gem", self.player.Send_Gem, 10000, self.player)
	--
	self:AddTimer("Send_Divine_Active", self.player.Send_Divine_Active, 10000, self.player)
	--
	self:AddTimer("Send_Divine_Uplev", self.player.Send_Divine_Uplev, 10000, self.player)
	--
	self:AddTimer("Send_Divine_Switch", self.player.Send_Divine_Switch, 10000, self.player)
	--
	self:AddTimer("Send_Sweep_Vip_Instance", self.player.Send_Sweep_Vip_Instance, 10000, self.player)
	--
	self:AddTimer("Send_Hang_Up", self.player.Send_Hang_Up, 10000, self.player)
	--
	self:AddTimer("Send_Hang_Up_Setting", self.player.Send_Hang_Up_Setting, 10000, self.player)
	--
	self:AddTimer("Send_Sweep_Trial_Instance", self.player.Send_Sweep_Trial_Instance, 10000, self.player)
	--
	self:AddTimer("Send_Reset_Trial_Instance", self.player.Send_Reset_Trial_Instance, 10000, self.player)
	--
	self:AddTimer("Send_Social_Add_Friend", self.player.Send_Social_Add_Friend, 10000, self.player)
	--
	self:AddTimer("Send_Social_Sureadd_Friend", self.player.Send_Social_Sureadd_Friend, 10000, self.player)
	--
	self:AddTimer("Send_Social_Gift_Friend", self.player.Send_Social_Gift_Friend, 10000, self.player)
	--
	self:AddTimer("Send_Social_Recommend_Friend", self.player.Send_Social_Recommend_Friend, 10000, self.player)
	--
	self:AddTimer("Send_Social_Revenge_Enemy", self.player.Send_Social_Revenge_Enemy, 10000, self.player)
	--
	self:AddTimer("Send_Social_Del_Friend", self.player.Send_Social_Del_Friend, 10000, self.player)
	--
	self:AddTimer("Send_Social_Clear_Apply", self.player.Send_Social_Clear_Apply, 10000, self.player)
	--
	self:AddTimer("Send_Social_Add_Friend_Byname", self.player.Send_Social_Add_Friend_Byname, 10000, self.player)
	--
	self:AddTimer("Send_Chat_By_Channel", self.player.Send_Chat_By_Channel, 10000, self.player)
	--
	self:AddTimer("Send_Msg_Decline", self.player.Send_Msg_Decline, 10000, self.player)
	--
	self:AddTimer("Send_Block_Chat", self.player.Send_Block_Chat, 10000, self.player)
	--
	self:AddTimer("Send_Faction_Getlist", self.player.Send_Faction_Getlist, 10000, self.player)
	--
	self:AddTimer("Send_Faction_Manager", self.player.Send_Faction_Manager, 10000, self.player)
	--
	self:AddTimer("Send_Faction_Member_Operate", self.player.Send_Faction_Member_Operate, 10000, self.player)
	--
	self:AddTimer("Send_Faction_Fast_Join", self.player.Send_Faction_Fast_Join, 10000, self.player)
	--
	self:AddTimer("Send_Read_Mail", self.player.Send_Read_Mail, 10000, self.player)
	--
	self:AddTimer("Send_Pick_Mail", self.player.Send_Pick_Mail, 10000, self.player)
	--
	self:AddTimer("Send_Remove_Mail", self.player.Send_Remove_Mail, 10000, self.player)
	--
	self:AddTimer("Send_Pick_Mail_One_Step", self.player.Send_Pick_Mail_One_Step, 10000, self.player)
	--
	self:AddTimer("Send_Remove_Mail_One_Step", self.player.Send_Remove_Mail_One_Step, 10000, self.player)
	--
	self:AddTimer("Send_Cancel_Block_Chat", self.player.Send_Cancel_Block_Chat, 10000, self.player)
	--
	self:AddTimer("Send_World_Boss_Enroll", self.player.Send_World_Boss_Enroll, 10000, self.player)
	--
	self:AddTimer("Send_Rank_Add_Like", self.player.Send_Rank_Add_Like, 10000, self.player)
	--
	self:AddTimer("Send_Res_Instance_Sweep", self.player.Send_Res_Instance_Sweep, 10000, self.player)
	--
	self:AddTimer("Send_Get_Activity_Reward", self.player.Send_Get_Activity_Reward, 10000, self.player)
	--
	self:AddTimer("Send_Get_Achieve_Reward", self.player.Send_Get_Achieve_Reward, 10000, self.player)
	--
	self:AddTimer("Send_Get_Achieve_All_Reward", self.player.Send_Get_Achieve_All_Reward, 10000, self.player)
	--
	self:AddTimer("Send_Set_Title", self.player.Send_Set_Title, 10000, self.player)
	--
	self:AddTimer("Send_Init_Title", self.player.Send_Init_Title, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Shouchong_Reward", self.player.Send_Welfare_Shouchong_Reward, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Checkin", self.player.Send_Welfare_Checkin, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Checkin_All", self.player.Send_Welfare_Checkin_All, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Checkin_Getback", self.player.Send_Welfare_Checkin_Getback, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Level", self.player.Send_Welfare_Level, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Active_Getback", self.player.Send_Welfare_Active_Getback, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Getalllist_Getback", self.player.Send_Welfare_Getalllist_Getback, 10000, self.player)
	--
	self:AddTimer("Send_Welfare_Getall_Getback", self.player.Send_Welfare_Getall_Getback, 10000, self.player)
	--
	self:AddTimer("Send_Pick_Quest_Reward", self.player.Send_Pick_Quest_Reward, 10000, self.player)
	--
	self:AddTimer("Send_Use_Virtual_Item", self.player.Send_Use_Virtual_Item, 10000, self.player)
	--
	self:AddTimer("Send_Pick_Quest_Chapter_Reward", self.player.Send_Pick_Quest_Chapter_Reward, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_3v3_Match", self.player.Send_Kuafu_3v3_Match, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_3v3_Match_Oper", self.player.Send_Kuafu_3v3_Match_Oper, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_3v3_Buytimes", self.player.Send_Kuafu_3v3_Buytimes, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_3v3_Dayreward", self.player.Send_Kuafu_3v3_Dayreward, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_3v3_Getranlist", self.player.Send_Kuafu_3v3_Getranlist, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_3v3_Getmyrank", self.player.Send_Kuafu_3v3_Getmyrank, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_3v3_Cancel_Match", self.player.Send_Kuafu_3v3_Cancel_Match, 10000, self.player)
	--
	self:AddTimer("Send_Kuafu_Xianfu_Match", self.player.Send_Kuafu_Xianfu_Match, 10000, self.player)
	--
	self:AddTimer("Send_Buy_Xianfu_Item", self.player.Send_Buy_Xianfu_Item, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_Fight", self.player.Send_Doujiantai_Fight, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_Buytime", self.player.Send_Doujiantai_Buytime, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_Clearcd", self.player.Send_Doujiantai_Clearcd, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_First_Reward", self.player.Send_Doujiantai_First_Reward, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_Get_Enemys_Info", self.player.Send_Doujiantai_Get_Enemys_Info, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_Get_Rank", self.player.Send_Doujiantai_Get_Rank, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_Refresh_Enemys", self.player.Send_Doujiantai_Refresh_Enemys, 10000, self.player)
	--
	self:AddTimer("Send_Doujiantai_Top3", self.player.Send_Doujiantai_Top3, 10000, self.player)
	--
	self:AddTimer("Send_Submit_Quest_Daily2", self.player.Send_Submit_Quest_Daily2, 10000, self.player)
	
	-- 换线
	self:AddTimer("Send_Change_Line", self.player.Send_Change_Line, 10000, self.player)
	-- 地图分线信息
	self:AddTimer("Send_Show_Map_Line", self.player.Send_Show_Map_Line, 10000, self.player)
	
	--
	self:AddTimer("Send_Spell_Start", self.player.Send_Spell_Start, 10000, self.player)
	--
	self:AddTimer("Send_Teleport", self.player.Send_Teleport, 10000, self.player)
	--
	self:AddTimer("Send_Use_Gameobject", self.player.Send_Use_Gameobject, 10000, self.player)
	--
	self:AddTimer("Send_Instance_Exit", self.player.Send_Instance_Exit, 10000, self.player)
	--
	self:AddTimer("Send_Ride_Mount", self.player.Send_Ride_Mount, 10000, self.player)
	--
	self:AddTimer("Send_Change_Battle_Mode", self.player.Send_Change_Battle_Mode, 10000, self.player)
	--
	self:AddTimer("Send_Jump_Start", self.player.Send_Jump_Start, 10000, self.player)
	--
	self:AddTimer("Send_Enter_Vip_Instance", self.player.Send_Enter_Vip_Instance, 10000, self.player)
	--
	self:AddTimer("Send_Enter_Trial_Instance", self.player.Send_Enter_Trial_Instance, 10000, self.player)
	--
	self:AddTimer("Send_Teleport_Main_City", self.player.Send_Teleport_Main_City, 10000, self.player)
	--
	self:AddTimer("Send_Use_Broadcast_Gameobject", self.player.Send_Use_Broadcast_Gameobject, 10000, self.player)
	--
	self:AddTimer("Send_World_Boss_Fight", self.player.Send_World_Boss_Fight, 10000, self.player)
	--
	self:AddTimer("Send_Res_Instance_Enter", self.player.Send_Res_Instance_Enter, 10000, self.player)
	--
	self:AddTimer("Send_Teleport_Map", self.player.Send_Teleport_Map, 10000, self.player)
	--
	self:AddTimer("Send_Teleport_Field_Boss", self.player.Send_Teleport_Field_Boss, 10000, self.player)
	--
	self:AddTimer("Send_Talk_With_Npc", self.player.Send_Talk_With_Npc, 10000, self.player)
	--
	self:AddTimer("Send_Loot_Select", self.player.Send_Loot_Select, 10000, self.player)
	--
	self:AddTimer("Send_Gold_Respawn", self.player.Send_Gold_Respawn, 10000, self.player)
	--
	self:AddTimer("Send_Clientsubscription", self.player.Send_Clientsubscription, 10000, self.player)
	--
	self:AddTimer("Send_Use_Jump_Point", self.player.Send_Use_Jump_Point, 10000, self.player)

	--]]
end

--获取类型名
function ActionAppd:GetName()
	return 'ActionAppd'
end

--操作失败提示
function ActionAppd:HandlerOperationFailed(args)
	outDebug(OperationFailedToString(args.type, args.reason, args.data))
	if(args.type == OPERTE_TYPE_CLOSE)then
		outFmtInfo("%s  %s", OperationFailedToString(args.type, args.reason, args.data), self.player.account)
	end
end

--心跳
function ActionAppd:Update(diff)
	return true
end
return ActionAppd

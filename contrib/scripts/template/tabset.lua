tb_faction_shop_list = {}

for id,config in ipairs(tb_faction_shop) do
	local lev = config.lev
	local group_id = config.group_id
	local discount_id = config.discount_id
	local discount_ratio = config.discount_ratio

	if not tb_faction_shop_list[lev] then
		tb_faction_shop_list[lev] = {}
	end
	
	if not tb_faction_shop_list[lev][group_id] then
		tb_faction_shop_list[lev][group_id] = {}
	end
	
	if not tb_faction_shop_list[lev][group_id][discount_id] then
		tb_faction_shop_list[lev][group_id][discount_id] = {}
	end
	
	local total_weight = 0
	for index,info in ipairs(tb_faction_shop_list[lev][group_id][discount_id]) do
		total_weight = info.total_weight
	end
	
	table.insert(tb_faction_shop_list[lev][group_id][discount_id],{config = config,total_weight = total_weight + discount_ratio})
end


tb_achieve_type_list = {}

for id,config in ipairs(tb_achieve_base) do
	local achtype = config.achtype
	if not tb_achieve_type_list[achtype] then
		tb_achieve_type_list[achtype] = {}
	end
	table.insert(tb_achieve_type_list[achtype],config.id)
end

tb_quest_daily_list = {}
for id, config in ipairs(tb_char_level) do
	local elements = {}
	for _, val in ipairs(config.dailyQuests) do
		table.insert(elements, val)
	end
	
	local before = tb_quest_daily_list[id-1]
	if before then
		for _, val in ipairs(before) do
			table.insert(elements, val)
		end
	end
	
	tb_quest_daily_list[id] = elements
end

--print(#tb_faction_shop_list)
tb_system_base_task_list = {}
tb_system_base_level_list = {}

for id,info in pairs(tb_system_base) do
	if info.questId ~= 0 then
		if tb_system_base_task_list[info.questId] == nil then
			local list = {}
			table.insert(list,id)
			tb_system_base_task_list[info.questId] = list
		else
			table.insert(tb_system_base_task_list[info.questId],id)
			
		end
	
	else
		if tb_system_base_level_list[info.level] == nil then
			local list = {}
			table.insert(list,id)
			tb_system_base_level_list[info.level] = list
		else
			table.insert(tb_system_base_level_list[info.level],id)
			
		end
		
	end
	
end

tb_money_type_list = {}

for id,info in pairs(tb_item_template) do
	if info.money_type ~= 0 then
		tb_money_type_list[info.money_type - 1] = id
	end
end


tb_faction_boss_reward_list = {}

for id,info in pairs(tb_faction_boss_reward) do
	if not tb_faction_boss_reward_list[info.monster_id] then
		tb_faction_boss_reward_list[info.monster_id] = {}
	end
	for i = info.rank[1],info.rank[2] do
		tb_faction_boss_reward_list[info.monster_id][i] = {reward = info.reward,mail_name = info.mail_name,mail_desc = info.mail_desc,fail_reward = info.fail_reward,fail_mail_name = info.fail_mail_name,fail_mail_desc = info.fail_mail_desc}
	end
	
end
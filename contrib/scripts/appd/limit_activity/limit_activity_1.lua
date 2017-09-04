Limit_Activity_1 = class('Limit_Activity_1', LimitActivityBase)


Limit_Activity_1.Name = 'Limit_Activity_1'
Limit_Activity_1.Version = 1 --版本号
Limit_Activity_1.ClientConfigVersion = 1	--客户端用的判断规则版本号
--领奖规则
Limit_Activity_1.ReceiveTable ={
		[0] = {		--累计100
			condition = {
				{LIMIT_ACTIVITY_CONFIG_RECHARGE, 1000},
			},
			[1] ={
				{LIMIT_ACTIVITY_CONFIG_ITEM, 171, 5, 2,true,true,true,0, function(self)	return 0 end,},
				{LIMIT_ACTIVITY_CONFIG_ITEM, 181, 5, 2,true,true,true,0, function(self)	return 0 end,},
				{LIMIT_ACTIVITY_CONFIG_ITEM, 191, 5, 2,true,true,true,0, function(self)	return 0 end,},
			},
		},
		[1] = {
			func = function(self, player, gift_id, gift_type, count)
				
			end,
		},
		
	}


return Limit_Activity_1
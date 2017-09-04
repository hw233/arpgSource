function PlayerInfo:AfterQuestDoing(doing)
	-- 不需要做
	if #doing == 0 then
		return
	end
	local type = doing[ 1 ]
	
	-- 进入仙府体验副本
	if type == 1 then
		self:EnterXianfuTest()
	-- 进入主线剧情副本
	elseif type == 2 then
		self:EnterRemindInstance(doing[ 2 ], doing[ 3 ], doing[ 4 ], doing[ 5 ], doing[ 6 ], doing[ 7 ], doing[ 8 ])
	end
end
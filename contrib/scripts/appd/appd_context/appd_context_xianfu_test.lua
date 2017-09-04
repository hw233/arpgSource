function PlayerInfo:EnterXianfuTest()
	local gerneralId = string.format("%s%d", self:GetGuid(), os.time())
	local mapid = 2018
	local offsetX = 0
	local offsetY = 0
	local pos = tb_kuafu_xianfu_tst_base[ 1 ].bornPos[ 1 ]
	-- 发起传送
	call_appd_teleport(self:GetScenedFD(), self:GetGuid(), pos[ 1 ] + offsetX, pos[ 2 ] + offsetY, mapid, gerneralId)
end

-- 进入原地副本
function PlayerInfo:EnterRemindInstance(mapid, ex, ey, entry, x, y, prevmapid)
	local gerneralId = string.format("%s|%d|%d|%d|%d|%d|%d|%d", self:GetGuid(), os.time(), prevmapid, ex, ey, entry, x, y)
	self:CallScenedDoSomething(APPD_SCENED_REMIND_INSTANCE_ENTER, mapid, gerneralId)
end
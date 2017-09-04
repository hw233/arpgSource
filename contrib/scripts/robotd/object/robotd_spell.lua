local RobotdSpell = class('RobotdSpell', BinLogObject)

--构造函数
function RobotdSpell:ctor()
	
end



--获取神兵列表
function RobotdSpell:getDivineIdList()
	local tab = {}
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END, MAX_DIVINE_COUNT do
		local id = self:GetByte(i,0)
		if id > 0  then
			table.insert(tab, id)
		end
	end
	return tab
end

return RobotdSpell
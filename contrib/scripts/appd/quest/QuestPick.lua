local QuestPick = class("QuestPick", AbstractQuest)

IQuestPick = QuestPick:new{}

function QuestPick:ctor()
	
end

function QuestPick:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestPick:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestPick:OnUpdate(playerInfo, start, offset, params)
	local showname = nil
	if tb_gameobject_template[params[ 1 ]] then
		showname = tb_gameobject_template[params[ 1 ]].name
	end
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params, showname)
end

return QuestPick
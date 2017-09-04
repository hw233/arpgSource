local QuestWingsStrengthLevel = class("QuestWingsStrengthLevel", AbstractQuest)

IQuestWingsStrengthLevel = QuestWingsStrengthLevel:new{}

function QuestWingsStrengthLevel:ctor()
	
end

function QuestWingsStrengthLevel:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local spellMgr = playerInfo:getSpellMgr()
	local level = spellMgr:GetWingsLevel()
	-- 获得翅膀强化数
	self:OnUpdate(playerInfo, start, offset, {level})
end

-- 获得目标值
function QuestWingsStrengthLevel:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestWingsStrengthLevel:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeValue(playerInfo, start, offset, params)
end

return QuestWingsStrengthLevel
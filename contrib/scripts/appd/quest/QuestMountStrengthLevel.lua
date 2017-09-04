local QuestMountStrengthLevel = class("QuestMountStrengthLevel", AbstractQuest)

IQuestMountStrengthLevel = QuestMountStrengthLevel:new{}

function QuestMountStrengthLevel:ctor()
	
end

function QuestMountStrengthLevel:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local spellMgr = playerInfo:getSpellMgr()
	local level = spellMgr:getMountLevelBase()
	-- 获得坐骑强化数
	self:OnUpdate(playerInfo, start, offset, {level})
end

-- 获得目标值
function QuestMountStrengthLevel:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestMountStrengthLevel:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeValue(playerInfo, start, offset, params)
end

return QuestMountStrengthLevel
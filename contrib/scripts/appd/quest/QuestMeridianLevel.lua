local QuestMeridianLevel = class("QuestMeridianLevel", AbstractQuest)

IQuestMeridianLevel = QuestMeridianLevel:new{}

function QuestMeridianLevel:ctor()
	
end

function QuestMeridianLevel:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local spellMgr = playerInfo:getSpellMgr()
	local level = spellMgr:getMeridianLevel()
	-- 获得经脉等级
	self:OnUpdate(playerInfo, start, offset, {level})
end

-- 获得目标值
function QuestMeridianLevel:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestMeridianLevel:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeValue(playerInfo, start, offset, params)
end

return QuestMeridianLevel
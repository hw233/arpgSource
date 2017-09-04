local QuestGemTotalLevel = class("QuestGemTotalLevel", AbstractQuest)

IQuestGemTotalLevel = QuestGemTotalLevel:new{}

function QuestGemTotalLevel:ctor()
	
end

function QuestGemTotalLevel:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local level = playerInfo:GetGemTotalLevel()
	-- 获得翅膀阶数
	self:OnUpdate(playerInfo, start, offset, {level})
end

-- 获得目标值
function QuestGemTotalLevel:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestGemTotalLevel:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeValue(playerInfo, start, offset, params)
end

return QuestGemTotalLevel
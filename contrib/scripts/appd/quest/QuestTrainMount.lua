local QuestTrainMount = class("QuestTrainMount", AbstractQuest)

IQuestTrainMount = QuestTrainMount:new{}

function QuestTrainMount:ctor()
	
end

function QuestTrainMount:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestTrainMount:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestTrainMount:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestTrainMount
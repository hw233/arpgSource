local QuestSuit = class("QuestSuit", AbstractQuest)

IQuestSuit = QuestSuit:new{}

function QuestSuit:ctor()
	
end

function QuestSuit:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local pos = tb_quest[questId].targets[offset+1][ 2 ]
	local itemId = tb_quest[questId].targets[offset+1][ 3 ]
	
	local equiped = playerInfo:GetEquipment(pos)
	if equiped > 0 and (itemId == 0 or equiped == itemId) then
		self:OnUpdate(playerInfo, start, offset, {pos, itemId})
	end
end

-- 获得目标值
function QuestSuit:GetTargetValue(targetInfo)
	return 1
end

-- 更新进度, 如果目标完成返回true
function QuestSuit:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateDoubleModeObject(playerInfo, start, offset, params)
end

return QuestSuit
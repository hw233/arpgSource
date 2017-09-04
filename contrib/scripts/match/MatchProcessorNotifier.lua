MatchProcessorNotifier = class('MatchProcessorNotifier')

--构造函数
function MatchProcessorNotifier:ctor()
end

function MatchProcessorNotifier:completed(task, position)
	self:notice(task)
	self:notice(position)
end

function MatchProcessorNotifier:notice(task)
	-- 通知匹配完成
	task:Unmark()
end

function MatchProcessorNotifier:cancel(task)
	local  partList = task:getPartList()
	for _, teamPart in pairs(partList) do
		local userIdList = teamPart:getUserIdList()
		for _, id in pairs(userIdList) do
			-- 通知取消匹配
		end
	end
	
	task:Unmark();
end
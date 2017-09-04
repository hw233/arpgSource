--gm后台滚轮公告管理器
local GmNotice = class('GmNotice')

function GmNotice:ctor(  )
	self.gm_notice_map = {}

end

--一秒跳一次的心跳
function GmNotice:update(  )
	local need_free = {}
	local timenow = os.time()
	for notice_id,notice in pairs(self.gm_notice_map) do
		local next_time = timenow + notice.interval 	--下一次发公告时间
		if timenow > notice.end_tm or next_time > notice.end_tm then
			--结束时间到了			
			table.insert(need_free, notice_id)
		else
			if notice.next_time then

			end

			if not notice.next_time or (timenow >= notice.next_time and timenow < notice.end_tm) then				
				app:SendNotice(NOTICE_TYPE_SCROLL, notice.content)
				notice.next_time = next_time				
			end			
		end
	end

	for _,notice_id in ipairs(need_free) do		
		self.gm_notice_map[notice_id] = nil
	end
end

--添加一个滚轮公告
--[[
@notice_id:公告id
@start_tm：公告开始时间
@end_tm:公告结束时间
@interval:公告间隔
@content:公告内容
@status:状态
@return:公告状态
]]
function GmNotice:addGmNotice( notice_id, start_tm, end_tm, interval, content, status )
	if status == SYSTEM_NOTICE_STATUS_START or status == SYSTEM_NOTICE_STATUS_RUNNING then
		local notice = {["start_tm"] = start_tm, ["end_tm"] = end_tm, ["interval"] = interval, ["content"] = content, ["status"] = status }
		self.gm_notice_map[notice_id] = notice	
		return SYSTEM_NOTICE_STATUS_RUNNING
	end
	--删掉某个公告
	self.gm_notice_map[notice_id] = nil
	return SYSTEM_NOTICE_STATUS_GARBAGE
end

return GmNotice
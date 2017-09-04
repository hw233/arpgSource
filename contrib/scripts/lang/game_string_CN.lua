--发公告函数
function onSendNotice(id, ...)
	local config = tb_gonggao[id]
	if not config then return end
	
	local content = config.content
	content = string.format(content, unpack({...}))		
	app:SendNotice(id, content)
end

-- 数字文本字符串
NUMBER_TEXT_CONFIG = {
	[0	] = "零",
	[1	] = "一",
	[2	] = "二",
	[3	] = "三",
	[4	] = "四",
	[5	] = "五",
	[6	] = "六",
	[7	] = "七",
	[8	] = "八",
	[9	] = "九",
	[10	] = "十",
	[11	] = "百",
	[12	] = "千",
	[13	] = "万",
}
-- 获取数字文本（小于100）
function GetNumberString(num)
	local str = ""
	if num < 0 then
		return str
	end
	if num <= 10 then
		str = NUMBER_TEXT_CONFIG[num]
	elseif num < 100 then
		local decade = math.floor(num/10)
		local single = num%10
		local s1,s2,s3 = NUMBER_TEXT_CONFIG[decade],NUMBER_TEXT_CONFIG[10],NUMBER_TEXT_CONFIG[single]
		if decade == 0 or decade == 1 then
			s1 = ""
		end
		if single == 0 then
			s3 = ""
		end
		str = string.format("%s%s%s",s1,s2,s3)
	end
	return str
end

WEEKDAY_TEXT = {
	[1	] = "日",
	[2	] = "一",
	[3	] = "二",
	[4	] = "三",
	[5	] = "四",
	[6	] = "五",
	[7	] = "六",
}
-- 获取星期几周几
function GetWeekDayString(num)
	return WEEKDAY_TEXT[num] or ""
end


Timer = class('Timer')

--构造函数
function Timer:ctor(name, callback, interval, ...)
	self.name = name
	assert(type(callback) == 'function')
	self.callback = callback
	self.interval = interval	
	self.args = {...}
	self:InitDiff()
end

--每次触发callback以后，要重新随机下一次的时间
function Timer:InitDiff()
	if(self.diff == nil)then
		self.diff = 0
	end
	local float = 0
	if(self.interval >= 100)then
		--正负百分十随机
		float = self.interval / 10
		float = randInt(0-float, float)
	end
	self.diff = self.diff + self.interval + float
end

--心跳
function Timer:Update(diff)
	self.diff = self.diff - diff
	if(self.diff <= 0)then
		if(self.callback(unpack(self.args)) ~= true)then
			return false
		end
		self:InitDiff()
	end
	return true
end


return Timer

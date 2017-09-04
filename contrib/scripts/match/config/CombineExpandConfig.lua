CombineExpandConfig = class('CombineExpandConfig')

-- ICombineExpandConfig instance = CombineExpandConfig:new {config = config}
--构造函数
function CombineExpandConfig:ctor()
	
end

-- 获得长度
function CombineExpandConfig:size()
	return #self.config
end

-- 获得元素
function CombineExpandConfig:get(index)
	if index < 0 or index >= self:size() then
		return 0
	end
	return self.config[index+1]
end
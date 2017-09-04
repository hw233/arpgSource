local try		= require("base/try")
local catch		= require("base/catch")
local finally	= require("base/finally")

local security = {}

-- 使用try-catch-finally 安全调用函数
-- try_block 必须有,
-- catch_block 默认打印错误
function security.call(try_block, catch_block, finally_block)
	assert(try_block)
	catch_block = catch_block or function(errors)
		outFmtError(errors)
	end
	
	return try {
		-- try 代码块
		function ()
			return try_block()
		end,

		-- catch 代码块
		catch 
		{
			-- 发生异常后，被执行
			function (errors)
				if catch_block then
					catch_block(errors)
				end
			end
		},

		-- finally 代码块
		finally 
		{
			-- 最后都会执行到这里
			-- 如果try{}中存在异常，ok为true，errors为错误信息，否则为false，errors为try中的返回值
			function (ok, errors)
				if finally_block then
					finally_block(ok, errors)
				end
			end
		}
	}
end

return security


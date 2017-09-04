ScenedContext = class('ScenedContext', BinLogObject)

function ScenedContext:ctor()
end


--扩展包处理函数
require 'scened.context.scened_context_handler'
--require 'scened.scened_internal_handler'

return ScenedContext

local security = require("base/Security")

local ret = security.call(
	--try block
	function()
		return 1
	end,
	
	--catch block
	function(errors)
		outFmtError(errors)
	end,
	
	--finally block
	function(ok, errors)
		
	end
)
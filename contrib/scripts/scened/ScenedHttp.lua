require 'util.functions'
require 'util.json'

local ScenedHttp = class('ScenedHttp')

function ScenedHttp:ctor( )
	self.cobj = http.client.client()
	require('util.http_client').extend(self)
end

function ScenedHttp:update( )
	self.cobj:poll()
end

return ScenedHttp


--游戏配置
local GlobalGameConfig = class('GlobalGameConfig', assert(BinLogObject))

function GlobalGameConfig:ctor( )
	if(post_chat and not self:GetFlag(GAME_CONFIG_FIELD_FLAGS_SHOW_POST))then
		self:SetFlag(GAME_CONFIG_FIELD_FLAGS_SHOW_POST)
	elseif(not post_chat and self:GetFlag(GAME_CONFIG_FIELD_FLAGS_SHOW_POST))then
		self:UnSetFlag(GAME_CONFIG_FIELD_FLAGS_SHOW_POST)
	end
end

function GlobalGameConfig:GetFlag(index)
	return self:GetBit(GAME_CONFIG_INT_FIELD_FLAGS, index)
end

function GlobalGameConfig:SetFlag(index)
	self:SetBit(GAME_CONFIG_INT_FIELD_FLAGS, index)
end

function GlobalGameConfig:UnSetFlag(index)
	self:UnSetBit(GAME_CONFIG_INT_FIELD_FLAGS, index)
end

--获取平台id
function GlobalGameConfig:GetPlatfromID()
	return self:GetStr(GAME_CONFIG_STRING_FIELD_PLATFORM_ID)
end

--获取gmqq串
function GlobalGameConfig:GetGmQQ()
	return self:GetStr(GAME_CONFIG_STRING_FIELD_GM_QQ)
end

--设置gmqq串
function GlobalGameConfig:SetGmQQ(value)
	self:SetStr(GAME_CONFIG_STRING_FIELD_GM_QQ, value)
end

--获取开服时间
function GlobalGameConfig:GetKaiFuShiJian()
	return self:GetUInt32(GAME_CONFIG_INT_FIELD_KAIFUSHIJIAN)
end

--设置开服时间
function GlobalGameConfig:SetKaiFuShiJian(self,val)
	self:SetUInt32(GAME_CONFIG_INT_FIELD_KAIFUSHIJIAN,val)
end

--获取登录秘钥
function GlobalGameConfig:GetLoginKey()
	return Self:GetStr(GAME_CONFIG_STRING_FIELD_LOGIN_KEY)
end

--获取世界服类型
function GlobalGameConfig:GetWorldServerType()
	return self:GetUInt32(GAME_CONFIG_INT_FIELD_WORLD_SVR_TYPE) 
end

--是否游戏服务器
function GlobalGameConfig:IsGameServer()
	return self:GetWorldServerType() == WORLD_SERVER_TYPE_GAME
end

--是否pk服务器
function GlobalGameConfig:IsPKServer()
	return self:GetWorldServerType() == WORLD_SERVER_TYPE_PK
end

--获得争霸天下server_name
function GlobalGameConfig:GetZbtxServerName()
	return self:GetStr(GAME_CONFIG_STRING_FIELD_ZBTX_SVR_NAME)
end

--获得原始服务器server_name
function GlobalGameConfig:GetOriginServerName()
	return self:GetStr(GAME_CONFIG_STRING_FIELD_SERVER)
end

--获取所有合服server_name
function GlobalGameConfig:GetMergeServerName()
	local result = {}
	local only_one = {}
	for i = 0,10000
	do
		local server_name = self:GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i)
		if(server_name ~= "" and server_name ~= nil)then
			if(only_one[server_name] == nil)then
				table.insert(result,server_name)
				only_one[server_name] = 1
			end
		else
			break
		end
	end
	return result
end

function GlobalGameConfig:IsMyServer(sname)
	for i = 0,10000
	do
		local server_name = self:GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i)
		if(server_name ~= "" and server_name ~= nil)then
			if(server_name == sname)then
				return true
			end
		else
			break
		end
	end
	return false
end

-- 是否合服
function GlobalGameConfig:IsMergeServer()
	-- 先判断是否显示服务器ID
	local b = self:GetBit(GAME_CONFIG_INT_FIELD_FLAGS, GAME_CONFIG_FIELD_FLAGS_SHOW_SERVER_ID)
	-- 如果没有，判断服务器列表数是否大于等于2个
	if not b then
		local count = 0
		for i = 0,10000 do
			local server_name = self:GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i)
			if(server_name ~= "" and server_name ~= nil)then
				count = count + 1
			else
				break
			end
		end
		b = count >= 2
	end
	return b
end

--扩展的web接口
function GlobalGameConfig:GetExtWebInterface()
	return self:GetStr(GAME_CONFIG_STRING_EXT_WEB_INTERFACE)
end
--配置服url
function GlobalGameConfig:GetConfSvrUrl()
	return self:GetStr(GAME_CONFIG_STRING_CONF_SVR_URL)
end	
--获得风流镇url地址
function GlobalGameConfig:GetExtWebInterface()
	return self:GetStr(GAME_CONFIG_STRING_EXT_WEB_INTERFACE)
end
--世界服url配置
function GlobalGameConfig:GetWorldServerUrl()
	return self:GetStr(GAME_CONFIG_STRING_WORLD_SERVER_URL)
end

--取得平台id
function GlobalGameConfig:GetPlatformID( )
	return self:GetStr(GAME_CONFIG_STRING_FIELD_PLATFORM_ID)
end

--取得服务器Id
function GlobalGameConfig:GetServerID ( i  )
	i = i or 0
	return self:GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i)
end

return GlobalGameConfig


#include "lua_packet_binding.h"
#include "tolua++.h"
#include "tolua_fix.h"
#include <comm/b64.h>
#include <comm/crypt.h>

static int luapacket_init(lua_State *L)
{
	int optcode = luaL_checkint(L,1);
	packet *pkt = external_protocol_new_packet(optcode);
	lua_pushlightuserdata(L, pkt);
	return 1;
}

static int luapacket_free(lua_State *L)
{
	packet *pkt = (packet*)lua_touserdata(L,1);	
	ASSERT(pkt);//TODO:改成异常
	external_protocol_free_packet(pkt);
	return 0;
}

static int luapacket_optcode(lua_State *L)
{
	int n = lua_gettop(L);
	packet *pkt = (packet*)lua_touserdata(L,1);	
	ASSERT(pkt);//TODO:改成异常
	int optcode = -1;
	if(n > 1)
	{
		optcode = luaL_checkint(L,2);
		pkt->head->optcode = optcode;
		return 0;
	}
	else
	{
		lua_pushinteger(L, pkt->head->optcode);
		return 1;
	}	
}

static int luapacket_write_str(lua_State *L)
{
	packet *pkt = (packet*)lua_touserdata(L,1);	
	char *s = (char*)luaL_checkstring(L, 2);
	packet_write_str(pkt,s);
	return 0;
}

static int luapacket_read_str(lua_State *L)
{
	packet *pkt = (packet*)lua_touserdata(L,1);	
	char *s = nullptr;
	int ret = packet_read_str(pkt,&s);
	lua_pushboolean(L, ret == 0);		//等于0表示正常读取
	lua_pushstring(L, s);
	return 2;
}


#define CHAR_SIZE 50

static  int packet_write_str_by_len(packet *dst, const char *str,int len)
{
	size_t need_szie = dst->wpos+len;
	/*空间不够~加到够再翻倍*/
	if(need_szie>dst->size)
		packet_resize(dst,need_szie*2);
	
	char ch[CHAR_SIZE] = {0};
	uint16 size = strlen(str);
	size = size < len ? size : len;
	strcpy(ch, str);

	packet_write(dst, ch, len);
	//这才是真正的定位
	dst->wpos =  need_szie;
	return 0;
}

static  int packet_read_str_by_len(packet *src,char **str,int len)
{
	*str = (char *)(src->content+src->rpos);
	src->rpos += len;
	if(src->rpos > src->wpos)
		return -2;
	return 0;
}

static int luapacket_write_str_by_len(lua_State *L)
{
	packet *pkt = (packet*)lua_touserdata(L,1);		
	char *s = (char*)luaL_checkstring(L, 2);
	int len = (int)lua_tonumber(L,3);
	packet_write_str_by_len(pkt,s,len);
	return 0;
}

static int luapacket_read_str_by_len(lua_State *L)
{
	packet *pkt = (packet*)lua_touserdata(L,1);	
	int len = (int)lua_tonumber(L,2);
	char *s = nullptr;
	int ret = packet_read_str_by_len(pkt,&s,len);
	lua_pushboolean(L, ret == 0);		//等于0表示正常读取
	lua_pushstring(L, s);
	return 2;
}

static int luamd5tostr(lua_State *L)
{
	string input = lua_tostring(L,1);
	static char output[1024];
	MD5CryptToStr32((int8*)input.c_str(),input.size(),output);
	lua_pushstring(L, output);
	return 1;
}

static int luabase64decode(lua_State *L)
{
	string input = lua_tostring(L,1);
	static char output[1024];
	memset(output, 0, sizeof(output));
	if(base64_decode(output,(int)sizeof(output),input.c_str(),input.size()) != 0)
	{
		lua_pushnil(L);
		return 1;
	}
	lua_pushstring(L, output);
	return 1;
}

static int luabase64encode(lua_State *L)
{
	string input = lua_tostring(L,1);
	char output[1024];
	if(base64_encode(input.c_str(),input.size(),output,(int)sizeof(output)) != 0)
	{
		lua_pushnil(L);
		return 1;
	}
	lua_pushstring(L, output);
	return 1;
}

//
//static int luapacket_write_bytes(lua_State *L)
//{
//	packet *self = (packet*)lua_touserdata(L,1);	
//	packet *from = (packet*)lua_touserdata(L,2);	
//	int start_post = luaL_checkinteger(L, 3);
//	int size = luaL_checkinteger(L, 4);
//	
//	ASSERT(self && from);
//	if(start_post < 0)
//		start_post = from->rpos;
//	//TODO:这个size和下面在rpos会冲突,先不实现
//	if(size < 0)
//		size = from->wpos - start_post;
//	if(start_post + size > (int)from->wpos)
//	{
//		lua_pushboolean(L,FALSE);
//		return 1;
//	}
//	packet_write(self,(char*)from->content + start_post, size);
//	from->rpos += size;
//
//	lua_pushboolean(L, TRUE);
//	return 1;
//}

//////////////////////////////////////////////////////////////////////////
//#define packet_write_type(dst,var) packet_write(dst,(char*)&(var),sizeof(var));

#define PACKET_WRITE_IMPL(fname,type)\
static inline void packet_write_##fname(packet *dst,type val){\
	if(dst->wpos + sizeof(val) > dst->size)\
		packet_resize(dst,(dst->wpos+sizeof(val))*2);\
	(*(type*)(dst->content+dst->wpos)) = val;\
	dst->wpos += sizeof(val);\
}\

#define PACKET_READ_IMPL(fname,type)\
static inline type packet_read_##fname(packet *src){\
	type val;\
	if(src->wpos - src->rpos < sizeof(val))\
		return 0;\
	val = *(type*)(src->content+src->rpos);\
	src->rpos += sizeof(val);\
	return val;\
}\


#define LUA_PACKET_WRITE_IMPL(fname,type)\
static int luapacket_write_##fname(lua_State *L){\
	packet *pkt = (packet*)lua_touserdata(L,1);	\
	type u = (type)lua_tonumber(L, 2);\
	packet_write_##fname(pkt,u);\
	return 0;\
}\

#define LUA_PACKET_READ_IMPL(fname,type)\
static int luapacket_read_##fname(lua_State *L){\
	type val;\
	packet *src = (packet*)lua_touserdata(L, 1);	\
	if(src->wpos - src->rpos < sizeof(val)){\
		lua_pushboolean(L, FALSE);\
		return 1;\
	}\
	val = *(type*)(src->content+src->rpos);\
	src->rpos += sizeof(val);\
	lua_pushboolean(L, TRUE);\
	lua_pushnumber(L, val);\
	return 2;\
}\


PACKET_WRITE_IMPL(u32,uint32_t);
PACKET_WRITE_IMPL(i32,int32_t);
PACKET_WRITE_IMPL(u16,uint16_t);
PACKET_WRITE_IMPL(i16,int16_t);
PACKET_WRITE_IMPL(u8,uint8_t);
PACKET_WRITE_IMPL(float,float);
PACKET_WRITE_IMPL(double,double);

PACKET_READ_IMPL(u32,uint32_t);
PACKET_READ_IMPL(i32,int32_t);
PACKET_READ_IMPL(u16,uint16_t);
PACKET_READ_IMPL(i16,int16_t);
PACKET_READ_IMPL(u8,uint8_t);
PACKET_READ_IMPL(float,float);
PACKET_READ_IMPL(double,double);

LUA_PACKET_WRITE_IMPL(u32,uint32_t);
LUA_PACKET_WRITE_IMPL(i32,int32_t);
LUA_PACKET_WRITE_IMPL(u16,uint16_t);
LUA_PACKET_WRITE_IMPL(i16,int16_t);
LUA_PACKET_WRITE_IMPL(u8,uint8_t);
LUA_PACKET_WRITE_IMPL(float,float);
LUA_PACKET_WRITE_IMPL(double,double);

LUA_PACKET_READ_IMPL(u32,uint32_t);
LUA_PACKET_READ_IMPL(i32,int32_t);
LUA_PACKET_READ_IMPL(u16,uint16_t);
LUA_PACKET_READ_IMPL(i16,int16_t);
LUA_PACKET_READ_IMPL(u8,uint8_t);
LUA_PACKET_READ_IMPL(float,float);
LUA_PACKET_READ_IMPL(double,double);

//////////////////////////////////////////////////////////////////////////

static int gOnInternalPacket_delegate_index = 0;

//当收到时候触发
static int luapacket_register_on_internal_packet(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
	if(!toluafix_isfunction(tolua_S, 1, "LUA_FUNCTION", 0, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luapacket_register_on_internal_packet'.",&tolua_err);
		return 0;
	}
	if(gOnInternalPacket_delegate_index)
		toluafix_remove_function_by_refid(tolua_S, gOnInternalPacket_delegate_index);
	gOnInternalPacket_delegate_index = toluafix_ref_function(tolua_S,1,0);
	return 0;
}

static int gOnExternalPacket_delegate_index = 0;

//当收到外部包时候触发
static int luapacket_register_on_external_packet(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
	if(!toluafix_isfunction(tolua_S, 1, "LUA_FUNCTION", 0, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luapacket_register_on_external_packet'.",&tolua_err);
		return 0;
	}
	if(gOnExternalPacket_delegate_index)
		toluafix_remove_function_by_refid(tolua_S, gOnExternalPacket_delegate_index);
	gOnExternalPacket_delegate_index = toluafix_ref_function(tolua_S,1,0);
	return 0;
}

//发包委托
static std::function<bool(uint32_t cid,packet* pkt)> internal_send_packet_delegate = nullptr;

static int luapacket_internal_send(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
	if(!tolua_isnumber(tolua_S,1,0,&tolua_err) || !tolua_isuserdata(tolua_S,2,0,&tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luapacket_send'.",&tolua_err);
		return 0;
	}
	
	uint32_t cid = luaL_checkint(L, 1);
	packet *pkt = (packet*)lua_touserdata(L,2);
	update_packet_len(pkt);

	ASSERT(internal_send_packet_delegate);

	bool ret = internal_send_packet_delegate(cid,pkt);
	
	tolua_pushboolean(tolua_S, ret);
	return 1;
}

static std::function<bool(void*,packet*)> external_send_packet_delegate = nullptr;

static int luapacket_external_send(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
	if(!tolua_isuserdata(tolua_S,1,0,&tolua_err) || !tolua_isuserdata(tolua_S,2,0,&tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luapacket_send'.",&tolua_err);
		return 0;
	}

	void *self = lua_touserdata(tolua_S, 1);
	packet *pkt = (packet*)lua_touserdata(L,2);
	update_packet_len(pkt);

	ASSERT(external_send_packet_delegate);

	bool ret = external_send_packet_delegate(self,pkt);

	tolua_pushboolean(tolua_S, ret);
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static lua_State *lua_packet_binding_L = nullptr;

int do_on_internal_packet(int cid, packet *pkt)
{
	int ret = 0;
	if (gOnInternalPacket_delegate_index)
	{
		LuaStackAutoPopup autoPopup(lua_packet_binding_L);
		toluafix_get_function_by_refid(lua_packet_binding_L,gOnInternalPacket_delegate_index);
		lua_pushnumber(lua_packet_binding_L, cid);
		lua_pushlightuserdata(lua_packet_binding_L, (void*)pkt);
		lua_call(lua_packet_binding_L, 2, 1);
		ret = (int)lua_tonumber(lua_packet_binding_L, -1);
	}
	return ret;
}

int do_on_external_packet(void *self, packet *pkt)
{	
	int ret = 0;
	if (gOnExternalPacket_delegate_index)
	{
		LuaStackAutoPopup autoPopup(lua_packet_binding_L);
		toluafix_get_function_by_refid(lua_packet_binding_L,gOnExternalPacket_delegate_index);
		lua_pushlightuserdata(lua_packet_binding_L, self);
		lua_pushlightuserdata(lua_packet_binding_L, (void*)pkt);
		lua_call(lua_packet_binding_L, 2, 1);
		ret = (int)lua_tonumber(lua_packet_binding_L, -1);
	}
	return ret;
}

int lua_open_packet_binding_init_delegate(std::function<bool(uint32_t cid,packet* pkt)> internal, std::function<bool(void*,packet*)> external_send)
{
	internal_send_packet_delegate = internal;
	external_send_packet_delegate = external_send;
	return 0;
}

int lua_open_packet_binding(lua_State *L)
{
	lua_packet_binding_L = L;

	int startPos = lua_gettop(L);
	luaL_Reg l[] = {
		{"init",		&luapacket_init},
		{"free",		&luapacket_free},		
		{"optcode",		&luapacket_optcode},

		{"write_str",   &luapacket_write_str},
		{"read_str",	&luapacket_read_str},
		{"write_str_by_len",   &luapacket_write_str_by_len},
		{"read_str_by_len",	&luapacket_read_str_by_len},

//		{"write_bytes",	&luapacket_write_bytes},
		
		{"read_u32",	&luapacket_read_u32},
		{"read_i32",	&luapacket_read_i32},
		{"read_u16",	&luapacket_read_u16},
		{"read_i16",	&luapacket_read_i16},
		{"read_u8",		&luapacket_read_u8},
		{"read_float",	&luapacket_read_float},
		{"read_double",	&luapacket_read_double},

		{"write_u32",   &luapacket_write_u32},
		{"write_i32",   &luapacket_write_i32},
		{"write_u16",   &luapacket_write_u16},
		{"write_i16",   &luapacket_write_i16},
		{"write_u8",	&luapacket_write_u8},
		{"write_float",	&luapacket_write_float},
		{"write_double",&luapacket_write_double},

		{"register_on_internal_packet",	&luapacket_register_on_internal_packet},
		{"register_on_external_packet",	&luapacket_register_on_external_packet},
		{"internal_send",		&luapacket_internal_send},
		{"external_send",		&luapacket_external_send},
		{"md5tostr",			&luamd5tostr},	
		{"base64decode",		&luabase64decode},	
		{"base64encode",		&luabase64encode},	

		{NULL, NULL}
	};
	luaL_register(L, "packet", l);	
	lua_settop(L,startPos);
	return 0;
}


#ifndef _LUA_PACKET_BINDING_H_
#define _LUA_PACKET_BINDING_H_

#include <shared/lua_helper.h>
#include <svrcore-internal.h>

int lua_open_packet_binding(lua_State *L);
int do_on_internal_packet(int cid, packet *pkt);
int do_on_external_packet(void *self, packet *pkt);

//初始化发包函数
int lua_open_packet_binding_init_delegate(std::function<bool(uint32_t cid,packet* pkt)> internal, std::function<bool(void*,packet*)> external_send);

#endif // !_LUA_PACKET_BINDING_H_

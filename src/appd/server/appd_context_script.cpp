#include "appd_app.h"
#include "appd_context.h"
#include "appd_script.h"
#include "appd_object_manager.h"

#include <protocol/internal.h>
#include <shared/log_handler.h>
#include <object/OprateResult.h>
#include <comm/crypt.h>
#include <tolua_fix.h>

//通过GUID获取玩家实例
int AppdContext::LuaGetPlayer(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	string guid = LUA_TOSTRING(scriptL, 1);
	AppdContext *player = AppdContext::FindPlayer(guid);
	if(player != NULL)
	{
		lua_pushlightuserdata(scriptL, player);
		lua_pushinteger(scriptL, player->GetStatus() == STATUS_LOGGEDIN ? PLAYER_ONLINE_STATE_ONLINE:PLAYER_ONLINE_STATE_OUTLINE);//是否在线
		return 2;
	}
	lua_pushnil(scriptL);
	lua_pushinteger(scriptL, PLAYER_ONLINE_STATE_OUTLINE);//不在线
	return 2;
}

//调用离线玩家，并作某些事
int AppdContext::LuaOfflineOper(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n >= 2);
	string callback_guid = LUA_TOSTRING(scriptL, 1);
	string callback_name = LUA_TOSTRING(scriptL, 2);
	auto ds = ObjMgr.FindDataSetByOwnerGuid(callback_guid);
	if(!ds || ds->GetAllData().empty())
	{
		//不存在，就发包去远程取吧
		WorldPacket pkt(INTERNAL_OPT_OFFLINE_OPER);
		pkt << callback_guid << 1 << callback_name;
		AppdApp::g_app->SendToLogind(pkt);
		return 0;
	}

	vector<GuidObject*> vec;
	ObjMgr.GetDataSetAllObject(callback_guid, vec);
	DoGetObjectsCallBack(vec, callback_guid, callback_name);
	return 0;
}

//锻造日志
int AppdContext::LuaWriteForgeLog(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	if(n != 7)
		return 0;
	AppdContext *player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	if (!player)
	{
		return 0;
	}
	const char *traget_id		= LUA_TOSTRING(scriptL,2);
	uint32 forge_type		= (uint32)LUA_TOINTEGER(scriptL,3);
	uint32 forge_property	= (uint32)LUA_TOINTEGER(scriptL,4);
	uint32 result_type		= (uint32)LUA_TOINTEGER(scriptL,5);
	string before_data		= LUA_TOSTRING(scriptL,6);
	string after_data		= LUA_TOSTRING(scriptL,7);

	WorldPacket pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TYPE_FORGE_LOG, player->GetAccount(), player->GetGuid());
	pkt << traget_id << forge_type << forge_property << result_type << before_data << after_data;

	//WorldPacket pkt(INTERNAL_OPT_ADD_FORGE_LOG);
	//pkt << player->GetAccount().c_str() << player->GetGuid().c_str() << player->GetName().c_str() << traget_id << forge_type << forge_property << result_type << before_data << after_data;
	AppdApp::g_app->SendToPoliced(pkt);
	return 0;
}

//写后台锻造升级日志
int AppdContext::LuaHTForgeLog(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	if(n != 3)
		return 0;
	const char *player_id	= LUA_TOSTRING(scriptL,1);
	uint32 forge_type		= (uint32)LUA_TOINTEGER(scriptL,2);
	uint32 forge_level		= (uint32)LUA_TOINTEGER(scriptL,3);
	WorldPacket pkt(INTERNAL_OPT_HT_FORGE_UP);
	pkt << player_id << forge_type << forge_level;
	AppdApp::g_app->SendToPoliced(pkt);
	return 0;
}

//修改金钱
int AppdContext::LuaModifyMoney(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n >= 4);
	AppdContext *player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	if (!player)
	{
		lua_pushboolean(scriptL, FALSE);
		return 1;
	}
	uint8 _type = (uint8)LUA_TONUMBER(scriptL, 2);
	uint8 oper_type = (uint8)LUA_TOINTEGER(scriptL, 3);
	double val = LUA_TONUMBER(scriptL, 4);
	string p1 = "";
	if(n >= 5)
		p1 = LUA_TOSTRING(scriptL, 5);
	int32 p2 = 0;
	if(n >= 6)
		p2 = (int32)LUA_TOINTEGER(scriptL, 6);
	int32 p3 = 0;
	if(n >= 7)
		p3 = (int32)LUA_TOINTEGER(scriptL, 7);
	uint8 p4 = 0;
	if(n >= 8)
		p4 = (int32)LUA_TOINTEGER(scriptL, 8);
	uint8 p5 = 0;
	if(n >= 9)
		p5 = (int32)LUA_TOINTEGER(scriptL, 9);

	if (val > 0)
	{
		player->addMoney(_type, oper_type, val, p1, p2, p3, p4, p5);
		lua_pushboolean(scriptL, TRUE);
	}
	else
	{
		if (player->subMoney(_type, oper_type, -1 * val, p1, p2, p3, p4, p5))
			lua_pushboolean(scriptL, TRUE);
		else
			lua_pushboolean(scriptL, FALSE);
	}
	
	return 1;
}

//获取金钱
int AppdContext::LuaGetMoney(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	AppdContext *player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	if (!player)
	{
		lua_pushboolean(scriptL, FALSE);
		return 1;
	}
	uint8 money_type = (uint8)LUA_TONUMBER(scriptL, 2);
	double money =  player->GetMoney(money_type);
	//TODO:对于欠钱的处理
	lua_pushnumber(scriptL,money);
	return 1;
}

int AppdContext::LuaSendAttr(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	AppdContext *player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	if (!player)
	{
		tea_perror("LuaSendAttr !player");
		return 0;
	}
	player->SetCalculFlag();
	return 0;
}

//使用元宝通知场景服做些什么
int AppdContext::LuaSendScenedConsumeMoney(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);

	AppdContext *player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	if (!player)
	{
		tea_perror("LuaSendScenedUseGold player == null");
		return 0;
	}
	uint8 money_type = (uint8)LUA_TONUMBER(scriptL, 2);
	uint8 use_type = (uint8)LUA_TONUMBER(scriptL, 3);
	string use_param = (string)LUA_TOSTRING(scriptL, 4);
	//通知场景服
	WorldPacket pkt(INTERNAL_OPT_SCENED_CONSUME_MONEY);
	pkt << player->GetGuid() << money_type << use_type << use_param;
	AppdApp::g_app->SendToScened(pkt, player->GetScenedFD());
	
	return 0;
}

// 通知登陆服托管登录
int AppdContext::LuaSendLoginHosting(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	AppdContext *player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	string hosting_guid = (string)LUA_TOSTRING(scriptL, 2);
	if (!player)
	{
		tea_perror("LuaSendLoginHosting player == null");
		return 0;
	}

	//通知场景服
	WorldPacket pkt(INTERNAL_OPT_LOGIND_HOSTING);
	pkt << player->GetGuid() << hosting_guid.c_str();
	AppdApp::g_app->SendToLogind(pkt);

	return 0;
}


// 脚本获得会话
int AppdContext::LuaGetFD(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	ASSERT(player);
	lua_pushnumber(scriptL, player->GetFD());
	return 1;
}

//获得被动技能id对应的binlog位置
int AppdContext::LuaGetPassiveIndex(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	uint16 spellId = (uint16)LUA_TONUMBER(scriptL, 2);
	ASSERT(player);

	uint16 index = 0;
	auto it = player->m_PassiveIndex.find(spellId);
	if (it != player->m_PassiveIndex.end()) {
		index = it->second;
	}
	lua_pushnumber(scriptL, index);

	return 1;
}

//更新被动技能的信息
int AppdContext::LuaUpdatePassive(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	uint16 spellId = (uint16)LUA_TONUMBER(scriptL, 2);
	uint8  spellLv = (uint8) LUA_TONUMBER(scriptL, 3);
	ASSERT(player);

	// 找到index
	uint16 index = 0;
	auto it = player->m_PassiveIndex.find(spellId);
	if (it != player->m_PassiveIndex.end()) {

		index = it->second;
		// 如果是删除技能
		if (spellLv == 0) {

			player->m_PassiveIndex.erase(spellId);
			player->m_SpellLevel.erase(spellId);
			player->SetUInt32(index, 0);

			return 0;
		}

	} else {
		for (int i = PLAYER_INT_FIELD_PASSIVE_SPELL_START; i < PLAYER_INT_FIELD_PASSIVE_SPELL_END; ++ i) {
			if (player->GetUInt16(i, 0) == 0) {
				index = i;
				break;
			}
		}
	}

	// 维护映射关系
	player->m_PassiveIndex[spellId] = index;
	player->m_SpellLevel[spellId] = spellLv;

	//存储到binlog
	if (player->GetUInt16(index, SHORT_SPELL_ID) != spellId) {
		player->SetUInt16(index, SHORT_SPELL_ID, spellId);
	}
	player->SetUInt16(index, SHORT_SPELL_LV, spellLv);

	return 0;
}


//更新技能槽技能的信息
int AppdContext::LuaUpdateSlotSpell(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	uint8  slot    = (uint8) LUA_TONUMBER(scriptL, 2);
	uint16 spellId = (uint16)LUA_TONUMBER(scriptL, 3);
	uint8  spellLv = (uint8) LUA_TONUMBER(scriptL, 4);
	ASSERT(player);

	uint32 emptyIndex = 0;
	
	uint32 size = player->uint32_values_size();
	for (uint32 i = PLAYER_INT_FIELD_SPELL_START; i < PLAYER_INT_FIELD_SPELL_END && i < size; ++ i) {
		uint8 sl = player->GetByte(i, BYTE_SLOT);

		if (sl == 0) {
			emptyIndex = i;
			break;
		}

		// 找到就更新
		if (sl == slot) {
			uint16 prevId = player->GetUInt16(i, SHORT_SLOT_SPELL_ID);

			// 连招永远不可能替换
			if (sl == 1 && prevId != spellId) {
				continue;
			}

			if (spellId > 0) {
				player->m_SpellLevel[spellId] = spellLv;
				if (prevId != spellId) {
					player->SetUInt16(i, SHORT_SLOT_SPELL_ID, spellId);
				}
				player->SetByte(i, BYTE_SLOT_SPELL_LV, spellLv);
			} else {
				player->SetUInt32(i, 0);
			}
			return 0;
		}
	}
	
	//没找到就增加
	if (emptyIndex > 0) {
		player->SetUInt16(emptyIndex, SHORT_SLOT_SPELL_ID, spellId);
		player->SetByte(emptyIndex, BYTE_SLOT_SPELL_LV, spellLv);
		player->SetByte(emptyIndex, BYTE_SLOT, slot);

		player->m_SpellLevel[spellId] = spellLv;
	}

	return 0;
}

int AppdContext::LuaAddSupportSpell(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	uint16 spellId = (uint16)LUA_TONUMBER(scriptL, 2);
	ASSERT(player);

	player->m_SpellLevel[spellId] = 1;
	player->l_supportSpell.push_back(spellId);

	return 0;
}

//获得技能id对应的等级
int AppdContext::LuaGetSpellLevel(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	uint16 spellId = (uint16)LUA_TONUMBER(scriptL, 2);
	ASSERT(player);

	uint32 level = 0;
	auto it = player->m_SpellLevel.find(spellId);
	if (it != player->m_SpellLevel.end()) {
		level = it->second;
	}

	lua_pushnumber(scriptL, level);

	return 1;
}

//设置技能id对应的等级
int AppdContext::LuaSetSpellLevel(lua_State* scriptL) {
	
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	uint16 spellId = (uint16)LUA_TONUMBER(scriptL, 2);
	uint8  spellLv = (uint8) LUA_TONUMBER(scriptL, 3);
	ASSERT(player);

	if (spellLv == 0) {
		player->m_SpellLevel.erase(spellId);
	} else {
		// 维护映射关系
		player->m_SpellLevel[spellId] = spellLv;
	}

	return 0;
}

// 获取推荐好友
int AppdContext::LuaGetAndSendRecommendFriends(lua_State* scriptL) {
	
	CHECK_LUA_NIL_PARAMETER(scriptL);
	
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	string guid = (string)LUA_TOSTRING(scriptL, 1);
	
	AppdContext *context = AppdContext::FindPlayer(guid);
	ASSERT(context);

	vector<string> ret;
	AppdContext::GetRecommendFriendGuid(ret, guid);

	vector<social_friend_info> list;
	for (string fguid : ret) {
		AppdContext* friendContext = AppdContext::FindPlayer(fguid);
		ASSERT(friendContext);
		social_friend_info stru;
		memset(&stru, 0 ,sizeof(stru));
		strcpy(stru.guid, friendContext->GetGuid().c_str());
		strcpy(stru.name, friendContext->GetName().c_str());
		strcpy(stru.faction, friendContext->GetFactionName().c_str());
		stru.level = friendContext->GetLevel();
		stru.icon = friendContext->GetGender();
		stru.vip = friendContext->GetVIP();

		list.push_back(stru);
	}

	Call_social_get_recommend_friend(context->m_delegate_sendpkt, list);

	return 0;
}

//设置修炼场信息
int AppdContext::luaSetCultivation(lua_State* scriptL){
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n >= 7);

	string guid = (string)LUA_TOSTRING(scriptL, 1);
	if (guid.empty()) {
		return 0;
	}
	uint32 time = (uint32)LUA_TONUMBER(scriptL, 2);
	uint32 lost = (uint32)LUA_TONUMBER(scriptL, 3);
	uint32 weapon = (uint32)LUA_TONUMBER(scriptL, 4);
	uint32 avatar = (uint32)LUA_TONUMBER(scriptL, 5);
	uint32 divine = (uint32)LUA_TONUMBER(scriptL, 6);
	uint32 vip = (uint32)LUA_TONUMBER(scriptL, 7);
	uint32 reverse1 = 0;
	uint32 reverse2 = 0;
	uint32 reverse3 = 0;
	uint32 reverse4 = 0;
	uint32 reverse5 = 0;
	if (n > 7){
		reverse1 = (uint32)LUA_TONUMBER(scriptL, 8);
	}
	if (n > 8){
		reverse2 = (uint32)LUA_TONUMBER(scriptL, 9);
	}
	if (n > 9){
		reverse3 = (uint32)LUA_TONUMBER(scriptL, 10);
	}
	if (n > 10){
		reverse4 = (uint32)LUA_TONUMBER(scriptL, 11);
	}
	if (n > 11){
		reverse5 = (uint32)LUA_TONUMBER(scriptL, 12);
	}

	std::vector<uint32> vec;
	vec.push_back(time);
	vec.push_back(lost);
	vec.push_back(weapon);
	vec.push_back(avatar);
	vec.push_back(divine);
	vec.push_back(vip);

	vec.push_back(reverse1);
	vec.push_back(reverse2);
	vec.push_back(reverse3);
	vec.push_back(reverse4);
	vec.push_back(reverse5);

	AppdApp::m_cultivationMap[guid] = vec;
	return 0;
}

//设置修炼场信息的某项
int AppdContext::luaSetCultivationByIndexValue(lua_State* scriptL){
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);

	string guid = (string)LUA_TOSTRING(scriptL, 1);
	if (guid.empty()) {
		return 0;
	}
	uint32 index = (uint32)LUA_TONUMBER(scriptL, 2);
	uint32 value = (uint32)LUA_TONUMBER(scriptL, 3);
 
	AppdApp::m_cultivationMap[guid][index] = value;
	return 0;
}
//获取修炼场信息
int AppdContext::luaGetCultivation(lua_State* scriptL){
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	string guid = (string)LUA_TOSTRING(scriptL, 1);
	if (guid.empty()) {
		return 0;
	}
	int32 flag = (int32)LUA_TONUMBER(scriptL, 2);

	if (AppdApp::m_cultivationMap.find(guid) == AppdApp::m_cultivationMap.end()) {
		return 0;
	}
	std::vector<uint32> info = AppdApp::m_cultivationMap.find(guid)->second;
	std::vector<uint32> vec;

	if (flag < 0){
		for (auto it : info) {
			vec.push_back(it);
		}
	}
	else{
		if ((uint32)flag >= info.size()){
			return 0;
		}
		vec.push_back(info[flag]);
	}

	lua_newtable(scriptL);    /* We will pass a table */
	int i = 0;
	std::for_each(vec.begin(), vec.end(),[&i,scriptL](uint32 it){
		lua_pushnumber(scriptL, i+1);   /* Push the table index */
		lua_pushnumber(scriptL, it);	
		lua_rawset(scriptL, -3);      /* Stores the pair in the table */
		i++;
	});

	return 1;
}

int AppdContext::luaFastGetFactionList(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);

	int n = lua_gettop(scriptL);
	ASSERT(n == 4)

	string guid = (string)LUA_TOSTRING(scriptL, 1);
	AppdContext *context = AppdContext::FindPlayer(guid);
	ASSERT(context);

	int16 page = (int16)LUA_TONUMBER(scriptL, 2);
	int16 num = (int16)LUA_TONUMBER(scriptL, 3);
	int16 limted = (int16)LUA_TONUMBER(scriptL, 4);

	std::vector<BinLogObject*> vec;
	//先筛选
	ObjMgr.ForeachIndexer(REGEX_FACRION, [limted, &vec](core_obj::GuidObject *obj){	
		if (limted == 0){
			vec.push_back(dynamic_cast<BinLogObject*>(obj));
		}else if(limted > 0){
			int lev = obj->GetUInt32(FACTION_INT_FIELD_MINLEV);
			if (limted >= lev){
				vec.push_back(dynamic_cast<BinLogObject*>(obj));
			}
		}

		return false;
	});
	//后排序
	std::sort(vec.begin(),vec.end(),[](const BinLogObject *a, const BinLogObject *b){                                                
		int la = a->GetUInt32(FACTION_INT_FIELD_LEVEL);
		int lb = b->GetUInt32(FACTION_INT_FIELD_LEVEL);
		if ( la > lb){
			return true;
		}else if(la == lb){
			if (a->GetUInt32(FACTION_INT_FIELD_PLAYER_NOW_INDEX) > b->GetUInt32(FACTION_INT_FIELD_PLAYER_NOW_INDEX))
			{
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	});

	

	unsigned int beg = (page-1) * num;
	unsigned int end = beg + num;

	if (beg >= vec.size() && vec.size() > 0){
		return 0;
	}
	if (end > vec.size()){
		end = vec.size();
	}
	vector<faction_info> list;
	for (unsigned int i=beg;i < end;i++)
	{
		BinLogObject *t = vec[i];
		faction_info stru;
		memset(&stru, 0 ,sizeof(stru));
		strcpy(stru.faction_guid, t->GetGuid().c_str());
		strcpy(stru.faction_name, t->GetName().c_str());
		strcpy(stru.faction_bz,t->GetStr(FACTION_STRING_FIELD_MANGER_NAME).c_str());
		strcpy(stru.faction_gg, t->GetStr(FACTION_STRING_FIELD_ZHAOMU_GONGGAO).c_str());
		stru.level = t->GetUInt32(FACTION_INT_FIELD_LEVEL);
		stru.icon = t->GetByte(FACTION_INT_FIELD_FLAGS_ID,0);
		stru.player_count = t->GetUInt32(FACTION_INT_FIELD_PLAYER_NOW_INDEX);
		stru.minlev = t->GetUInt32(FACTION_INT_FIELD_MINLEV);
		list.push_back(stru);
	}
	int allPage = int(ceil(vec.size()/float(num)));

	Call_faction_get_list_result(context->m_delegate_sendpkt, list,uint8(page),allPage);
	return 0;
}

int AppdContext::LuaGetSocialFriend(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);

	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	ASSERT(player);

	string guid = (string)LUA_TOSTRING(scriptL, 2);

	int16 ret = -1;
	auto it = player->l_socialFriendMap.find(guid);
	if (it != player->l_socialFriendMap.end()) {
		ret = it->second;
	}

	lua_pushnumber(scriptL, ret);

	return 1;
}


int AppdContext::LuaSetSocialFriend(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);

	int n = lua_gettop(scriptL);
	ASSERT(n == 3);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	ASSERT(player);

	string guid = (string)LUA_TOSTRING(scriptL, 2);
	int16 indx = (int16)LUA_TONUMBER(scriptL, 3);
	player->l_socialFriendMap[guid] = indx;

	return 0;
}

int AppdContext::LuaGetSocialEnemy(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);

	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	ASSERT(player);

	string guid = (string)LUA_TOSTRING(scriptL, 2);

	int16 ret = -1;
	auto it = player->l_socialEnemyMap.find(guid);
	if (it != player->l_socialEnemyMap.end()) {
		ret = it->second;
	}

	lua_pushnumber(scriptL, ret);

	return 1;
}


int AppdContext::LuaSetSocialEnemy(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);

	int n = lua_gettop(scriptL);
	ASSERT(n == 3);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	ASSERT(player);

	string guid = (string)LUA_TOSTRING(scriptL, 2);
	int16 indx = (int16)LUA_TONUMBER(scriptL, 3);
	player->l_socialEnemyMap[guid] = indx;

	return 0;
}

//获得指定技能等级
int AppdContext::LuaGetDesignatedSkillLevel(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);

	int n = lua_gettop(scriptL);
	ASSERT(n == 2);

	AppdContext* player = (AppdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	ASSERT(player);

	uint32 skillId = (uint32)LUA_TONUMBER(scriptL, 2);

	uint32 level = 0;
	if (skillId > 0) {
		if (player->m_SpellLevel.find(skillId) != player->m_SpellLevel.end()) {
			level = player->m_SpellLevel[skillId];
		}
	} else {
		for (auto it : player->m_SpellLevel) {
			if (level < it.second) {
				level = it.second;
			}
		}
	}
	lua_pushnumber(scriptL, level);

	return 1;
}

int AppdContext::__LuaAddString(lua_State *scriptL, vector<string>& ret)
{
	lua_newtable(scriptL);    /* We will pass a table */
	int i = 0;
	std::for_each(ret.begin(), ret.end(),[&i,scriptL](string it){
		lua_pushnumber(scriptL, i+1);   /* Push the table index */
		lua_pushstring(scriptL, it.c_str());	
		lua_rawset(scriptL, -3);      /* Stores the pair in the table */
		i++;
	});

	return 1;
}
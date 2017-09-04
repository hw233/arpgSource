#include "appd_app.h"
#include "appd_script.h"
#include <shared/probability_rand.h>
#include "appd_context.h"
#include <shared/log_handler.h>
#include <shared/lua_packet_binding.h>
#include "appd_object_manager.h"
#include <lua-lib.h>
#include "RankListManager.h"
#include "appd_mongodb.h"
#include <CCLuaStack.h>
#include <tolua/tolua++.h>
#include <shared/storage_loader.h>

LuaStack *gLuaStack = NULL;
lua_State *L = NULL;

static const struct luaL_reg mylib[] = { 

	{"outString",				__outString<AppdApp>}, 
	{"outDebug",				__outDebug<AppdApp>}, 
	{"outError",				__outError<AppdApp>}, 
	{"randInt",					__RandInt}, 
	{"randIntD",				__RandIntD}, 
	{"randIndex",				__RandIndex}, 

	{"getBit",					&LuaGetBit},
	{"setBit",					&LuaSetBit},
	{"gettimeofday",			&lua_gettimeofday},
	//{"open_robert_debugger",	&luaopen_robert_debugger},					//打开调试支持

	{"getCXXGlobalPtr",			&LuaGetCXXGlobalPtr},						//取得C++定义的全局变量		
	//{"writeLog",				&LuaWriteLog},								//日志统一接口
	//{"writeItemLog",			&LuaWriteItemLog},							//lua道具所有行为日志
	{"fuckPingBi",				&LuaFuckPingBi},							//关键词屏蔽接口
	{"loadFuckPingBi",			&LuaLoadFuckPingBi},						//加载屏蔽接口
	{"getEquipAttrsInfo",		&LuaGetEquipAttrsInfo},						//属性重算binlog

	{"getRankPlayerGuidByType",	&LuaGetRankPlayerGuidByType},				//获取排行榜玩家guid
	{"getMsTime",				&LuaGetMsTime},								//获取服务器运行时间
	
	{"playerLevelChanged",		&LuaLevelChanged},							//玩家等级改变

	{"rankInsertTask",			&LuaRankInsertTask},				//排行榜添加计划
	{"clearRankTask",			&LuaClearRankTask},					//清空排行榜
	{"OnUpdateRankList",		&LuaUpdateRankList},				//更新排行榜
	{"GetRankGuidTable",		&LuaGetRankGuidTable},				//更新排行榜
	{"RankHasGuid",				&LuaRankHasGuid},					//排行榜中是否有某个GUID数据
	{"UpdateRankLike",			&LuaUpdateRankLike},				//更新排行榜like数
	{"mongoInsert",				&LuaMongoInsert},				//lua调用c插入数据
	{"mongoUpdate",				&LuaMongoUpdate},				//lua调用c更新数据
	{"mongoQuery",				&LuaMongoQuery},				//lua调用c查询数据	
	{"mongoDelete",				&LuaMongoDelete},				//lua调用c删除数据
	
	{"SaveXiulianData",			&LuaSaveXiulianData},			//10分钟存储一次修炼记录
	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LBinLog[] =
{
	LUA_BINLOG_REG_LIBNAME(BinLogObject),
	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LPlayer[] = 
{
	{"GetPlayer",				&AppdContext::LuaGetPlayer},				//获取玩家
	{"ModifyMoney",				&AppdContext::LuaModifyMoney},				//操作玩家金钱
	{"GetMoney",				&AppdContext::LuaGetMoney},					//操作玩家金钱

	{"SendAttr",				&AppdContext::LuaSendAttr},					//更新装备/坐骑属性、技能、BUFF

	{"OfflineOper",				&AppdContext::LuaOfflineOper},				//离线操作

	{"SendScenedConsumeMoney",	&AppdContext::LuaSendScenedConsumeMoney},	//发送给场景服消耗money做些什么
	{"SendLoginHosting",		&AppdContext::LuaSendLoginHosting},			//通知登陆服托管登录

	{"GetFD",					&AppdContext::LuaGetFD},				//脚本获得会话

	{"GetPassiveIndex",					&AppdContext::LuaGetPassiveIndex},				//获得被动技能id对应的binlog位置
	{"UpdatePassive",					&AppdContext::LuaUpdatePassive},				//更新被动技能的信息
	{"UpdateSlotSpell",					&AppdContext::LuaUpdateSlotSpell},				//更新技能槽技能的信息
	

	{"GetSpellLevel",					&AppdContext::LuaGetSpellLevel},				//获得技能id对应的等级
	{"SetSpellLevel",					&AppdContext::LuaSetSpellLevel},				//设置技能id对应的等级

	{"AddSupportSpell",					&AppdContext::LuaAddSupportSpell},				//加辅助技能

	{"GetSocialFriend",					&AppdContext::LuaGetSocialFriend},				//获取好友idx
	{"SetSocialFriend",					&AppdContext::LuaSetSocialFriend},				//设置好友idx
	{"GetSocialEnemy",					&AppdContext::LuaGetSocialEnemy},				//获取仇人idx
	{"SetSocialEnemy",					&AppdContext::LuaSetSocialEnemy},				//设置仇人idx
	{"GetDesignatedSkillLevel",			&AppdContext::LuaGetDesignatedSkillLevel},		//获得指定技能等级
	
	{"GetAndSendRecommendFriends",		&AppdContext::LuaGetAndSendRecommendFriends},	// 获取推荐好友
	{"FastGetFactionList",				&AppdContext::luaFastGetFactionList},	// 获取帮派列表


	{"SetCultivation",		&AppdContext::luaSetCultivation},	// 设置修炼场信息
	{"GetCultivation",		&AppdContext::luaGetCultivation},	// 获取修炼场信息
	{"SetCultivationByIndexValue",		&AppdContext::luaSetCultivationByIndexValue},	// 获取修炼场信息



	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LLog[] = 
{
	{"WriteForgeLog",			&AppdContext::LuaWriteForgeLog},			//写锻造日志
	{"HTForgeLog",				&AppdContext::LuaHTForgeLog},			//写后台锻造升级日志
	{"WriteAttackPacker",		&LuaWriteAttackPacker},						//写攻击包日志
	{NULL, NULL} /* sentinel */ 
};

static const struct luaL_reg mylibname[] = 
{	
	{"binLogLib",		luaopen_lib<LBinLog>},
	{"logLib",			luaopen_lib<LLog>},
	{"playerLib",		luaopen_lib<LPlayer>},
	{NULL, NULL} /* sentinel */ 
};

extern int tolua_lua_mongo_wrap_binding_open(lua_State* tolua_S);
extern int  tolua_lua_core_obj_binding_open (lua_State* tolua_S);

lua_State *__script_init(const char* path)
{
	/*创建一个指向Lua解释器的指针。*/
	gLuaStack = new LuaStack;
	gLuaStack->init();
	L = gLuaStack->getLuaState();

	//对象管理器封装接口
	lua_open_objects_manger(L);

	/*函数加载Lua库*/
	for(const luaL_reg *ptr = mylib;ptr->name != NULL && ptr->func != NULL;ptr++)
		lua_register(L,ptr->name,ptr->func);

	for (const luaL_reg * lib = mylibname; lib->func; lib++) {
		lua_pushcfunction(L, lib->func);
		lua_pushstring(L, lib->name);
		lua_call(L, 1, 0);
	}

	gLuaStack->addSearchPath(path);

	/*设置包路径*/	
	
	char tmp[500];
	sprintf(tmp,"__SCRIPTS_ROOT__='%s'",path);		/*将脚本的路径注册进lua中*/
	gLuaStack->executeString(tmp);
	sprintf(tmp,"__RECHARGE_FOLDER__='%s'",g_Config.recharge_folder.c_str());		/*将充值保存路径注册进lua中*/
	gLuaStack->executeString(tmp);
	sprintf(tmp,"__OFFLINE_MAIL_FOLDER__='%s'",g_Config.mail_folder.c_str());		/*将离线邮件保存路径注册进lua中*/
	gLuaStack->executeString(tmp);

	////加载mongodb-driver
	//	luaopen_bson(L);
	//	luaopen_mongo_socket(L);
	//	luaopen_mongo_driver(L);	
	
	//加载http接口
	luaopen_lua_http_binding(L);
	//加载物品管理器接口
	luaopen_lua_item_binding(L);

	tolua_lua_core_obj_binding_open(L);
	tolua_lua_mongo_wrap_binding_open(L);

	//包处理函数
	lua_open_packet_binding_init_delegate([](uint32_t cid,packet* pkt)->bool{
		//TODO:单独处理发送给中心服的问题
		WorldPacket wPkt(*pkt);
		if (cid)
			AppdApp::g_app->SendToServerByFd(wPkt,cid);
		else
			AppdApp::g_app->SendToCentd(wPkt);
		return true;
	},[](void* self, packet* pkt)->bool{		
		//TODO:这里要考虑一下关于session指针及多继承的问题
		auto *ctx = (AppdContext*)self;
		ctx->SendPacket(*pkt);
		return true;
	});
	lua_open_packet_binding(L);

	/*加载脚本*/
	int status;
	sprintf(tmp, "%s/cow_appd.lua", path);
	if((status = luaL_dofile(L, tmp)))
	{
		tea_perror("lua error:Load the application script [%s] ERROR!", tmp);
		tea_perror("lua error:%s!\n", LUA_TOSTRING(L, -1));
		lua_pop(L, 1);
		scripts_free();
		return NULL;
	}

	ASSERT(lua_gettop(L) == 0);//不要有残留
	return L;
}


int scripts_init(const char* path)
{
	/*创建一个指向Lua解释器的指针。*/
	tea_pinfo("初始化默认LUA栈");
	L = __script_init(path);

	ASSERT(L);
	DoGetConfig(L);	
	return 0;
}

int scripts_free()
{
	if(gLuaStack != NULL)
	{
		delete gLuaStack;
		gLuaStack = NULL;
	}
	return 0;
}

int scripts_reload()
{
	//重载前先做点什么
	DoCloseServerSaveDB();

	scripts_free();
	scripts_init(g_Config.script_folder.c_str());
	AppdApp::g_app->RegSessionOpts();
	if(!AppdApp::g_app->IsPKServer())
	{
		DoStartServerLoadDB();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

//执行回调
int DoGetObjectsCallBack(vector<core_obj::GuidObject*> v, const string& callback_guid, const string &callback_string)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoGetObjectsCallBack");
	lua_pushstring(L, callback_guid.c_str());
	lua_newtable(L);

	for(GuidObject *it:v)
	{
		lua_pushstring(L, it->guid().c_str());
		lua_pushlightuserdata(L, (void *)it);	
		lua_rawset(L, -3);
	}
	
	if (LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("lua error:DoCorpsGlobalCheck error %s", callback_string.c_str());
		return 0;
	}
	
	return 0;
}

int LuaFuckPingBi(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);	
	if(n == 1)
	{
		const char* get_content = LUA_TOSTRING(scriptL, 1);
		//复制到动态内存中
		static char dup_content[8192];
		strcpy(dup_content, get_content);
		Pingbi(dup_content);
		lua_pushstring(scriptL, dup_content);	
		return 1;
	}
	return 0;
}

int LuaLoadFuckPingBi(lua_State *scriptL)
{
	AppdApp::g_app->LoadFuckPingbi(g_Config.storage_path);
	return 0;
}

//属性重算binlog
int LuaGetEquipAttrsInfo(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	if(n == 0)
	{
		lua_pushlightuserdata(scriptL, AppdApp::g_app->m_equip_attrs);
		return 1;
	}
	return 0;
}

bool LoadTemplate()
{	
	if(!Load_item_template(L))
		return false;

	if(!Load_skill_base(L))
		return false;
	
	return true;
}

//---------------------------------------------------------------------------------------------/
//获取配置信息
int DoGetConfig(lua_State *L)
{
	{
		LuaStackAutoPopup autoPopup(L);

		lua_getglobal(L,"config");
		if(!lua_istable(L,-1))
		{
			tea_perror("lua error:lua脚本中找不到配置信息");
			return -1;
		}
		lua_readvalue(L,"external_router_map",			AppdApp::g_app->m_opts, [](lua_State *scriptL, uint16 &op_code){
			op_code = (uint16)LUA_TOINTEGER(scriptL, -1);
			ASSERT(op_code);
		});
		lua_readvalue(L,"pk_external_router_map",			AppdApp::g_app->m_pk_opts, [](lua_State *scriptL, uint16 &op_code){
			op_code = (uint16)LUA_TOINTEGER(scriptL, -1);
			ASSERT(op_code);
		});
		lua_readvalue(L,"bag_init_size_main",			g_Config.bag_init_size_main);
		lua_readvalue(L,"bag_init_size_equip",			g_Config.bag_init_size_equip);
		lua_readvalue(L,"bag_init_size_storage",		g_Config.bag_init_size_storage);
		lua_readvalue(L,"bag_init_size_repurchase",		g_Config.bag_init_size_repurchase);
		lua_readvalue(L,"bag_init_size_system",			g_Config.bag_init_size_system);
		lua_readvalue(L,"bag_init_size_stall",		    g_Config.bag_init_size_stall);

		lua_readvalue(L,"bag_max_size_main",			g_Config.bag_max_size_main);
		lua_readvalue(L,"bag_max_size_storage",			g_Config.bag_max_size_storage);
		lua_readvalue(L,"bag_extension_count",			g_Config.bag_extension_count);
		lua_readvalue(L,"bag_extension_material",		g_Config.bag_extension_material);

		lua_readvalue(L,"player_max_level",				g_Config.player_max_level);
		
		lua_readvalue(L,"update_firend_info_interval",	g_Config.update_firend_info_interval);
		lua_readvalue(L,"update_detection_player_info", g_Config.update_detection_player_info);
		lua_readvalue(L,"player_chat_world_level",		g_Config.player_chat_world_level);
		lua_readvalue(L,"player_chat_whisper_level",	g_Config.player_chat_whisper_level);
		lua_readvalue(L,"laba_use_vip_level",			g_Config.laba_use_vip_level);
		lua_readvalue(L,"laba_use_need_money",			g_Config.laba_use_need_money);
		lua_readvalue(L,"rank_list_work_interval",		g_Config.rank_list_work_interval);
		lua_readvalue(L,"update_online_playerinfo_interval",	g_Config.update_online_playerinfo_interval);
	}

	return 0;
}

int LuaGetCXXGlobalPtr(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(   !tolua_iscppstring(tolua_S,1,0,&tolua_err))
		goto tolua_lerror;
	else
	{
		string clsName = ((string)  tolua_tocppstring(tolua_S,1,0));
		std::map<string,void*> all_instance;
		{			
			all_instance["MongoWrap"] = AppdApp::g_app->m_db_access_mgr;
			all_instance["AsyncMongoDB"] = AppdApp::g_app->m_db_access_mgr->get_async_db();
		}
		auto it = all_instance.find(clsName);
		if( it != all_instance.end())		
			tolua_pushusertype(tolua_S,it->second,it->first.c_str());		
	}	
	return 1;
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'LuaGetCXXGlobalPtr'.",&tolua_err);
	return 0;
}

int DoGMScripts(AppdContext *player, string &gm_commands)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoGMScripts");

	lua_pushlightuserdata(L, player);
	lua_pushstring(L,gm_commands.c_str());
	if(LUA_PCALL(L, 2, 1, 0))
	{
		tea_perror("lua error:DoGMScripts  ERROR %s",gm_commands.c_str());
		return 0;
	}
	string result_str = LUA_TOSTRING(L, -1);
	return 0;
}

//玩家限时活动初始化
int DoLimitActivityConsumption(AppdContext *player,uint32 money_type,double val)
{
	/*if(AppdApp::g_app->GetActivityVersion() == 0)
	{
		return 1;
	}
	string limit_script = AppdApp::g_app->GetActivityScript();
	if(limit_script.empty())
	{
		return 1;
	}*/
	LuaStackAutoPopup autoPopup(L);
	LuaGetObjFunc(L, player->GetGuid().c_str(), "OnConsumption");
	lua_pushnumber(L,money_type);
	lua_pushnumber(L,val);
	if(LUA_PCALL(L, 3, 0, 0))
	{
		tea_perror("lua error:DoLimitActivityConsumption %s", player->GetGuid().c_str());
		return 0;
	}
	return 0;
}

//GM命令的KEY转换
int GetGmCommandKey(string &gm_str)
{
	
	//lua_State *L = itemL;
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "GetGmCommandKey");
	lua_pushstring(L, gm_str.c_str());		
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("lua error:DoWingsBonus  ERROR");
		return 0;
	}
	return (int)LUA_TONUMBER(L,-1);
}

//玩家属性重算，包括物品加成、坐骑加成、军团加成
//TODO: Cal Attr
int DoCalculAttr(AppdContext* player, BinLogObject *attr)
{
	if (player == NULL)
	{
		tea_perror("lua error:DoCalculAttr  player == null");
		return 0;
	}
	
	
	LuaStackAutoPopup autoPopup(L);
	LuaGetObjFunc(L, player->GetGuid().c_str(), "DoCalculAttr");
	lua_pushlightuserdata(L, attr);

	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("lua error:DoCalculAttr player guid = %s ", player->GetGuid().c_str());
		return 0;
	}

	return 0;
}

//处理元宝消费问题
int DoGlodConsumeStatistics(AppdContext* player, double val)
{
	if(!player)
	{
		tea_perror("lua error:DoSetPresQuestCoin player is null");
		return 0;
	}
	LuaStackAutoPopup autoPopup(L);
	LuaGetObjFunc(L, player->GetGuid().c_str(), "DoGlodConsumeStatistics");
	lua_pushnumber(L, val);
	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("lua error:DoSetPresQuestCoin player guid: %s", player->GetGuid().c_str());
		return 0;
	}
	return 0;
}

//私聊后做点什么
int DoAfterChatWhisper(AppdContext* player1, AppdContext* player2)
{
	if(!player1 || !player2)
	{
		tea_perror("lua error:DoAfterChatWhisper player is null");
		return 0;
	}
	LuaGetObjFunc(L, player1->GetGuid().c_str(), "DoAfterChatWhisper");
	lua_pushstring(L, player2->GetGuid().c_str());
	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("lua error:DoAfterChatWhisper player guid: %s", player1->GetGuid().c_str());
		return 0;
	}
	return 0;
}

//获取排行榜玩家guid
int LuaGetRankPlayerGuidByType(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n==2);
	int type = (int)LUA_TONUMBER(scriptL, 1);
	uint32 rank = (uint32)LUA_TOINTEGER(scriptL, 2);
	string player_guid = AppdApp::g_app->GetRankPlayerGuidByType(type, rank);
	lua_pushstring(scriptL, player_guid.c_str());
	return 1;
}

int LuaSaveXiulianData(lua_State *scriptL) {
	AppdApp::saveXiulian();
	return 1;
}

//获取服务器运行时间
int LuaGetMsTime(lua_State *scriptL)
{	
	lua_pushnumber(scriptL, ms_time());
	return 1;
}

int LuaLevelChanged(lua_State *scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);

	string player_guid	= (string)LUA_TOSTRING(scriptL, 1);
	uint32 prevLevel	= (uint32)LUA_TONUMBER(scriptL, 2);
	uint32 currLevel	= (uint32)LUA_TONUMBER(scriptL, 3);
	
	AppdContext::LevelChanged(player_guid, prevLevel, currLevel);

	return 0;
}

//合服后数据处理
int DoAppdMergeSomething()
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "AppdMergeSomething");
	if(LUA_PCALL(L, 0, 0, 0))
	{
		tea_perror("AppdMergeSomething:  ERROR");
		return 0;
	}
	return 0;
}

//排行榜新增计划
int LuaRankInsertTask(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	string guid = LUA_TOSTRING(scriptL, 1);
	uint32 rank_type = LUA_TOINTEGER(scriptL, 2);
	if(RankListMgr)
	{
		RankListMgr->InsertTask(guid,(ERankTypes)rank_type);
	}	
	return 1;
}

//清空排行榜
int LuaClearRankTask(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	uint32 rank_type = LUA_TOINTEGER(scriptL, 1);
	if(RankListMgr)
	{
		RankListMgr->ClearRankList((ERankTypes)rank_type);
	}	
	return 1;
}

// 更新排行榜
int LuaUpdateRankList(lua_State *scriptL) {
	if(RankListMgr) {
		RankListMgr->UpdateRankList();
	}
	return 0;
}

//获取排行榜Guid列表
int LuaGetRankGuidTable(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	uint32 type = (uint32)LUA_TONUMBER(scriptL, 1);
	

	lua_newtable(scriptL);    /* We will pass a table */

	vector<string> guidList = RankListMgr->GetRankGuidList((ERankTypes)type);
	int i = 0;
	for (uint32 j=0;j<guidList.size();j++)
	{
		lua_pushnumber(scriptL, i+1);   /* Push the table index */
		lua_pushstring(scriptL, guidList[j].c_str());
		lua_rawset(scriptL, -3);      /* Stores the pair in the table */
		i ++;
	}

	return 1;
}

//获取排行榜Guid列表
int LuaRankHasGuid(lua_State* scriptL) {

	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	uint32 type = (uint32)LUA_TONUMBER(scriptL, 1);
	string guid	= (string)LUA_TOSTRING(scriptL, 2);

	//lua_newtable(scriptL);    /* We will pass a table */
	int rank = -1;
	bool tf = RankListMgr->HasRankGuid((ERankTypes)type,guid,rank);
	//int i = 0;
	//for (uint32 j=0;j<guidList.size();j++)
	//{
	//	lua_pushnumber(scriptL, i+1);   /* Push the table index */
	//	lua_pushstring(scriptL, guidList[j].c_str());
	//	lua_rawset(scriptL, -3);      /* Stores the pair in the table */
	//	i ++;
	//}
	lua_pushboolean(scriptL,tf);
	lua_pushnumber(scriptL,rank);


	return 2;
}



extern int LuaUpdateRankLike(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	uint32 type = (uint32)LUA_TONUMBER(scriptL, 1);
	string guid	= (string)LUA_TOSTRING(scriptL, 2);
	uint32 num = (uint32)LUA_TONUMBER(scriptL, 3);
	

	RankListMgr->UpdateRankLike(type,guid,num);

	return 1;
}

//服务器启动后，需要loadDB的一些数据
int DoStartServerLoadDB()
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "StartServerLoadDB");
	if(LUA_PCALL(L, 0, 0, 0))
	{
		tea_perror("StartServerLoadDB:  ERROR");
		return 0;
	}
	return 0;
}

//服务器关闭前，保存lua一些数据
int DoCloseServerSaveDB()
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "CloseServerSaveDB");
	if(LUA_PCALL(L, 0, 0, 0))
	{
		tea_perror("CloseServerSaveDB:  ERROR");
		return 0;
	}
	return 0;
}

//玩家充值以后执行的LUA
int DoAddRechargeSum(AppdContext *player, const string& recharge_id, double val, uint32 time)
{
	LuaStackAutoPopup autoPopup(L);
	LuaGetObjFunc(L, player->GetGuid().c_str(), "DoAddRechargeSum");
	lua_pushstring(L, recharge_id.c_str());
	lua_pushnumber(L, val);
	lua_pushnumber(L, time);
	if(LUA_PCALL(L, 4, 0, 0))
	{
		tea_perror("lua error:DoAddRechargeSum: %s,%s,%u,%u", player->GetGuid().c_str(), recharge_id.c_str(), val, time);
		return 1;
	}
	return 0;
}

//数据库插入
int LuaMongoInsert(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	string ns = LUA_TOSTRING(scriptL, 1);		//如：world.showhand_dairulinghui_log
	if (!lua_istable(scriptL, 2))
		return 0;
	std::map<string,string> values;
	int t_idx = lua_gettop(scriptL);
	lua_pushnil(scriptL);
	while(lua_next(scriptL, t_idx))
	{
		if (!lua_isstring(scriptL, -2))
			return 0;		//非字符串就忽略
		
		string key = LUA_TOSTRING(scriptL, -2);
		string value = LUA_TOSTRING(scriptL, -1);
		values[key] = value;
		lua_pop(scriptL, 1);
	}

	g_DAL.Insert(ns, values);
	return 0;
}

//数据库更新
int LuaMongoUpdate(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	string ns = LUA_TOSTRING(scriptL, 1);		//如：world.showhand_dairulinghui_log
	if (!lua_istable(scriptL, 2) || !lua_istable(scriptL, 3))
		return 0;
	std::map<string,string> where;
	std::map<string,string> values;
	int t_idx = 2;
	lua_pushnil(scriptL);
	while(lua_next(scriptL, t_idx))
	{
		if (!lua_isstring(scriptL, -2))
			return 0;		//非字符串就忽略

		string key = LUA_TOSTRING(scriptL, -2);
		string value = LUA_TOSTRING(scriptL, -1);
		where[key] = value;
		lua_pop(scriptL, 1);
	}
	t_idx = 3;
	lua_pushnil(scriptL);
	while(lua_next(scriptL, t_idx))
	{
		if (!lua_isstring(scriptL, -2))
			return 0;		//非字符串就忽略

		string key = LUA_TOSTRING(scriptL, -2);
		string value = LUA_TOSTRING(scriptL, -1);
		values[key] = value;
		lua_pop(scriptL, 1);
	}

	g_DAL.Update(ns, where, values);
	return 0;
}


//数据库查询
int LuaMongoQuery(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int t_idx = lua_gettop(scriptL);
	ASSERT(t_idx == 2);
	string ns = LUA_TOSTRING(scriptL, 1);		//如：world.showhand_dairulinghui_log
	std::map<string,string> where;
	vector<map<string,string>> results;

	if (lua_istable(scriptL, 2))
	{
		lua_pushnil(scriptL);
		while(lua_next(scriptL, t_idx))
		{
			string key = LUA_TOSTRING(scriptL, -2);
			string value = LUA_TOSTRING(scriptL, -1);
			where[key] = value;
			lua_pop(scriptL, 1);
		}	
	}

	lua_newtable(scriptL);
	g_DAL.Query(ns, where, results);
	if (results.size() > 0)
	{
		int index = 0;
		for (auto it:results)
		{
			lua_pushinteger(scriptL, ++index);
			lua_newtable(scriptL);
			for (auto r:it)
			{
				lua_pushstring(scriptL, r.first.c_str());
				lua_pushstring(scriptL, r.second.c_str());	
				lua_rawset(scriptL, -3);
			}
			lua_rawset(scriptL, -3);
		}
	}

	return 1;
}


//数据库删除
int LuaMongoDelete(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int t_idx = lua_gettop(scriptL);
	ASSERT(t_idx == 2);
	string ns = LUA_TOSTRING(scriptL, 1);		//如：world.showhand_dairulinghui_log
	std::map<string,string> values;
	if (lua_istable(scriptL,t_idx))
	{
		lua_pushnil(scriptL);
		while(lua_next(scriptL, t_idx))
		{
			if (lua_isstring(scriptL, -2))
			{
				string key = LUA_TOSTRING(scriptL, -2);
				string value = LUA_TOSTRING(scriptL, -1);
				values[key] = value;
				lua_pop(scriptL, 1);
			}
		}
	}
	if (values.size() == 0)
		return 0;

	g_DAL.Delete(ns, values);
	return 0;
}


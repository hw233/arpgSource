#ifndef _SCENED_CONTEXT_H_
#define _SCENED_CONTEXT_H_

#include "appd_app.h"
#include <object/SharedDef.h>
#include <object/PlayerBase.h>
#include <vector>

class ItemManager;

//增加在线时长更新时间
#define  ONLING_TIME     60000

typedef std::map<uint16, uint16> PassiveIndexMap;
typedef std::map<uint16, uint8> SpellLevelMap;
typedef std::list<uint16> SupportSpellList;
typedef std::map<string, int16> SocialMap;

#define EXPIRE_SHIFT 8

//应用服会话 上下文信息
class AppdContext:public PlayerBase,public SvrContext
{
	//静态
public:
	static AppdContext *FindPlayer(const string &guid);	
	static AppdContext *FindAccount(const string &account);
	typedef UNORDERED_MAP<string, AppdContext*> ContextStrMap;
	static ContextStrMap AccountMaps;

	// 给推荐好友来个等级维护列表
	typedef std::map<uint8, std::set<string>> RecommendMap;
	static RecommendMap m_recommend;
	
	static int LuaGetAndSendRecommendFriends(lua_State* scriptL);	//获取推荐好友
	static int luaFastGetFactionList(lua_State* scriptL);	//获取帮派列表
	static void GetRecommendFriendGuid(vector<string>& ret, const string& guid);
	static void LevelChanged(const string& guid, uint32 prevLevel, uint32 currLevel);
	static void AddNew(const string& guid, uint32 currLevel);
	static void RemoveOld(const string& guid, uint32 prevLevel);

	static int luaSetCultivation(lua_State* scriptL);
	static int luaSetCultivationByIndexValue(lua_State* scriptL);
	static int luaGetCultivation(lua_State* scriptL);
	
public:
	AppdContext(int fd,ContextManager *mgr);
	virtual ~AppdContext();

	//函数路由,初始化
	void OpcodeHandlerInitor();
	void InsertPlayerMap();		//以guid为key维护成另外一个context表	
	void RemovePlayerMap();

	void Update(uint32 diff);

	bool Login(uint32 fd);
	void Logout();

	void online();
	void offline();

	int OnLevelChanged();

	int OnForceChanged();

	//货币相关
	double GetMoney(uint16 type);
	void addMoney(uint8 type, uint8 oper_type, double val, const string &trace_id = "", uint32 p1 = 0, uint32 p2 = 0, uint8 item_bind = 0, uint8 item_del = 0);
	bool subMoney(uint8 type, uint8 oper_type, double val, const string &trace_id = "", uint32 p1 = 0, uint32 p2 = 0, uint8 item_bind = 0, uint8 item_del = 0);

	//保存元宝消费信息
	void SaveGoldLog(const string &account, const string &player_id,  const string &player_name, uint16 money_type, uint16 oper_type,
		double v,double old_value, double new_value, const string &trace_id, uint32 item_id, uint32 count, 
		double unit_price,uint16 item_bind, uint16 item_del, uint32 level, uint32 quest);


	//玩家登录做点什么
	int DoPlayerLogin();
	//玩家下线做点什么
	int DoPlayerLogout();
	// 判断是否是朋友
	bool isFriend(const string& fguid);

	void OnAddFirstQuest();

protected:
	void Handle_LUA(packet& pkt);

	void Handle_Rank_List_Query(packet& pkt);			//请求排行榜信息

	void Handle_Char_Remotestore(packet& pkt);			//保存客户端配置信息
	void Handle_Char_Remotestore_Str(packet& pkt);		//保存客户端配置信息

	void Handle_Syns_Mstime(packet& pkt);			//同步时间
public:
	void Call_Operation_Failed(uint16 type,uint16 reason, string data = "");						//操作失败
public:
	// 被动技能index映射
	PassiveIndexMap m_PassiveIndex;
	
	// 技能对应的等级
	SpellLevelMap m_SpellLevel;

	// 辅助技能列表
	SupportSpellList l_supportSpell;

	SocialMap l_socialFriendMap;
	SocialMap l_socialEnemyMap;
	
	// 首次登录同步初始技能
	void firstLoginAndCopySpellInfoToGuidObject(GuidObject* obj);

	void firstLoginAndInitQuestGuidObject();
	
	//产生临时技能信息 方便应用服处理技能逻辑
	void generateTempSpellLevelInfo(GuidObject* obj);

	//产生临时被动技能binlog位置信息
	void generatePassiveIndexInfo();

	// 生成辅助技能信息
	void mapSupportSpell(uint16 spellId, uint8 spellLv);


	void SetCalculFlag()
	{
		m_isCanRecalcul = true;
	}
private:
	bool m_isCanRecalcul;						//是否可以重算
	//计算并下发装备/坐骑属性、技能、BUFF
	void SendEquipAttrs();						//ps：仅供类内部使用

	void NewOtherBinlog();
	void NewOtherBinlog(const string new_guid, uint32 flag, std::function<BinLogObject*()> create_fun = nullptr);
	void NewOtherBinlog(EObjectTypePrefix t, uint32 flag, std::function<BinLogObject*()> create_fun = nullptr);
	void CreateOtherBinlog(const string new_guid, uint32 flag, std::function<BinLogObject*()> create_fun = nullptr);

private:
	//钱相关
	vector<double> m_all_money;
public:
	static int LuaGetPlayer(lua_State* scriptL);			//通过GUID获取玩家实例
	static int LuaOfflineOper(lua_State *scriptL);			//调用离线玩家，并作某些事

	static int LuaWriteForgeLog(lua_State* scriptL);		//锻造日志
	static int LuaHTForgeLog(lua_State* scriptL);			//写后台锻造升级日志
	
	static int LuaModifyMoney(lua_State* scriptL);			//修改金钱
	static int LuaGetMoney(lua_State* scriptL);			//获取金钱
		
	static int LuaSendAttr(lua_State* scriptL);					//更新装备/坐骑属性、技能、BUFF

	static int LuaSendScenedConsumeMoney(lua_State* scriptL);	//使用元宝通知场景服做些什么
	static int LuaSendLoginHosting(lua_State* scriptL);		//通知登陆服托管登录

	static int LuaGetFD(lua_State* scriptL);	// 脚本获得会话

	static int LuaGetPassiveIndex(lua_State* scriptL);	//暂时没用 获得被动技能id对应的binlog位置
	static int LuaUpdatePassive(lua_State* scriptL);	//更新被动技能的信息
	static int LuaUpdateSlotSpell(lua_State* scriptL);	//更新技能槽技能的信息
	
	static int LuaActiveDivineSpell(lua_State* scriptL);//更新神兵技能的信息

	static int LuaAddSupportSpell(lua_State* scriptL);	//增加被动技能

	static int LuaGetSpellLevel(lua_State* scriptL);	//获得技能id对应的等级
	static int LuaSetSpellLevel(lua_State* scriptL);	//设置技能id对应的等级

	static int LuaGetSocialFriend(lua_State* scriptL);//获取好友 index
	static int LuaSetSocialFriend(lua_State* scriptL);//设置好友 index
	static int LuaGetSocialEnemy(lua_State* scriptL);//获取好友 index
	static int LuaSetSocialEnemy(lua_State* scriptL);//设置好友 index

	static int LuaGetDesignatedSkillLevel(lua_State* scriptL);	//获得指定技能等级

	static int __LuaAddString(lua_State* scriptL, vector<string> &ret);
};


#endif

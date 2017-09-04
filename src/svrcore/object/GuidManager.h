#ifndef _GUID_MANAGER_H_
#define _GUID_MANAGER_H_

#include <object/BinLogObject.h>
#include <svrcore-internal.h>
#include <comm/cstr_util.h>

//////////////////////////////////////////////////////////////////////////
//所有对象的类型枚举
enum EObjectTypePrefix
{
	ObjectTypeNone = ' ',			//无效的前缀

	ObjectTypeAccount = 'A',		//账户信息
	ObjectTypeGlobalValue = 'G',	//全局对象
	ObjectTypeGroup = 'T',			//组队

	ObjectTypeUnit = 'U',			//生物
	ObjectTypePlayer = 'p',			//玩家
	ObjectTypeItemMgr = 'I',		//物品
	ObjectTypeLimit = 'l',			//限时活动对象
	ObjectTypeSocial = 's',			//社交
	ObjectTypeQuest = 'Q',			//任务
	ObjectTypeLogical = 'B',		//业务逻辑
	ObjectTypeSpell	= 'X',			//技能
	ObjectTypeFaction = 'L',		//帮派
	ObjectTypeFactionData = 'D',	//帮派数据
	ObjectTypeGiftPacks = 'g',		//礼包
	ObjectTypePKinfo	= 'k',		//玩家PK记录	
	ObjectTypeMap					= 'M',			//地图变量
	ObjectTypeGridLoot				= 'O',			//地图上的grid战利品信息
	ObjectTypeInstance				= 'C',			//副本

	ObjectTypeMapPlayerInfo = 'u',	//任意地图实例信息(目前主要是在线玩家列表),前缀被用光了，这个将就点用吧

	//以下纯粹是为了借用机制，所以搞些符号代替
	ObjectTypeMapInstance = '#',	//地图实例的id规则
	ObjectTypeGMCommand	 = '@',		//GM命令产生
};

#define SUB_GUID_STR_TO_INT(g) g.substr(1, g.find('.'))

//根据传送的guid取得短guid,一般用于unit
inline uint32 shortGUID(const char* s)
{
	char c;
	uint32 t;
	if (s == nullptr) {
		return 0;
	}
	sscanf(s,"%c%d.",&c, &t);
	return t;
}

class GuidManager
{
public:
	GuidManager();
	~GuidManager();
public:
	//这里不负责分配内存哦
	const char* MAKE_NEW_GUID(EObjectTypePrefix prefix,uint32 index,const char *suffix);	

	inline string NewGUID(EObjectTypePrefix prefix,uint32 index,const char *suffix)
	{
		return MAKE_NEW_GUID(prefix,index,suffix);
	}

	inline const char* MAKE_NEW_GUID(EObjectTypePrefix prefix,string suffix = "")
	{
		return MAKE_NEW_GUID(prefix,NewIndex(prefix),suffix.c_str()); 
	}

	static EObjectTypePrefix GetPrefix(const string& str)
	{
		return GetPrefix(str.c_str());
	}

	static EObjectTypePrefix GetPrefix(const char* guid)
	{
		if(guid[0]=='\0')
			return ObjectTypeNone;
		return EObjectTypePrefix(guid[0]);
	}
	
	inline const char* GetSuffixFromGUID(const char* guid)
	{
		char *s = cstr_read_token((char*)guid,'.');
		return s;
	}

	//返回GUID中的跳号
	inline const char* GetJumpNo(const char* guid);


	static string ReplaceSuffix(const string& guid,EObjectTypePrefix t);

	static string ScenePrefixRegex(const string& guid);

	//
	uint32 NewIndex(EObjectTypePrefix t);

	void LoadMyData();
	void SyncMaxGuid(uint32 player_max);	//同步guid累加最大值
	void SetUnitGuid(uint32 unit_id);
public:
	BinLogObject *m_data;		//guid管理数据

private:
	BinLogObject *m_my_data;
	char m_tmp[128];
};

#endif

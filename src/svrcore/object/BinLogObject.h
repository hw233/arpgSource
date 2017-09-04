#ifndef _BIN_LOG_OBJECT_H_
#define _BIN_LOG_OBJECT_H_

#include <svrcore-internal.h>
#include <core_obj/GuidObject.h>
#include <core_obj/UpdateMask.h>
#include "SharedDef.h"

#define REGEX_GLOBAL			"^G\\.[[:alpha:]]"
#define REGEX_PLAYER			"^p[[:digit:]]"		//玩家对象
#define REGEX_MAPPLAYERINFO		"^u[[:digit:]]"		//地图内玩家信息
#define REGEX_MAP				"^M[[:digit:]]"		//地图binlog
#define REGEX_LIMIT				"^l[[:digit:]]"		//限时活动
#define REGEX_WORLD				"^w"				//世界服Binlog
#define REGEX_FACRION			"^L[[:digit:]]"		//帮派对象
struct lua_State;		//for script

#define GET_BIT(v,i) (((v)>>(i))&0x1)
#define SET_BIT(v,i) ((1<<(i))|v)
#define UNSET_BIT(v,i) (~(1<<(i))&v)

using core_obj::GuidObject;
using core_obj::UpdateMask;

class BinLogObject:public core_obj::GuidObject
{
public:
	BinLogObject(uint32_t _m = core_obj::SYNC_SLAVE):core_obj::GuidObject(_m),m_db_hashcode(0),m_debug_deleted(0)
	{
		//SetBinlogMaxSize(MAX_BINLOG_SIZE_UNLIME);
	}

	virtual ~BinLogObject()
	{
		m_debug_deleted=-1;
	}

	//操作记录为空
	bool BinlogEmpty(){return binlogs_.empty();}

	//默认胜于配置,当存在guid时,永远位于第一个
	const string &GetGuid() const 
	{
		return guid_;
	}
	
	//
	inline void SetGuid(string s)
	{
		guid(s);
		SetStr(BINLOG_STRING_FIELD_GUID, s);
	}

	//默认胜于配置,Name时,永远位于第2个
	const string& GetName() const 
	{
		return GetStr(BINLOG_STRING_FIELD_NAME);
	}

	void SetName(const string &name)
	{
		SetStr(BINLOG_STRING_FIELD_NAME,name);
	}

	// 获得主人ID
	const string &GetOwner()const
	{
		return GetStr(BINLOG_STRING_FIELD_OWNER);
	}

	void SetOwner(const string &val) 
	{
		SetStr(BINLOG_STRING_FIELD_OWNER,val);
	}

	const string &GetVersion()const
	{
		return GetStr(BINLOG_STRING_FIELD_VERSION);
	}
	
	//上一次保存到数据库时的特征码，用于是否需要保存
	size_t m_db_hashcode;
	//没有作用,用于调试,内存	
	int m_debug_deleted;			
};

//从ByteArray转换出数据包
void WriteTo(core_obj::ByteArray &src,packet& dst);
void ReadFrom(core_obj::ByteArray &dst,packet& src);

#endif

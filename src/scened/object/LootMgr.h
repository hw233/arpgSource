#ifndef _LOOT_OBJECT_H_
#define _LOOT_OBJECT_H_

#include "GameObject.h"

struct Grid;
class Map;

class LootObject: public BinLogObject
{
public:
	LootObject(Grid *g);
	~LootObject();
public:	
	void Update(uint32 diff);
	void Init(Map *m, int index);
	uint32 GetUIntGuid(){return m_uint_guid;}
	void PickUpLoot(ScenedContext *player, uint32 index, double &money);	
	
	BinLogObject *GetPrivateObj(){return m_private;}
	int GetLootIndex(int x, int y);
	int GetMaxIndex();

	void WriteCreateBlock(ByteArray &);

	uint32 GetEntry(uint32 index){return GetUInt32(LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_ENTRY);}
	uint32 GetCount(uint32 index){return GetUInt32(LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_MONEY_SUM);}

	std::set<uint32>				m_has_loot;		//拥有战利品的格子
protected:
	uint32 m_uint_guid;			//整形guid
	Map *m_map;
	Grid *m_grid;

	BinLogObject *m_private;

	int m_width;
	void GetLootPos(uint32 index, int &x, int &y);
public:
	void ClearLoot(uint32 index);
private:
	//public
	uint16 GetStartX(){return GetUInt16(LOOT_PUBLIC_INT_START_POS, 0);}
	void SetStartX(uint16 v){SetUInt16(LOOT_PUBLIC_INT_START_POS, 0, v);}
	uint16 GetStartY(){return GetUInt16(LOOT_PUBLIC_INT_START_POS, 1);}
	void SetStartY(uint16 v){SetUInt16(LOOT_PUBLIC_INT_START_POS, 1, v);}
	uint16 GetEndX(){return GetUInt16(LOOT_PUBLIC_INT_END_POS, 0);}
	void SetEndX(uint16 v){SetUInt16(LOOT_PUBLIC_INT_END_POS, 0, v);}
	uint16 GetEndY(){return GetUInt16(LOOT_PUBLIC_INT_END_POS, 1);}
	void SetEndY(uint16 v){SetUInt16(LOOT_PUBLIC_INT_END_POS, 1, v);}
	
	void SetEntry(uint32 index, uint32 v){SetUInt32(LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_ENTRY, v);}	
	void SetCount(uint32 index, uint32 v){SetUInt32(LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_MONEY_SUM, v);}
	const string &GetLootOwner(uint32 index){return GetStr(LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PUBLIC_STR_FIELD + LOOT_PUBLIC_STR_OWNER);}
	void SetLootOwner(uint32 index, const string &str){return SetStr(LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PUBLIC_STR_FIELD + LOOT_PUBLIC_STR_OWNER, str);}
	//private
	//物品失效时间
	uint32 GetFailTime(uint32 index){return m_private->GetUInt32(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_FAIL_TIME);}
	void SetFailTime(uint32 index, uint32 v){m_private->SetUInt32(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_FAIL_TIME, v);}
	//标志位
	bool GetFlag(uint32 index, uint8 f){return m_private->GetBit(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_FLAG, f);}
	void SetFlag(uint32 index, uint8 f){m_private->SetBit(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_FLAG, f);}
	void UnSetFlag(uint32 index, uint8 f){m_private->UnSetBit(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_FLAG, f);}
	//是否拾取广播
	bool IsGetItemNotice(uint32 index){return GetFlag(index, LOOT_FLAG_GET_ITEM_NOTICE);}
	//掉落怪物名称
	const string &GetCreateName(uint32 index){return m_private->GetStr(LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PRIVATE_STRING + LOOT_PRIVATE_STRING_CREATE_NAME);}
	void SetCreateName(uint32 index, const string &str){m_private->SetStr(LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PRIVATE_STRING + LOOT_PRIVATE_STRING_CREATE_NAME, str);}
	//战利品消失倒计时
	int32 GetExistTime(uint32 index){return m_private->GetInt32(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_EXIST_TIME);}
	void SubExistTime(uint32 index, uint32 v){m_private->SubInt32(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_EXIST_TIME, v);}
	//战利品拥有者保护倒计时
	int32 GetOwnerTime(uint32 index){return m_private->GetInt32(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_OWNER_TIME);}
	void SubOwnerTime(uint32 index, uint32 v){m_private->SubInt32(index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_OWNER_TIME, v);}
public:

};

#endif


#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Unit.h"

class LootManager;

//typedef UNORDERED_MAP<uint32,uint32> GAMEOBJ_GUID_MAP;
class GameObject : public Unit
{
public:
    GameObject();
    virtual ~GameObject();
    bool Create(const string &lguid, uint32 entry);
	void UpdateLiveStatus(uint32 diff);
	void SetSpellCD(uint32 id){};
	uint32 m_respawn_time;							//重生时间
private:
	bool m_is_garbage;
public:
	bool IsGarbage(){return m_is_garbage;}
	void SetGarbage(bool value){m_is_garbage = value;}
public:
    bool IsTeleteport()
    {
        return GetBit(GO_FIELD_FLAGS, GO_FLAG_TELE);
    }
    bool IsQuestGO()
    {
        return GetBit(GO_FIELD_FLAGS, GO_FLAG_QUEST);
    }

    bool CanUse()
    {
        return !GetBit(GO_FIELD_FLAGS, GO_FLAG_UNUSE);
    }
    bool CanLoot()
    {
        return GetBit(GO_FIELD_FLAGS, GO_FLAG_LOOT);
    }

	//所谓的动态即，刷出该对象后，一段时间后会自己消失
    bool IsDynamic()
    {
        return GetBit(GO_FIELD_FLAGS, GO_FLAG_DYNAMIC);
    }
    void SetDynamic()
    {
        GetBit(GO_FIELD_FLAGS, GO_FLAG_DYNAMIC);
    }

	//需要读条
	bool NeedUseMode()
	{
		return GetBit(GO_FIELD_FLAGS, GO_FLAG_USEMODE);
	}

	//使用后是否会消失，如果会的话，几秒钟后会重刷
    bool IsForeverUse()
    {
        return GetBit(GO_FIELD_FLAGS, GO_FLAG_FOREVER);
    }

	// 是否会重生
	bool IsReborn()
	{
		return GetBit(GO_FIELD_FLAGS, GO_FLAG_REBORN);
	}

	//是否机关
	bool IsGEAR()
	{
		return GetBit(GO_FIELD_FLAGS, GO_FLAG_GEAR);
	}

	void UseGear(Player *player); //触碰机关

    void LootSelect(Player *player);//玩家拾取物品//, uint32 entry, uint8 count);

    //是否使用脚本
    bool IsExecScript()
    {
        return GetBit(GO_FIELD_FLAGS, GO_FLAG_SCRIPT);
    }
	//拾起是否需要公告
	bool IsGetItemNotice()
	{
		return GetBit(GO_FIELD_FLAGS, GO_FLAG_GET_ITEM_NOTICE);
	}
	//爆出战利品的怪的ID
	const string &GetLootCreateBy() const
	{
		return GetStr(GO_STRING_FIELD_CREATED_BY);
	}

	void SetLootCreateBy(const string &val)
	{
		SetStr(GO_STRING_FIELD_CREATED_BY,val);
	}

    //////////////////////////////////////////////////////////////////////////
    //for 传送点
    //目标地图ID
    uint32 GetToMapId()
    {
        return GetUInt32(GO_TELETEPORTER_TO_MAPID);
    }
    void SetToMapId(uint32 id)
    {
        SetUInt32(GO_TELETEPORTER_TO_MAPID, id);
    }

    //目标X坐标
    float GetToPositionX()
    {
        return GetFloat(GO_TELETEPORTER_TO_X);
    }
    void SetToPositionX(float x)
    {
        SetFloat(GO_TELETEPORTER_TO_X, x);
    }

    //目标Y坐标
    float GetToPositionY()
    {
        return GetFloat(GO_TELETEPORTER_TO_Y);
    }
    void SetToPositionY(float y)
    {
        SetFloat(GO_TELETEPORTER_TO_Y, y);
    }

    //////////////////////////////////////////////////////////////////////////
    //for 任务
    uint32 GetQuestID_0()
    {
        return GetUInt32(GO_QUEST_ID_0);
    }
    uint32 GetQuestID_1()
    {
        return GetUInt32(GO_QUEST_ID_1);
    }
    uint32 GetQuestID_2()
    {
        return GetUInt32(GO_QUEST_ID_2);
    }
    uint32 GetQuestID_3()
    {
        return GetUInt32(GO_QUEST_ID_3);
    }
	//机关状态
	uint32 GetGearStatus()
	{
		return (GO_GEAR_STATUS_TYPE)GetUInt32(GO_GEAR_STATUS);
	}
	void SetGearStatus(uint32 val)
	{
		SetUInt32(GO_GEAR_STATUS, val);
	}

	//使用之后，广播动作包
	bool BroadcastAction(uint32 player_id);
};


#endif


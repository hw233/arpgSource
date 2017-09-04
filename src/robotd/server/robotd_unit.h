#ifndef _ROBOTD_UNIT_H_
#define _ROBOTD_UNIT_H_
#include <object/SharedDef.h>
#include "robotd_context.h"

class RobotdUnit : public BinLogObject
{
public:
	RobotdUnit(RobotdContext *owner):BinLogObject(core_obj::SYNC_UPDATEDATA)
		,m_main_player(owner)
		,m_positionX(0), m_positionY(0),m_uint_guid(0)
		,m_moving_last_diff(0),m_moving_ms(0),m_orientation(0)
		,m_client_path(false),hasCreated(false)
	{
		m_create_time = time(nullptr);
	}
	virtual ~RobotdUnit(){}
public:
	// x,y...x,y...x,y...
	std::deque<float> m_moving_path;
	bool IsMoving(){return m_moving_ms != 0;}
	bool UpdateLocate(uint32 diff);					//更新位置
	uint32 GetTypeId() const { return GetByte(UNIT_FIELD_BYTE_0,0); }
	uint32 GetInstanceId() const{return GetUInt32(UNIT_FIELD_INSTANCE_ID);}		
	float GetPosX() const{return m_positionX;}
	float GetPosY() const{return m_positionY;}
	void SetPosition(float x, float y, string from);
	void OnAfterUpdate(SyncEventRecorder *data,int flags,UpdateMask& mask,UpdateMask& str_mask);	

	uint32 GetDeathState()
	{
/*
		if(m_cur_hp <= 0.0)
			SetUInt32(UNIT_FIELD_DEATH_STATE,DEATH_STATE_DEAD);*/
		return GetByte(UNIT_FIELD_BYTE_1, 1);
	}
	uint32 GetMapId()const {return GetUInt16(UNIT_FIELD_UINT16_1, 0);}
	
	uint32 GetMoveSpeed() {return GetUInt32(UNIT_FIELD_MOVE_SPEED);}	
	bool IsNpc(){return GetUInt32(UNIT_FIELD_NPC_FLAG) != 0;}
	bool isCreature(){return GetTypeId() == TYPEID_UNIT;}
	bool isNotPlayer(){return GetTypeId() != TYPEID_PLAYER;}
	bool isPlayer(){return GetTypeId() == TYPEID_PLAYER;}
	bool isGameObject(){return GetTypeId() == TYPEID_GAMEOBJECT;}
	uint32 GetUIntGuid()
	{
		if(m_uint_guid == 0)
		{
			m_uint_guid = atol(SUB_GUID_STR_TO_INT(GetGuid()).c_str());
		}
		return m_uint_guid;
	}
	void ReadOffsetPath(ByteArray& bytes);
	//得到朝向角度
	float GetAngle( const RobotdUnit* obj ) const;
	float GetAngle( const float x, const float y ) const;

	//得到距离
	float GetDistance(const float x,const float y) const;
	float GetDistance(const RobotdUnit *obj) const {return GetDistance(obj->GetPosX(), obj->GetPosY());}
	bool IsMainPlayer(){return m_main_player->GetMyUnit() == this;};
	void StopMoving(uint16 x, uint16 y);
	void StartMoving(bool client_path, string from);
protected:
	virtual bool ReadFrom(int flags,ByteArray& bytes);
protected:
	bool hasCreated;
	RobotdContext *m_main_player;
	float m_positionX, m_positionY;
	uint32 m_uint_guid;
	uint32 m_moving_last_diff, m_moving_ms;//m_moving_ms为移动所花的总时间
	float m_orientation;		//朝向
	bool m_client_path;
	time_t m_create_time;
public:
	static int LuaGetPos(lua_State* scriptL);			//获取坐标
	static int LuaSetPos(lua_State* scriptL);			//设置坐标
	static int LuaGetDistance(lua_State* scriptL);		//获取两个unit之间的距离
	static int LuaIsMoving(lua_State* scriptL);			//是否移动状态
	static int LuaStopMoving(lua_State* scriptL);		//停止移动
	static int LuaGetDistanceByPos(lua_State* scriptL);	//获取unit和坐标之间的距离
};

#endif

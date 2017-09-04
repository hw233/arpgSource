#ifndef _BUFF_MANAGER_H_
#define _BUFF_MANAGER_H_

#include <object/std.h>
class Unit;

class BufferManager
{
public:
	BufferManager(Unit* buff_owner);
	~BufferManager();

public:
	static int LuaAddBuff(lua_State* scriptL);
	static int LuaSystemAddBuff(lua_State* scriptL);
	static int LuaRemoveBuff(lua_State* scriptL);
	static int LuaHasBuff(lua_State* scriptL);
	static int LuaGetBuffLevel(lua_State* scriptL);
	static int LuaSetBuffLevel(lua_State* scriptL);
	static int LuaGetReserveVal(lua_State* scriptL);
	static int LuaSetReserveVal(lua_State* scriptL);
	static int LuaGetBuffDuration(lua_State* scriptL);
	static int LuaSetBuffDuration(lua_State* scriptL);
	static int LuaGetBuffGiverUnit(lua_State* scriptL);		//获得buff giver的对象

public:
	void Update(uint32 diff);								//心跳						
	void UpdateBuff();										//BUFF定时器
	inline uint16 GetBuff(uint8 pos);								//获取BUFF

	void AddBuffAttr();										//增加BUFF属性		
	bool RemoveBuff(uint16 buff_id);						//移除BUFF			
	void AddBuff(uint16 buff_id, uint32 buffEffectId=0,int32 bonus_time = 0, const char* name = NULL, const char* guid = NULL, uint32 reserve = 0);							//增加BUFF
	uint8 HasBuff(uint16 buff_id);							//是否有BUFF

	uint16 GetBuffDurationByID(uint16 buff_id);					//获取BUFF持续时间
	void SetBuffDurationByID(uint16 buff_id,uint16 duration);	//设置BUFF持续时间

	uint32 GetReserveVal(uint16 buff_id);					//获取buff预留值
	void SetReserveVal(uint16 buff_id, uint32 reserve);		//设置buff预留值

	void ClearBuff();										//清空BUFF	
	void UpDateHasBuffId();									//更新buffid
	uint32 GetBuffEffectId(uint16 buff_id);						//获取BUFF效果id
	void SetBuffLevelByID(uint16 buff_id, uint32 buffEffectId);		//设置BUFF效果id
	uint32 GetBuffGiverGuidByID(uint16 buff_id);					//根据buffid获得giver guid
private:
	inline bool IsBuffTrigger(uint8 pos);		//BUFF是否触发	
	inline uint32 GetBuffDuration(uint8 pos);						//获取BUFF持续时间 第0~11个
	inline void SetBuffDuration(uint8 pos,uint32 time);				//设置BUFF持续时间
	inline void ReduceBuffTime(uint8 pos);							//减少BUFF持续时间


	inline void SetBuff(uint8 pos,uint16 buff_id,uint32 buffEffectId);		//获取BUFF ID 第0~11个
	inline void SetBuffReserve(uint8 pos,uint32 val);				//设置预留值
	inline uint32 GetBuffEffectId(uint8 pos);							//获取BUFF等级
	inline void SetBuffLevel(uint8 pos,uint32 buffEffectId);		//设置BUFF效果id	
	inline bool IsBuffOver(uint8 pos);								//buff是否结束
	inline void RemoveBuff(uint8 pos);								//移除BUFF
	inline void SetBuffGiverGuid(uint8 pos, uint32 guid);			//设置buff giver的Guid	
	uint32 GetBuffGiverGuid(uint8 pos);							//获得buff giver的Guid
private:
	Unit* m_buffOwner;
	TimeTracker m_buff_timer;	
	uint32 m_last_time;
	uint32 m_update_tick_num;
	uint32 m_ms_last_time;
	std::map<uint16,uint8> m_has_buffid;			//精灵拥有的buffid
	vector<uint8> m_diff_need_del_buff;				//用来存心跳时候需要删除的BUFF位置
};

#endif


#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scened_app.h"
#include <object/SharedDef.h>
#include "Unit.h"
#include <object/PlayerBase.h>

class ScenedContext;
class Map;
class QuestGetter;

typedef std::map<uint16,uint32> ImportanceSpellCDMap;
typedef std::map<uint16,uint8> SpellLevelMap;
typedef std::map<uint16,uint32> SpellCDMap;
typedef std::multimap<uint8, uint16> SlotMultiSpellMap;
typedef std::vector<Creature*> PetVec;
typedef vector<uint32> AttrsVec;
typedef vector<AttrsVec> All_AttrsVec;
typedef std::map<string,uint32> CounterMap;	
typedef std::map<uint32,uint32> SpellStyleMap;

typedef struct buff_info_t
{
	uint32 buff_id;
	uint32 duration;
	uint32 lv;
} BuffInfo;

//系数，用于调整GM玩家在游戏中的一些系数
struct Coefficient 
{
	Coefficient()
	{
		speed_multiples = 100;
		exp_multiples = 100;
		drop_rate_multiples = 100;
		attack_multiples = 100;
		def_multiples = 100;
		hp_multiples = 100;
		mp_multiples = 100;		
		waza_multiples = 100;
		eva_multiples = 100;
		crit_multiples = 100;
	}
	float speed_multiples;			//速度倍率，最大支持5倍
	float exp_multiples;			//经验倍率，最大支持100倍
	float drop_rate_multiples;		//怪物掉率倍数 最大100倍
	float attack_multiples;			//攻击倍率。支持100倍 下同
	float def_multiples;			//防御倍率。
	float hp_multiples;				//HP倍率。
	float mp_multiples;				//MP倍率。
	float waza_multiples;			//命中倍率。
	float eva_multiples;			//闪躲倍率。
	float crit_multiples;			//暴击倍率。
};

#define SKILL_INTERVAL 300

class Player:public Unit
{
	friend class ScenedContext;
public:
	Player();
	virtual ~Player();
public:
	//弄出一个vlaues数据
	void Create (ScenedContext *session);

	void Update(uint32 p_time);					//定时器	
	void UpdateAutoRecovery(uint32 diff);		//自动回复以及战斗状态心跳,	
	void UpdateLiveStatus(uint32 diff);			//更新生存状态
	void UpdateFCMLoginTime(uint32 diff);		//更新累积在线时间	

	void OnKilled(Unit* killer, uint16 id = 0);
	void OnSendKilled(Player* killer, uint16 id);	//处理被玩家杀,发给应用服和写日志
	void OnJoinMap();
	void OnLeaveMap();

	//对象更新以后触发
	void OnAfterPlayerDataUpdate(SyncEventRecorder *,int,UpdateMask&,UpdateMask&);

	//标准时间转服务器运行时间
	uint32 TimestampToServerRuntime(uint32 value);

	//服务器运行时间转标准时间
	uint32 ServerRuntimeToTimestamp(uint32 value);

	//同步
	void SyncUnitToPlayerData();
public:
	ScenedContext *GetSession(){ return m_session;}	
	void SetSession(ScenedContext *_s){m_session = _s;}
	QuestGetter *GetQuestMgr(){return m_questMgr;}

	//获取经验
	void GainExp(double exp,uint8 type = 0,double vip_exp = 0);

	//升级	
	bool Upgrade ();
	//升级后重算
	void Upgrade_Calculate(uint32 prevLevel);
	//获取付费等级
	uint8 GetPayLevel(){return GetByte(UNIT_FIELD_BYTE_2, 1);}

	//战斗力
	double GetForce() {return GetDouble(UNIT_FIELD_FORCE);}
	void SetForce(double f)
	{
		SetDouble(UNIT_FIELD_FORCE,f);
	}

	//获取攻击模式
	uint8 GetAttackMode() const{return GetByte(UNIT_FIELD_BYTE_3,3);}
	//设置攻击模式
	void SetAttackMode(uint8 mode){SetByte(UNIT_FIELD_BYTE_3,3,mode);}
	//PVP的一些相关处理
	void DoPvPEvent(Player* target);
	//PVB的相关处理（玩家打boss）
	void DoPvBEvent(Unit* target);

	//获取红名时间
	uint8 GetRedNameTime()const{return GetByte(UNIT_FIELD_BYTE_2,0);}
	void  SetRedNameTime(uint8 time){SetByte(UNIT_FIELD_BYTE_2,0,time);}

	//挂机状态
	bool IsHungUp();
	void SetHungUp(bool is_hung_up);
	//pvp状态
	void SetPVPstate(bool flag = true);
	//获取pvp状态的bit位
	bool GetPVPstate();
	//是否在安全区
	bool isInSafeArea()const;
	
	//是否在副本地图中
	bool IsInTheInstanceMap()
	{
		return MapTemplate::GetMapTempalte(GetMapId())->IsInstance();
	}
	//获得某标志位
	bool GetPlayerFlags(int offset)
	{
		return GetBit(UNIT_FIELD_FLAGS, offset);
	}

	// 设置精灵的保留下标
	void SetGOData(uint32 data){SetUInt32(GO_FIELD_DATA+3,data);}

public:
	//-----------------------------------------------------------------------------------------
	void Calcul_Attr();					//重新计算计算属性
	Coefficient &GetCoefficient(){return m_coefficent;}
	void SetCoefficient(string str,uint16 multiples);
	void Calcul_Coefficient();			//属性进行系数计算
	bool IsReasonableDistancePos(Unit *obj);			//是否合理距离
public:
	//技能相关	
	virtual void SetSpellCD(uint32 id);	

public:
	bool isFighting(){return !m_combat_state.Passed();}
	void resetCombatStateTime();
	
	void SetPeaceModeCDIfEnabled();

	bool isPVP(){return !m_PVP_state.Passed();}	//是否在PVP战斗中
	void resetPVPStateTime();				//重置PVP定时器

	//判断是否反击
	bool IsCounter(const Unit * unit);
public:
	BinLogObject *GetEquipAttrs(){return m_equip_attrs;}
	//---------------------------------------------------------------------------
	QuestGetter		*m_questMgr;											//任务管理者
	
	SpellCDMap m_spell_cd;						//技能cd集合
	ImportanceSpellCDMap m_importance_spell_cd;	//重要技能cd集合
	SlotMultiSpellMap m_slot_spell;				//技能槽对应的技能
	SpellStyleMap m_spell_style;				//技能招式

	SpellLevelMap m_spell_level;				//在技能槽中的技能和等级

	// 替换技能
	void slotReplaceSpell(uint8 slot, uint16 spell, uint8 level);

	void SetCurSpell(uint32 id);

	bool m_can_calcul_attr;		   //是否允许重算属性

	//设置最后一次接收动作包的时间
	void SetLastActionPktTime()
	{
		m_last_action_pkt_time = ms_time();
	}

	void SetLastSpellTime() {
		m_last_spell_time = ms_time();
		//printf("set last spell time = %u\n", m_last_spell_time);
	}

	/************************************************************************/
	/* 是否可以使用技能(间隔SKILL_INTERVAL毫秒)                                                                     */
	/************************************************************************/
	bool canUseSkill() {
		uint32 now = ms_time();
		//printf("compare now = %u, last = %u, diff = %d", now, m_last_spell_time, now - m_last_spell_time);
		return now >= m_last_spell_time + SKILL_INTERVAL;
	}

	//查看现在是否可以使用GO
	bool CanUseGameObject()
	{
		uint32 now = ms_time();
		uint32 diff =  get_time_diff(m_last_action_pkt_time,now);
		//置一下新的时间点
		//m_last_action_pkt_time = now;
		return diff > g_Config.use_gameobject_diff;
	}
	
	//获取当前战斗状态
	uint8 GetCurFightStatus()
	{
		return m_cur_combet_state;
	}

	void SetPeaceModeCD(uint32 cd);

	uint32 GetPeaceModeCD();

	uint16 GetBattleMode();

	void SetBattleMode(uint16 mode);

	uint32 GetSelfProtectUintGuid() {
		return GetUInt32(UNIT_FIELD_SELF_DEFENSE_GUID);
	}

	void SetSelfProtectUintGuid(uint32 guid) {
		SetUInt32(UNIT_FIELD_SELF_DEFENSE_GUID, guid);
	}

	// 获得挂机自动吃药百分比
	uint16 GetHookHpPercents() {
		return GetUInt16(PLAYER_FIELD_HOOK_SHORT, 0);
	}

	// 获得挂机自动吃药的id
	uint16 GetHookUseItemId() {
		return GetUInt16(PLAYER_FIELD_HOOK_SHORT, 1);
	}

	// 获得挂机状态
	bool isInHook() {
		return GetUInt32(PLAYER_FIELD_HOOK) > 0;
	}

	// 加入战斗
	void fightStatusEnter();

	// 离开战斗
	void ChangeFightStatusToLeave();
	
	void DelPet(Creature *unit);
	void DelPet(uint32 entry);
	void AddPet(Creature *unit);
	void ClearPet(){m_pets.clear();}//只允许在地图释放的时候使用

	//通过模板查找npc， （同一张地图要是出现两个一模一样的npc那就不好办了，所有慎用，咨询lhs）
	Unit* FindNPC(const uint32 npc_entry);

	double GetExp()
	{
		return m_player_exp;
	}

	void lostExp(uint16 rate);

	void DoIfNeedAddProtectBuff();

protected:
	void UnitToPlayerUint32(uint32 index, uint32 player_index);
	void UnitToPlayerDouble(uint32 index, uint32 player_index);
	void UnitToPlayerString(uint32 index, uint32 player_index);
	void UnitByteToPlayerUInt32(uint32 index, uint8 offset, uint32 player_index);
	void UnitUInt16ToPlayerUInt32(uint32 index, uint8 offset, uint32 player_index);
	void UnitUInt32ToPlayerDouble(uint32 index, uint32 player_index);
	void UnitByteToPlayerByte(uint32 index, uint8 offset, uint32 player_index, uint8 player_offset);
	void UnitByteToPlayerDouble(uint32 index,uint8 offset, uint32 player_index);
	void UnitUInt16ToPlayerDouble(uint32 index,uint8 offset, uint32 player_index);

	void SyncCacheData();		//同步缓存数据
private:
	ScenedContext	*m_session;
	BinLogObject *m_equip_attrs;//玩家装备属性，包含各个系统的属性加成
	CounterMap m_counter_map;			//反击列表

	uint8 m_gm_level;
	Coefficient m_coefficent;			//系数
	uint8 m_cur_combet_state;			//当前战斗状态
	uint32 m_exp_bonus;					//套装经验加成
	uint32 m_last_action_pkt_time;		//最后的动作包接收时间
	uint32 m_last_spell_time;			//最后的技能包接收时间
	double daZuoStartTimestamp;			//打坐的开始时间戳
	double m_player_exp;				//玩家经验
	static char m_tmp[128];					//精灵ID转换用的

	TimeTrackerSmall	m_combat_state;			//战斗状态定时器
	TimeTrackerSmall	m_PVP_state;			//是否在PVP中
	TimeTrackerSmall	m_auto_recovery;		//自动用大力丸定时器
	TimeTrackerSmall	m_energy_auto_recovery ;//自动回体定时器
	TimeTrackerSmall    m_endurance_recovery;	//自动回精力定时器

	IntervalTimer		m_fcm_timer;			//防沉迷定时器	
	IntervalTimer		m_red_name_timer;		//红名时间定时器	
	IntervalTimer		m_counter_timer;		//反击时间定时器
	IntervalTimer		m_dazuo_timer;			//打坐回血回蓝定时器
	IntervalTimer		m_hp_timer;				//玩家自身回血定时器

	PetVec				m_pets;
	map<uint32, int32>	m_bingzhong_spells_cd;			//兵种技能CD
	//////////////////////////////////////////////////////////////////////////
	//for lua script
public:

	static int LuaGetPlayerGuid(lua_State* scriptL);
	static int LuaGetSession(lua_State* scriptL);				//获取会话,同时也是真正的玩家数据对象
	//任务接口
	static int LuaGetQuestStatus(lua_State* scriptL);			//获得指定任务的状态
	static int LuaAcceptQuest(lua_State* scriptL);				//玩家接受任务
	static int LuaHasQuest(lua_State* scriptL);					//是否有这个任务
	static int LuaPrintSlotQuest(lua_State* scriptL);			//输出槽里的所有任务id(调试用)
	static int LuaHasQuestType(lua_State* scriptL);				//是否有某任务类型的任务
	static int LuaCompleteCurQuest(lua_State *scriptL);			//完成当前指定类型的任务
	static int LuaCompleteQuestId(lua_State *scriptL);			//完成当前指定的任务
	static int LuaGetQuestCounter(lua_State* scriptL);			//是否有这个任务		
	static int LuaAddQuestItem(lua_State* scriptL);				//获得任务物品
	static int LuaRemoveQuestItem(lua_State* scriptL);			//删除任务物品
	static int LuaClearQuestSlotItem(lua_State* scriptL);		//清空任务下标物品
	static int LuaTalkedToCreature(lua_State* scriptL);			//与生物对话
	static int LuaAreaExploredOrEventHappens(lua_State* scriptL);//事件发生
	static int LuaAddQuestItemBySlot(lua_State* scriptL);		//增加任务下标进度

	static int LuaGetSpellCD(lua_State* scriptL);		//获取技能cd
	static int LuaSetSpellCD(lua_State* scriptL);		//设置技能cd
	static int LuaSetImportanceSpellCD(lua_State* scriptL);		//设置重要技能cd
	static int LuaGetSpellLevel(lua_State* scriptL);	//获得技能等级
	static int LuaSetSpellLevel(lua_State* scriptL);	//设置技能等级

	static int LuaGetDaZuoStartTime(lua_State* scriptL);	//获得打坐初始时间
	static int LuaSetDaZuoStartTime(lua_State* scriptL);	//设置打坐初始时间

	//获得传送id
	static int LuaGetTeleportID(lua_State* scriptL);

	//传送
	static int LuaTeleport(lua_State* scriptL);
	
	//获得物品
	static int LuaAddItem(lua_State* scriptL);

	//升级
	static int LuaUpgrade(lua_State* scriptL);

	//清除玩家小弟
	static int LuaClearPets(lua_State *scriptL);

	static int LuaIsInSaveArea(lua_State* scriptL);			//是否在安全区域

	static int LuaGetCounters(lua_State* scriptL);			//获取反击者
	static int LuaIsCounter(lua_State* scriptL);			//玩家是否反击模式
	
	static int LuaGetPlayeCurFightStatus(lua_State* scriptL);	//获取玩家当前的战斗状态

	static int LuaIsLogined(lua_State* scriptL);	//判断玩家是否已经登录场景服

	static int LuaGetEquipAttrs(lua_State* scriptL);	//获取玩家各系统的属性
	//获得经验
	static int LuaAddExp(lua_State* scriptL);
	//扣经验
	static int LuaSubExp(lua_State* scriptL);
	//设置系数
	static int LuaSetCoefficient(lua_State *scriptL);
	//通知应用服增加money
	static int LuaSendAddMoney(lua_State* scriptL);		
	//通知应用服扣money
	static int LuaSendSubMoney(lua_State* scriptL);
	//通知应用服重算属性
	static int LuaSendRecalculate(lua_State* scriptL);		
	static int LuaGetPetTable(lua_State* scriptL);	//返回玩家的宠物列表
	//设置重算开关
	static int LuaSetCanRecalcuAttr(lua_State* scriptL);
	//添加礼包
	static int LuaAddGiftPacks(lua_State* scriptL);
	//添加观察者
	static int LuaAddWatcher(lua_State* scriptL);
	static int LuaGetSpellStyle(lua_State* scriptL);		//获取最近一次使用的技能族中的技能id
	static int LuaSetSpellStyle(lua_State* scriptL);		//设置最近一次使用的技能族中的技能id

	static int LuaGetSlotSpell(lua_State* scriptL);		//获得某一槽位技能

	static int LuaSendMountJumpDown(lua_State* scriptL);		//坐骑骑乘

	static int LuaLostExpOnDead(lua_State* scriptL);		//死亡掉落经验

	static int LuaSetNeedProtectBuff(lua_State* scriptL);		//设置是否需要死亡保护

	static int LuaSendToAppdDoSomething(lua_State* scriptL);	//发送到场景服进行进入逻辑判断

	static int LuaGetAllPlayerNearBy(lua_State *scriptL);	//获取附近的玩家

	static int LuaSetPlayerEnterOrigin(lua_State *scriptL);	// 设置玩家的进入副本的位置
};

extern uint16 AttrTypePos[MAX_EQUIP_ATTR];

#endif


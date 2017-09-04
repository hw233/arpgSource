#ifndef _PLAYER_BASE_H
#define _PLAYER_BASE_H

#include <svrcore-internal.h>
#include <object/BinLogObject.h>
#include <object/SharedDef.h>
#include <object/BinLogObject.h>

struct lua_State;		//for script

class TEA_SVRCORE_SPEC PlayerBase : public BinLogObject
{	
public:
	PlayerBase(core_obj::SyncMode _m = core_obj::SYNC_SLAVE);
	virtual ~PlayerBase ();		

public:
	uint32 GetScenedFD(){return GetUInt32(PLAYER_FIELD_FD);}
	void SetScenedFD(uint32 fd){SetUInt32(PLAYER_FIELD_FD, fd);}

	void SetAccount(const char *account){SetStr(PLAYER_STRING_FIELD_ACCOUNT, account);}
	virtual string GetAccount(){return GetStr(PLAYER_STRING_FIELD_ACCOUNT);}

	string getLinkName(){
		stringstream ss;
		ss << "[c]" << GetGuid() << "|" << GetName() << "[/c]";
		return ss.str();
	}

	void  SetGender(uint8 gen){SetByte(PLAYER_FIELD_BYTES_0,0,gen);}
	uint8 GetGender(){return GetByte(PLAYER_FIELD_BYTES_0,0);}

	uint32 GetVIP() {

		uint32 vipLevel = GetUInt32(PLAYER_FIELD_VIP_LEVEL);
		uint32 timestamp = (uint32)time(NULL);
		if (GetUInt32(PLAYER_FIELD_VIP_TIME_OUT) < timestamp) {
			vipLevel = 0;
		}

		return vipLevel;
	}

	uint8 GetFaction(){return GetByte(PLAYER_FIELD_BYTES_0,1);}

	void SetRace(uint8 race){SetByte(PLAYER_FIELD_BYTES_0,2,race);}
	uint8 GetRace(){return GetByte(PLAYER_FIELD_BYTES_0,2);}

	uint32 GetLevel(){return GetUInt32(PLAYER_FIELD_LEVEL);}
	void  SetLevel(uint32 level){ SetUInt32(PLAYER_FIELD_LEVEL,level);}

	void SetHead(uint8 h){SetByte(PLAYER_FIELD_BYTES_3,0,h);}
	uint8 GetHead(){return GetByte(PLAYER_FIELD_BYTES_3,0);}

	void SetHair(uint8 h){SetByte(PLAYER_FIELD_BYTES_3,1,h);}
	uint8 GetHair(){return GetByte(PLAYER_FIELD_BYTES_3,1);}
	
	double GetCurExp(){return GetDouble(PLAYER_EXPAND_INT_XP);}
	double GetCurHp(){return GetDouble(PLAYER_FIELD_HEALTH);}
	void SetCurHp(double val)
	{
		if (GetDouble(PLAYER_FIELD_HEALTH) != val)
		{
			SetDouble(PLAYER_FIELD_HEALTH, val);
		}		
	}
	double GetMaxHp(){return GetDouble(PLAYER_FIELD_MAX_HEALTH);}
	/*double GetCurMp(){return GetDouble(PLAYER_FIELD_POWERS + POWER_MANA*2);}
	void SetCurMp(double val)
	{
	if (GetDouble(PLAYER_FIELD_POWERS + POWER_MANA*2) != val)
	SetDouble(PLAYER_FIELD_POWERS + POWER_MANA*2, val);
	}
	double GetMaxMp(){return GetDouble(PLAYER_FIELD_MAXPOWERS + POWER_MANA*2);}
	double GetCurSp(){return GetDouble(PLAYER_FIELD_POWERS + POWER_ENERGY*2);}
	void SetCurSp(double val)
	{
	if (GetDouble(PLAYER_FIELD_POWERS + POWER_ENERGY*2) != val)
	SetDouble(PLAYER_FIELD_POWERS + POWER_ENERGY*2, val);
	}
	double GetMaxSp(){return GetDouble(PLAYER_FIELD_MAXPOWERS + POWER_ENERGY*2);}
	double GetCrit(){return GetDouble(PLAYER_FIELD_COMBAT_RATE + COMBAT_RATE_CRIT*2);}*/
	double GetForce(){return GetDouble(PLAYER_FIELD_FORCE);}//战斗力	
	//double GetBear(){return GetDouble(PLAYER_FIELD_BEAR);}	//负重

	//大红
	double GetAutoHp(){return GetDouble(PLAYER_FIELD_AUTO_HP);}
	void   SetAutoHp(double v){SetDouble(PLAYER_FIELD_AUTO_HP,v);}
	//大蓝
	double GetAutoMp(){return GetDouble(PLAYER_FIELD_AUTO_MP);}
	void   SetAutoMp(double v){SetDouble(PLAYER_FIELD_AUTO_MP,v);}
	//GM标识串
	void SetGmNum(uint8 lv){return SetByte(PLAYER_FIELD_BYTES_5, 0, lv);}
	//GM等级
	uint8 GetGmNum(){return GetByte(PLAYER_FIELD_BYTES_5, 0);}
	//有GM命令权限
	uint8 GetGMLevel(){return GetByte(PLAYER_FIELD_BYTES_5, 0) % 10;}
	//客服GM权限 
	uint8 GetFalseGM(){return GetByte(PLAYER_FIELD_BYTES_5, 0) % 100 / 10;}
	//美女主播权限 2表示不如排行榜
	uint8 GetGirlGM(){return GetByte(PLAYER_FIELD_BYTES_5, 0) / 100;}

	//设置禁言时间
	void SetGagEndTime(uint32 v){SetUInt32(PLAYER_EXPAND_INT_GAG_END_TIME, v);}
	//获取禁言时间
	uint32 GetGagEndTime(){return GetUInt32(PLAYER_EXPAND_INT_GAG_END_TIME);}
	//是否禁言中
	bool IsGag(){return GetUInt32(PLAYER_EXPAND_INT_GAG_END_TIME) > time(NULL);}

	//防沉迷BEGIN
	//设置累计在线时间
	void SetFCMLoginTime(uint32 val){SetUInt32(PLAYER_EXPAND_INT_FCM_LOGIN_TIME, val);}
	uint32 GetFCMLoginTime(){return GetUInt32(PLAYER_EXPAND_INT_FCM_LOGIN_TIME);}
	void AddFCMLoginTime(uint32 val){AddUInt32(PLAYER_EXPAND_INT_FCM_LOGIN_TIME,val);}
	//设置累积离线时间
	void SetFCMLogoutTime(uint32 val){SetUInt32(PLAYER_EXPAND_INT_FCM_LOGOUT_TIME, val);}
	uint32 GetFCMLogoutTime(){return GetUInt32(PLAYER_EXPAND_INT_FCM_LOGOUT_TIME);}
	//防沉迷END

	//最后登录的IP
	void SetLastLoginIp(string ip){SetStr(PLAYER_STRING_FIELD_LAST_LOGIN_IP, ip);}
	const string &GetLastLoginIp() const{return GetStr(PLAYER_STRING_FIELD_LAST_LOGIN_IP);}
	//创建时的IP
	void SetCreateLoginIp(string ip){SetStr(PLAYER_STRING_FIELD_CREATE_LOGIN_IP, ip);}
	const string &GetCreateLoginIp() const{return GetStr(PLAYER_STRING_FIELD_CREATE_LOGIN_IP);}

	//平台信息
	void SetPlatData(string data){SetStr(PLAYER_APPD_STRING_FIELD_PINGTAI_INFO, data);}
	const string &GetPlatData() const{return GetStr(PLAYER_APPD_STRING_FIELD_PINGTAI_INFO);}

	//是否显示时装
	void SetUseFashion(uint16 pos);

	uint32 GetTrialLayers() {return GetUInt32(PLAYER_FIELD_TRIAL_LAYERS);}

	//获取角色创建时间
	uint32 GetCreateTime(){return GetUInt32(PLAYER_EXPAND_INT_CREATE_TIME);}
	void SetCreateTime(uint32 t){SetUInt32(PLAYER_EXPAND_INT_CREATE_TIME, t);}
	//最后登录时间
	uint32 GetLastLoginTime(){return GetUInt32(PLAYER_EXPAND_INT_LAST_LOGIN_TIME);}
	void SetLastLoginTime(uint32 t){SetUInt32(PLAYER_EXPAND_INT_LAST_LOGIN_TIME, t);}
	//最后登出时间
	uint32 GetLastLogoutTime(){return GetUInt32(PLAYER_EXPAND_INT_LAST_LOGOUT_TIME);}
	void SetLastLogoutTime(uint32 t){SetUInt32(PLAYER_EXPAND_INT_LAST_LOGOUT_TIME, t);}
	//离开冒险世界的时间戳
	uint32 GetLastLeaveRiskTime(){return GetUInt32(PLAYER_INT_FILED_LEAVE_RISK_TIME);}
	void SetLastLeaveRiskTime(uint32 t){SetUInt32(PLAYER_INT_FILED_LEAVE_RISK_TIME, t);}
	////是否已经领取不在冒险世界的挂机奖励
	//uint32 GetPickRiskRewardFlag(){return GetUInt32(PLAYER_INT_FILED_LEAVE_RISK_PICKED);}
	//void SetPickRiskRewardFlag(uint32 t){SetUInt32(PLAYER_INT_FILED_LEAVE_RISK_PICKED, t);}

	//累积登录次数
	void AddNumberOfLogins(){AddUInt32(PLAYER_EXPAND_INT_NUMBER_OF_LOGINS, 1);}
	//获取登录次数
	uint32 GetNumberOfLogins(){return GetUInt32(PLAYER_EXPAND_INT_NUMBER_OF_LOGINS);}
	//经验
	double GetExp(){return GetDouble(PLAYER_EXPAND_INT_XP);}

	//获取货币
	virtual double GetMoney(uint16 type){ return GetDouble(PLAYER_EXPAND_INT_MONEY + type * 2);}
	//杀怪数量
	uint32 GetKillMonster(){return GetUInt32(PLAYER_EXPAND_KILL_MONSTER);}
	void AddKillMonster(){AddUInt32(PLAYER_EXPAND_KILL_MONSTER,1);}
	//在线时长
	void AddOnlineTime(uint32 val){AddUInt32(PLAYER_FIELD_ONLINE_TIME, val);}
	uint32 GetOnlineTime(){return GetUInt32(PLAYER_FIELD_ONLINE_TIME);}
	//设置每天在线时长(min)
	void SetDailyOnlineTime(uint32 val){SetUInt32(PLAYER_APPD_INT_FIELD_DAILY_TIME, val);}
	//增加每天在线时长
	void AddDailyOnlineTime(uint32 val){AddUInt32(PLAYER_APPD_INT_FIELD_DAILY_TIME, val);}
	//返回每天在线时长
	uint32 GetDailyOnlineTime(void){return GetUInt32(PLAYER_APPD_INT_FIELD_DAILY_TIME);}

	void SetMapState(uint32 state){SetUInt32(PLAYER_EXPAND_INT_MAP_STATUS, state);}//设置地图进度

	//性别
	uint8 GetGender()const{return GetByte(PLAYER_FIELD_BYTES_0,0);}
	//阵营
	uint8 GetFaction()const{return GetByte(PLAYER_FIELD_BYTES_0,1);}
	// 获得当前复活玩家是否有死亡保护
	bool isNeedProtectedBuff() const{return GetByte(UNIT_FIELD_BYTE_3, 3) > 0;}
	// 设置死亡保护buff标志
	void SetNeedProtectedBuff(uint8 value) {SetByte(UNIT_FIELD_BYTE_3, 3, value);}

	//设置阵营
	void SetFaction(uint8 fa){SetByte(PLAYER_FIELD_BYTES_0,1,fa);}

	//获得应用服flag
	bool GetFlags(uint32 off) {return GetBit(PLAYER_APPD_INT_FIELD_FLAG, off);}
	//设置应用服flag
	void SetFlags(uint32 off) {SetBit(PLAYER_APPD_INT_FIELD_FLAG, off);}
	void UnSetFlags(uint32 off) {UnSetBit(PLAYER_APPD_INT_FIELD_FLAG, off);}

	//角色是否已删除
	bool IsDelete(){return GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_ZIBAO);}
	void SetDelete(){return SetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_ZIBAO);}	

	//获得某个任务是否已经完成,完成为1
	bool IsQuestIDComplete(uint32 quest_id) const
	{
		return false;
	}

	//设置显示用装备的模板下标
	void SetEquipment(uint16 pos,uint32 entry)
	{
		SetUInt32(PLAYER_FIELD_EQUIPMENT + pos, entry);
	}

	uint32 GetCurMountTemp()
	{
		return GetUInt32(PLAYER_FIELD_CUR_MOUNT);
	}
	void SetCurMountTemp(uint32 temp)//设置当前坐骑模板
	{
		SetUInt32(PLAYER_FIELD_CUR_MOUNT,temp);
	}
	//获取付费等级
	uint8 GetPayLevel(){return GetByte(PLAYER_FIELD_BYTES_2, 1);}
	//称号
	void SetTitle(uint8 value){SetByte(PLAYER_FIELD_BYTES_2, 3, value);}
	uint8 GetTitle() {return GetByte(PLAYER_FIELD_BYTES_2, 3);}

	//double GetEva(){return GetDouble(PLAYER_FIELD_COMBAT_RATE + COMBAT_RATE_EVA*2);}
	//增加免费复活次数
	void AddVipReliveCount(uint8 count){AddByte(PLAYER_FIELD_BYTES_2, 2, count);}
	//是否vip
	bool isVip(uint8 level){return GetUInt32(PLAYER_FIELD_VIP_LEVEL) >= level && GetUInt32(PLAYER_FIELD_VIP_TIME_OUT) >= time(NULL);}

	//变换对象位置和朝向
	void Relocate(float x, float y, float orientation)
	{
		SetPosition(x,y);
		SetOrientation(orientation);
	}
	//获取x坐标
	virtual float GetPositionX(){return GetFloat(PLAYER_FIELD_POS_X);}
	//获取y坐标
	virtual float GetPositionY(){return GetFloat(PLAYER_FIELD_POS_Y);}
	//获取坐标
	void GetPosition(float &x, float &y)
	{
		x = GetPositionX();
		y = GetPositionY();
	}

	void SetPosition( float x, float y) 
	{ 
		SetFloat(PLAYER_FIELD_POS_X, x);
		SetFloat(PLAYER_FIELD_POS_Y, y);
	}

	//变换对象朝向
	void SetOrientation(float orientation) { SetFloat(PLAYER_FIELD_ORIENTATION, orientation); }
	float GetOrientation( ) const { return GetFloat(PLAYER_FIELD_ORIENTATION); }		//获取朝向

	int GetMapId()const {return GetInt32(PLAYER_FIELD_MAP_ID);}
	void SetMapId(int id)
	{
		if (GetMapId() != id)
			SetInt32(PLAYER_FIELD_MAP_ID, id);		
	}

	int GetMapLineNo()const {return GetInt32(PLAYER_FIELD_LINE_NO);}
	void SetMapLineNo(int lineNo)
	{
		if (GetMapLineNo() != lineNo)
			SetInt32(PLAYER_FIELD_LINE_NO, lineNo);		
	}

	


	//玩家回档时的自增索引下标
	void AddHuiDangIndex(){AddByte(PLAYER_FIELD_MERGE_DATA_INDEX, 1, 1);}

	//传送进行数据完整性验证
	uint8 GetTeleportSign(){return GetByte(PLAYER_FIELD_MERGE_DATA_INDEX, 2);}
	void SetTeleportSign(uint8 val){SetByte(PLAYER_FIELD_MERGE_DATA_INDEX,2,val);}
	//所在地图实例的id，若id为0，则是传送中
	uint32 GetInstanceId() const{return GetUInt32(PLAYER_FIELD_INSTANCE_ID);}
	void SetInstanceId(uint32 id){SetUInt32(PLAYER_FIELD_INSTANCE_ID, id);}
	
	//传送相关
	int32 GetTeleportMapID() const{return GetInt32(PLAYER_EXPAND_INT_TO_MAP);}
	float GetTeleportPosX() const{return GetFloat(PLAYER_EXPAND_INT_TO_POS_X);}
	float GetTeleportPosY() const{return GetFloat(PLAYER_EXPAND_INT_TO_POS_Y);}
	uint32 GetTeleportLineNo() const{return GetUInt32(PLAYER_EXPAND_INT_TO_LINE_NO);}
	uint32 GetTeleportInstanceId() const{return GetUInt32(PLAYER_EXPAND_INT_TO_INSTANCE_ID);}	
	void SetTeleportInstanceId(uint32 id){SetUInt32(PLAYER_EXPAND_INT_TO_INSTANCE_ID, id);}
	const string &GetTeleportGuid() const {return GetStr(PLAYER_STRING_FIELD_TELEPORT_GUID);}		//传送用的字符串下标
	const void SetTeleportGuid(const string& teleportGuid) {this->SetStr(PLAYER_STRING_FIELD_TELEPORT_GUID, teleportGuid);}
	void SetTeleportInfo(int map_id, float x, float y, uint32 line_no, const string &teleport_id)
	{
		SetInt32(PLAYER_EXPAND_INT_TO_MAP, map_id);
		SetFloat(PLAYER_EXPAND_INT_TO_POS_X, x);
		SetFloat(PLAYER_EXPAND_INT_TO_POS_Y, y);
		SetUInt32(PLAYER_EXPAND_INT_TO_LINE_NO, line_no);
		if(GetTeleportGuid() != teleport_id)
			SetStr(PLAYER_STRING_FIELD_TELEPORT_GUID, teleport_id);
		SetInstanceId(0);//传送中
	}

	int GetDBMapID(){return GetInt32(PLAYER_EXPAND_INT_DB_MAP);}
	float GetDBPosX(){return GetFloat(PLAYER_EXPAND_INT_DB_POS_X);}
	float GetDBPosY(){return GetFloat(PLAYER_EXPAND_INT_DB_POS_Y);}

	void GetToDBPosition(int& mapid,float& x,float& y)
	{
		mapid = GetDBMapID();
		x = GetDBPosX();
		y = GetDBPosY();		
	}
	void RelocateDBPosition()
	{
		//ASSERT(m_toDB_map);
		if (!GetDBMapID())
		{
			return;
		}

		SetMapId(GetDBMapID());
		SetPosition(GetDBPosX(),GetDBPosY());
	}
	void SetToDBPositon(int mapid,float x,float y)
	{
		SetInt32(PLAYER_EXPAND_INT_DB_MAP, mapid);
		SetFloat(PLAYER_EXPAND_INT_DB_POS_X, x);
		SetFloat(PLAYER_EXPAND_INT_DB_POS_Y, y);
	}
		
	uint32 GetKuafuWarId(){return GetUInt32(PLAYER_APPD_INT_FIELD_KUAFU_WARID);}				//获得跨服id		
	void SetKuafuWarId(uint32 val){SetUInt32(PLAYER_APPD_INT_FIELD_KUAFU_WARID, val);}			//设置跨服id
	uint32 GetKuafuNumber(){return GetUInt16(PLAYER_INT_FIELD_KUAFU_NUMBER,0);}					//获得跨服编号
	void SetKuafuNumber(uint32 number){SetUInt16(PLAYER_INT_FIELD_KUAFU_NUMBER, 0, number);}	//设置跨服编号
	uint32 GetKuafuType(){return GetUInt16(PLAYER_INT_FIELD_KUAFU_NUMBER,1);}					//获得跨服类型
	void SetKuafuType(uint32 type){SetUInt16(PLAYER_INT_FIELD_KUAFU_NUMBER, 1, type);}			//设置跨服类型
	//保存跨服信息
	void SaveDBKuafuInfo(uint32 warid, uint16 kuafu_type, uint16 kuafu_number, const string &bksvr)
	{
		SetKuafuWarId(warid);
		SetKuafuType(kuafu_type);
		SetKuafuNumber(kuafu_number);
		SetStr(PLAYER_STRING_FIELD_DB_KUAFU_INFO, bksvr);
	}
	void ClearDBKuafuInfo()
	{
		if (GetKuafuWarId())
			SetKuafuWarId(0);
		if (GetKuafuType())
			SetKuafuType(0);
		if (GetKuafuNumber())
			SetKuafuNumber(0);
		if (!GetStr(PLAYER_STRING_FIELD_DB_KUAFU_INFO).empty())
			SetStr(PLAYER_STRING_FIELD_DB_KUAFU_INFO,"");
	}
	const string &GetDBKuafuInfo(){return GetStr(PLAYER_STRING_FIELD_DB_KUAFU_INFO);}
	bool IsKuafuPlayer()
	{//是否跨服玩家
		if (GetStr(PLAYER_STRING_FIELD_DB_KUAFU_INFO).empty())
			return false;
		return true;
	}
	//设置玩家主线任务ID
	void SetMainQuestID(uint32 v){}
	//获取玩家主线任务ID
	uint32 GetMainQuestID()const{return 0;}

	//获取玩家充值数
	uint32 GetRechargeSum()const{return GetUInt32(PLAYER_APPD_INT_FIELD_RECHARGE_SUM);}
	//获取玩家最后充值时间
	uint32 GetRechargeLastTime()const{return GetUInt32(PLAYER_APPD_INT_FIELD_LAST_RECHARGE_TIME);}
	string GetRechargeLastID()const{return GetStr(PLAYER_STRING_FIELD_RECHARGE_ID);}
	//获取玩家场景状态标识
	bool GetPlayerScenedFlags(uint32 index){return GetBit(PLAYER_SCENED_INT_FLAGS, index);}
	//设置玩家场景状态标识
	void SetPlayerScenedFlags(uint32 index){SetBit(PLAYER_SCENED_INT_FLAGS, index);}
	//un设置玩家场景状态标识
	void UnSetPlayerScenedFlags(uint32 index){UnSetBit(PLAYER_SCENED_INT_FLAGS, index);}
	//玩家是否死亡
	bool IsAlive(){return GetUInt32(PLAYER_FIELD_DEATH_STATE) == DEATH_STATE_ALIVE;}

	//设置玩家主下标flags标识
	bool GetPlayerFlags(uint8 index){return GetBit(PLAYER_FIELD_FLAGS, index);}
	void SetPlayerFlags(uint8 index){SetBit(PLAYER_FIELD_FLAGS, index);}
	void UnSetPlayerFlags(uint8 index){UnSetBit(PLAYER_FIELD_FLAGS, index);}


	void SetFactionId(string factionId) {
		SetStr(PLAYER_STRING_FIELD_FACTION_GUID, factionId);
	}

	string GetFactionId() {
		return GetStr(PLAYER_STRING_FIELD_FACTION_GUID);
	}
	//帮派名字
	string GetFactionName(){
		return GetStr(PLAYER_STRING_FIELD_FACTION_NAME);
	}
};

#endif


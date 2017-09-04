#ifndef _RANK_LIST_MANAGER_H_
#define _RANK_LIST_MANAGER_H_
#include "appd_app.h"

class RankListManager;
class PlayerBase;
namespace core_obj{
class SyncEventRecorder;
}

//任务池中最大的任务个数
#define MAX_RANK_LIST_POOL_COUNT 5000
//每个任务最大的标志位长度 8 * 32
#define MAX_RAMK_LIST_POOL_MASK_LEN 8
//任务池任务数量下标
#define RANK_LIST_POLL_INT_FIELD_COUNT 0
//任务池标志位开始下标
#define RANK_LIST_POLL_INT_FIELD_MASK_START 1

//世界变量排行榜下标
enum ERankInfoFields
{
	//暂时没整形下标

	//排行榜子binlog开始下标，每个名次的binlog占两位（int_str, str_str）。
	RANK_INFO_STRING_FIELD_START = BINLOG_STRING_FIELD_OWNER + 1,	
};

//排行榜任务池
//不使用继承，用引用
class RankListTaskPool
{
public:
	RankListTaskPool()
	{
		//把任务池binlog赋值给本类
		ASSERT(AppdApp::g_app->m_rank_info_rask_pool);
		m_obj = AppdApp::g_app->m_rank_info_rask_pool;
	}
	~RankListTaskPool(){m_obj = NULL;}
public:
	//目前有多少任务
	int GetCount(){return m_obj->GetInt32(RANK_LIST_POLL_INT_FIELD_COUNT);}
	//某位置上的任务的工作guid
	const string &GetTaskGuid(int index){return m_obj->GetStr(BINLOG_STRING_FIELD_OWNER + index + 1);}
	//增加一条任务
	void InsertTask(const string &guid, ERankTypes type);
	//清空任务池
	void Clear();
	//某位置上是否有某任务
	bool HasTask(int index, ERankTypes type)
	{
		return m_obj->GetBit(RANK_LIST_POLL_INT_FIELD_MASK_START + index * MAX_RAMK_LIST_POOL_MASK_LEN, type);
	}
private:
	BinLogObject *m_obj;			//任务池binlog
	map<string, int> m_guid_index;	//任务池索引
};

//排行榜子对象id，一位就好了，其实没用，节约流量
#define RANK_LIST_OBJECT_CURRENT_GUID "r"
//排行榜子binlog，每个代表排行榜中某一个排行位的详细信息
class RankListCurrentObj : public BinLogObject
{
public:
	RankListCurrentObj():BinLogObject(core_obj::SYNC_NONE){}
	virtual ~RankListCurrentObj(){}
protected:
	//对比两个数字
	template <class T>
	int CompareNumber(T p1, T p2)
	{
		if(p1 > p2)
			return 1;
		if(p1 < p2)
			return -1;
		return 0;
	}

public:
	//工厂方法，所有的子类都从这里new出来
	static RankListCurrentObj *Factoy(ERankTypes type);
	//与另一个类比较，必须同种类型
	virtual int Compare(RankListCurrentObj *) = 0;
	//详细信息赋值
	virtual bool SetData(BinLogObject *binlog) = 0;
	//本类所属的排行类型
	virtual ERankTypes GetType() = 0;
	//是否可以从排行榜中清除了(名字为空，默认不进榜)
	virtual bool LeaveRankList(){return GetCurrentPlayerName().empty();}
	// 加密
	void encode(ByteArray& bytes) {
		// 先写int
		bytes.writeShort(this->length_uint32());
		for (uint32 value : this->uint32_values_) {
			bytes.writeUnsignedInt(value);
		}
		// 在写string
		bytes.writeShort(this->length_str());
		for (string str : this->str_values_) {
			bytes.writeUTF(str);
		}
	}
	//初始化
	void Init(const string &guid = "", BinLogObject *binlog = nullptr);
	//排名
	int GetRanking(){return GetInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_RANKING) - 1;}
	void SetRanking(int v){SetInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_RANKING, v + 1);}

	//本排行信息的关键guid
	const string &GetCurrentGuid(){return GetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_GUID);}
	void SetCurrentGuid(const string & str){SetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_GUID, str);}

	//本排行信息中玩家的guid
	const string &GetCurrentPlayerGuid(){return GetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_GUID);}
	void SetCurrentPlayerGuid(const string & str){SetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_GUID, str);}

	//本排行信息中的玩家名
	const string &GetCurrentPlayerName(){return GetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_NAME);}
	void SetCurrentPlayerName(const string & str){SetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_NAME, str);}

	//本排行信息中的帮派名
	const string &GetCurrentPlayerFaction(){return GetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_FACTION_NAME);}
	void SetCurrentPlayerFaction(const string & str){SetStr(RANK_LIST_CURRENT_OBJ_STR_FIELD_FACTION_NAME, str);}

	//本排行榜信息中玩家要硬性踢除
	bool IsDelete(){return m_delete;}

	//本排行信息中玩家的性别
	uint8 GetCurrentPlayerGender(){return GetByte(RANK_LIST_CURRENT_OBJ_INT_FIELD_BYTE, 0);}
	void SetCurrentPlayerGender(uint8 v){SetByte(RANK_LIST_CURRENT_OBJ_INT_FIELD_BYTE, 0, v);}
	
	//本排行信息中玩家的称号
	uint32 GetCurrentPlayerTitle(){return GetUInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_TITLE);}
	void SetCurrentPlayerTitle(uint32 v){SetUInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_TITLE, v);}

	//排行榜中的外观
	uint32 GetCurrentPlayerShow(const int &id){return GetUInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_SHOW+id);}
	void SetCurrentPlayerShow(const int &id,const uint32 &v){SetUInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_SHOW+id,v);}

	//vip
	uint8 GetCurrentPlayerVip(){return GetByte(RANK_LIST_CURRENT_OBJ_INT_FIELD_BYTE, 2);}
	void SetCurrentPlayerVip(uint8 v){SetByte(RANK_LIST_CURRENT_OBJ_INT_FIELD_BYTE, 2, v);}
	//设置点赞数
	void SetCurrentPlayerLike(uint32 v){SetUInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_LIKE, v);}

	//翅膀id
	uint32 GetCurrentPlayerWing(){return GetUInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_WING);}
	void SetCurrentPlayerWing(uint32 v){SetUInt32(RANK_LIST_CURRENT_OBJ_INT_FIELD_WING, v);}

private:
	//是否可以从排行榜清除了，这个比较特殊，比如自爆这种硬性条件
	bool m_delete;
};

//很多问题
//如果只排内存里的，确实有点坑爹
//但是如果需要读数据库，写起来更郁闷
//外围需要重排的时候，可通过运维工具生成数据？？
//合服以后，登录服会发老服的排行榜过来。
class RankListManager
{
public:
	RankListManager();
	~RankListManager();

	//排行榜数据，多出一个给排行功能使用
	RankListCurrentObj *m_data[MAX_RANK_TYPE][RANK_LIST_MAX_VALUES + 1];
	UpdateMask m_mask;
private:
	//对调排行榜信息
	void SwitchRankInfo(ERankTypes type, int index, RankListCurrentObj *obj);
private:
	//获取某排行榜某个名次的主guid
	const string &GetRankGuid(ERankTypes type, int rank)
	{
		return m_data[type][rank]->GetCurrentGuid();
	}
	//获取类型排名所对应的世界变量排行榜整数下标
	inline int GetRankListInfoIntIndex(int type, int rank)
	{
		return RANK_INFO_STRING_FIELD_START + type * RANK_LIST_MAX_VALUES * 2 + rank * 2;
	}
	//获取类型排名所对应的世界变量排行榜字符串下标
	inline int GetRankListInfoStrIndex(int type, int rank)
	{
		return RANK_INFO_STRING_FIELD_START + type * RANK_LIST_MAX_VALUES * 2 + rank * 2 + 1;
	}
	//把当前数据同步给世界变量排行榜
	void SyncData();
	//开始针对性的进行排序
	void Sort(ERankTypes type);
	//生成排行榜数据
	bool MakeBinlogData(const string &guid, ERankTypes type);
	//获取排行榜某个位置的binlog
	inline RankListCurrentObj *GetRankListData(int type, uint32 rank)
	{
		ASSERT(type < MAX_RANK_TYPE);
		RankListCurrentObj *obj = m_data[type][rank];
		ASSERT(obj->GetType() == type);
		ASSERT(obj->GetRanking() == rank);
		return obj;
	}
	//设置排行榜某个位置的binlog
	inline void SetRankListData(int type, uint32 rank, RankListCurrentObj *obj)
	{
		ASSERT(type < MAX_RANK_TYPE);
		ASSERT(obj->GetType() == type);
		ASSERT(obj->GetRanking() == rank);
		m_data[type][rank] = obj;
	}
	//查找某玩家在某排行榜类型中所在的排名
	int FindPlayerRanking(int type, const string &guid);

	uint32 GetRankingCount(int type);
public:
	//初始化排行榜信息
	void InitRankInfo();
	//初始化排行榜任务池
	void InitTaskPool();
	// 更新排行榜信息
	void UpdateRankList();

	vector<string> GetRankGuidList(ERankTypes type);
	bool HasRankGuid(ERankTypes type,string &guid,int &rank);
	void UpdateRankLike(int type,const string &guid,uint32 num);
	//心跳。。
	void Update(uint32 diff);
	//增加一条任务
	void InsertTask(const string &guid, ERankTypes type);
	//清空某排行榜
	void ClearRankList(int type);
	//写查询包
	int WriteQueryByteArray(core_obj::ByteArray &bytes, const string &guid, uint32 call_back_id, uint8 rank_list_type, uint16 start_index, uint16 end_index);
	//合服以后，要与本服合并的排行榜
	void MergeResort(const string &data, const string &str_data);
	//查询某个排行榜某个位置的主GUID
	RankListCurrentObj *FindRankDataByType(int type, uint32 rank);
	//获取战力排行榜玩家guid
	string GetForceRankPlayerGuid(uint32 rank, uint32 force = 0);
	//获取等级排行榜玩家guid
	string GetLevelRankPlayerGuid(uint32 rank, uint32 level = 0);

	

public:
	//玩家下标发生变化
	void OnAfterUpdate(core_obj::SyncEventRecorder *data,int flags,UpdateMask& mask,UpdateMask& str_mask);
	//清空排行榜
	void DoQaClearRankInfo();

private:
	RankListTaskPool *m_task_pool;			//任务池
	TimeTrackerSmall m_timer;				//定时器
	bool m_init_ok;							//初始化完成

};

#endif

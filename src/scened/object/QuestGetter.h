#ifndef _QUEST_MANAGER_H_
#define _QUEST_MANAGER_H_

#include <comm/common_stl.h>
#include <fastdelegate/FastDelegate.h>
#include "scened_app.h"
#include "object/QuestDef.h"
#include "Player.h"
#include "scened_context.h"

class Creature;

//任务承接者
class QuestGetter
{
public:	
	QuestGetter(Player& player):m_obj(player)
	{	
	}
	~QuestGetter(){
		m_canSeeQuests.clear();
	};

	void InitQuest();		
	void Update(uint32 diff);

	//-----------------------------------------验证可接任务要求----------------------------------------------------------------
    bool SatisfyQuestLog(bool msg=false) const;										// 是否有任务空位
    bool SatisfyQuestLevel(quest_template const *qInfo, bool msg=false) const;		// 是否符合任务等级
    bool SatisfyQuestReputation(quest_template const *qInfo, bool msg=false) const;	// 是否符合阵营声望值
    bool SatisfyQuestStatus(quest_template const *qInfo, bool msg=false) const;		// 是否已接任务
    bool SatisfyQuestNextChain(quest_template const *qInfo, bool msg=false) const;	// 是否已经完成后续任务
    bool SatisfyQuestPrevChain(quest_template const *qInfo, bool msg=false) const;	// 是否已经完成前置任务
	bool SatisfyQuestFaction(quest_template const *qInfo,bool msg=false) const;		// 是否符合阵营

	bool CanSeeStartQuest( quest_template const *pQuest ) const;
	bool CanTakeQuest( quest_template const *pQuest, bool msg =false) const;
	bool CanAddQuest( quest_template const *pQuest, bool msg =false) const;	
	void RefreshCanSeeQuests();														// 可接任务列表
	vector<uint32>& GetCanSeeQuests(){return m_canSeeQuests;}
	void Upgrade();
	void UpForce();

	//日常先不处理
	bool SatisfyQuestDay(quest_template const* , bool ) const {return true;};
	bool SatisfyQuestWeek(quest_template const* , bool ) const {return true;};

	void AddQuest(quest_template const *pQuest);
	void RewardQuest( quest_template const *pQuest, uint32 reward);
	void FailQuest( uint32 quest_id );

	//-------------------------------------------任务完成阶段----------------------------------------------
	bool CanCompleteQuest( uint32 quest_id ) const;
	bool CanCompleteRepeatableQuest(quest_template const *pQuest) const;
	bool CanRewardQuest( quest_template const *pQuest, bool msg =false) const;
//	bool CanRewardQuest( quest_template const *pQuest, uint32 reward, bool msg =false) const;
	void CompleteQuest(uint32 quest_id);

	/*-----------------------------------------任务属性操作---------------------------------------------*/
    //打印任务槽所有任务
	void PintSlotQuests();
	
	//根据任务ID取插槽位置
    uint16 FindQuestSlot(uint32 quest_id) const;
	
	uint32 GetLevel() const
	{
		return m_obj.GetLevel();
	}

	uint32 GetQuestLevel(quest_template const *pQuest) const
	{
		return pQuest && pQuest->quest_level ?  pQuest->quest_level : GetLevel();
	}

    //根据插槽位置取任务ID
    uint32 GetQuestSlotQuestId(uint16 slot) const
    {
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		return 0;
    }

    //根据插槽位置取任务状态
    uint32 GetQuestSlotState(uint16 slot)   const
    {
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		return 0;
    }
	void SetQuestSlotState(uint16 slot, uint32 state)
	{
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		//m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, state);
	}
	//根据插槽位置设置任务flag
	void SetQuestSlotFlag(uint16 slot, uint8 flag)  
	{
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		//m_obj.GetSession()->SetBit(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_FLAG_OFFSET, flag);
	}
	//根据插槽位置获取任务flag
	bool GetQuestSlotFlag(uint16 slot, uint8 flag) const
	{
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		//return m_obj.GetSession()->GetBit(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_FLAG_OFFSET, flag);
		return false;
	}
	
	//设置插槽
    void SetQuestSlot(uint16 slot, uint32 quest_id, uint32 timer = 0)
    {
		/*ASSERT(slot < MAX_QUEST_LOG_SIZE);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_ID_OFFSET, quest_id);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, QUEST_STATUS_INCOMPLETE);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET, 0);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_1, 0);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_2, 0);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_3, 0);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_TIME_OFFSET, timer);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_FLAG_OFFSET, 0);*/
    }
	//根据插槽位置取任务要求完成数量
	uint16 GetQuestSlotCounter(uint16 slot, uint8 counter) const
	{
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		/*if(counter == 0 || counter == 1)
		return m_obj.GetSession()->GetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET, counter);
		else if(counter == 2 || counter == 3)
		return m_obj.GetSession()->GetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_1, counter-2);
		else if(counter == 4 || counter == 5)
		return m_obj.GetSession()->GetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_2, counter-4);
		else if (counter == 6 || counter == 7)
		return m_obj.GetSession()->GetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_3, counter-6);*/
		return 0;
	}
	//根据插槽位置设置任务要求完成数量
    void SetQuestSlotCounter(uint16 slot, uint8 counter, uint16 count)
    {
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		/*if(counter == 0 || counter == 1)
		m_obj.GetSession()->SetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET, counter, count);
		else if(counter == 2 || counter == 3)
		m_obj.GetSession()->SetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_1, counter-2, count);
		else if(counter == 4 || counter == 5)
		m_obj.GetSession()->SetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_2, counter-4, count);
		else if (counter == 6 || counter == 7)
		m_obj.GetSession()->SetUInt16(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET_3, counter-6, count);*/
    }
    
    /*void RemoveQuestSlotState(uint16 slot, uint32 state)
    {
       // m_obj.RemoveFlag(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, state);
		m_obj.GetPlayerExpand()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, state);
    }*/
	//根据插槽位置取任务完成时间限制
	uint32 GetQuestSlotTime(uint16 slot)    const
	{
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
		return 0;
	}
    void SetQuestSlotTimer(uint16 slot, uint32 timer)
    {
		ASSERT(slot < MAX_QUEST_LOG_SIZE);
        //m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + slot * MAX_QUEST_OFFSET + QUEST_TIME_OFFSET, timer);
    }
    void SwapQuestSlot(uint16 slot1, uint16 slot2)
    {
		/*for (int i = 0; i < MAX_QUEST_OFFSET; ++i)
		{
		uint32 temp1 = m_obj.GetSession()->GetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + MAX_QUEST_OFFSET * slot1 + i);
		uint32 temp2 = m_obj.GetSession()->GetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + MAX_QUEST_OFFSET * slot2 + i);

		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + MAX_QUEST_OFFSET * slot1 + i, temp2);
		m_obj.GetSession()->SetUInt32(PLAYER_SCENED_INT_FIELD_QUEST_LOG_0 + MAX_QUEST_OFFSET * slot2 + i, temp1);
		}*/
    }
	void ClearQuestSlot()
	{
		for (int i = 0; i < MAX_QUEST_LOG_SIZE; i++)
		{
			if(GetQuestSlotQuestId(i) != 0)
			{
				SetQuestSlot(i,0);				
			}
		}
		tea_pdebug("清理任务SLOT END");
	}

	uint32 GetQuestItemSlot(uint16 slot) const
	{
		ASSERT(slot < QUEST_ITEMS_COUNTS);
		return 0;
	}

	void SetQuestItemSlot(uint16 slot,uint32 itemid)
	{
		ASSERT(slot < QUEST_ITEMS_COUNTS);
	}

	uint32 FindQuestItemSlot(uint32 itemid) const
	{
		for (int i = 0; i < QUEST_ITEMS_COUNTS; i++)
		{
			if(GetQuestItemSlot(i) == itemid )
				return i;
		}	
		return QUEST_ITEMS_COUNTS;
	}

	//增加物品到任务包裹
	bool PushQuestItemSlot(uint32 itemid)
	{
		ASSERT(itemid);
		for(int i = 0; i < QUEST_ITEMS_COUNTS; i++)
		{
			if (GetQuestItemSlot(i) == 0)
			{
				SetQuestItemSlot(i,itemid);
				return true;
			}
		}
		tea_perror("guid[%s]任务包裹已满",m_obj.GetGuid().c_str());
		//ASSERT(false);//直接一点
		return false;
	}

	//从任务包裹中去掉1个该物品,删掉后把后面的往前移
	void PopQuestItemSlot(uint32 itemid)
	{		
		for (uint32 i = FindQuestItemSlot(itemid);i < QUEST_ITEMS_COUNTS;i++)
		{
			if(i+1 == QUEST_ITEMS_COUNTS)	//最后一条记录
				SetQuestItemSlot(i,0);
			else
				SetQuestItemSlot(i,GetQuestItemSlot(i+1));
		}
	}

	//清空任务下标进度
	bool ClearQuestSlotItem(uint32 quest_id)
	{
		if ( quest_id == 0 )
			return false;

		uint16 slot = FindQuestSlot(quest_id);
		if (slot < MAX_QUEST_LOG_SIZE && GetQuestSlotState(slot) == QUEST_STATUS_INCOMPLETE)
		{//槽里有，且未完成
			SetQuestSlotFlag(slot, QUEST_SLOT_FLAGS_REWARD);		//清空领取过任务奖励flag
			SetQuestSlotFlag(slot, QUEST_SLOT_FLAGS_EXPLORE);		//清空已探索flag
			for (int j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
			{
				SetQuestSlotCounter(slot,j,0);				
			}
		}
	
		return true;
	}

	void ClearQuestItemSlot()
	{
		for(int i = 0; i < QUEST_ITEMS_COUNTS; i++)
		{
			if (GetQuestItemSlot(i) != 0)
				SetQuestItemSlot(i,0);			
		}
	}

	QuestStatus GetQuestStatus( uint32 quest_id ) const;
	void SetQuestStatus( uint32 quest_id, QuestStatus status );
	bool GetQuestRewardStatus( uint32 quest_id ) const;
	//const QuestStatusMap& GetStatusMap() const{return m_questStatus;}

	//从生物身上获得可接任务列表
	bool GetQuestList(Creature *creature,vector<quest_option>& ) const;
	QuestGiverStatus GetGiverStatus(quest_wo_relation* relation) const;	
//	QuestGiverStatus GetGiverStatus( uint32 quest_id ) const;	
	bool GetQuestList_CanAccept(vector<quest_canaccept_info_t>& ) const;
	void SendNextChainQuest(Creature *creature ,uint32);										//发送任务链的下条可接任务
	
	//---------------------------------------通信-----------------------------------------------
	//发送任务已完成
	void SendQuestComplete(uint32 quest_id);
	void SendQuestAccept(uint32 quest_id); //发送任务已接受
    void SendQuestReward(quest_template const *pQuest, uint32 XP, Unit *questGiver);	//发送任务奖励
    void SendQuestFailed(uint32 quest_id);
	void SendQuestLogFull() const;
	void SendCanTakeQuestResponse(uint32 msg) const;
    void SendQuestTimerFailed(uint32 quest_id);    
    //void SendQuestConfirmAccept(quest_template const *pQuest, Player *pReceiver);
    //void SendPushToPartyResponse(Player *pPlayer, uint32 msg);
    //void SendQuestUpdateAddItem(quest_template const *pQuest, uint32 item_idx, uint32 count);
    void SendQuestUpdateAddCreatureOrGo(quest_template const *pQuest, uint64 guid, uint32 creatureOrGO_idx, uint32 old_count, uint32 add_count);	
	void SendBookQuest(uint32 dynasty_id);

	//-------------------------------------更新任务进度-----------------------------------------	
	void KilledMonster(creature_template const *creature);	//杀死生物
	void KilledLevelMonster(uint32 create_level);	//杀等级生物
	void KilledLevelPlayer(uint32 bekiller_level);	//杀等级玩家	
	void TalkedToCreature( uint32 entry, const string &guid );					//与NPC对话
	bool AddQuestItem(uint32 entry,uint32 count);						//获得任务物品
	bool AddQuestItemBySlot(uint32 quest_id,uint32 slot_id,uint32 count);						//增加任务下标进度
	void AreaExploredOrEventHappens( uint32 questId );					//事件发生
	void CastedCreatureOrGO(Unit*,uint32 item_id);	//使用物品弄死生物或者收集物品
	void AutoFailOfflineOrDie();										//自动失败任务
	void OnJoinMap();
	void OnLeaveMap();

	void AddTimedQuest( uint32 quest_id ) { m_timedquests.insert(quest_id); }
	
	//根据任务id找到提交任务的npc模板
	uint32 FindCommitNpcEntry(uint32 questid);
	

	//-------------------------------------内部调用接口-----------------------------------------
//	int rpc_centd_quest_load();	
//	static int rpc_centd_quest_delete_item(uint32 guid,int quest_id);
//	static int rpc_centd_quest_add_req_item(uint32 guid,int item_id,int item_count);
	static int quest_add_rew_item(const string &lowguid,uint32 item_id,uint32 item_count,uint8 bind_type = ITEM_BIND_GET, uint32 streng = 0,uint32 fail_time = 0, uint8 op_type = 4);

	//设置某个任务已经完成
	void SetCompleteQuestID(uint32 quest_id, bool flag = true)
	{
		/*if (flag)
		{
		m_obj.GetSession()->SetBit(PLAYER_SCENED_INT_FIELD_QUEST_COMPLETE_LIST, quest_id);
		}
		else
		{
		m_obj.GetSession()->UnSetBit(PLAYER_SCENED_INT_FIELD_QUEST_COMPLETE_LIST, quest_id);
		}		*/
	}
	//获得某个任务是否已经完成
	bool IsQuestIDComplete(uint32 quest_id) const
	{
		return false;
	}	
	
	//将该任务的前置全部完成
	void SetQuestIDPrevComplete(uint32 quest_id);
private:
    Player &m_obj;
	std::set<uint32> m_timedquests;
	vector<uint32> m_canSeeQuests;			//可接任务列表	
};

//////////////////////////////////////////////////////////////////////////

#endif


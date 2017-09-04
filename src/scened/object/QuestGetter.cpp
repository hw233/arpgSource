
#include "scened_context.h"
#include "QuestGetter.h"
#include "Creature.h"
#include "ScenedObjectManager.h"
#include <object/SharedDef.h>
#include "scened_scripts.h"
#include <shared/log_handler.h>
#include <object/OprateResult.h>
//////////////////////////////////////////////////////////////////////////
//for QuestGetter

void QuestGetter::InitQuest()
{
	Upgrade();		//检测一下任务的等级
	//UpForce();		//检测一下战斗力条件	
	//给客户端发可接任务列表
	RefreshCanSeeQuests();
}

void QuestGetter::Update(uint32 diff)
{
	if (!m_timedquests.empty())
	{
		std::set<uint32>::iterator iter = m_timedquests.begin();
		while (iter != m_timedquests.end())
		{
			uint16 slot = FindQuestSlot(*iter);
			if (slot == MAX_QUEST_LOG_SIZE)
				continue;

			uint32 cur_time = (uint32)time(NULL);
			uint32 limit_time = GetQuestSlotTime(slot);

			if(cur_time >= limit_time)
			{
				uint32 quest_id  = *iter;
				FailQuest(quest_id);	         // current iter will be removed in FailQuest
			}
			
			++iter;			
		}
	}
}

uint16 QuestGetter::FindQuestSlot(uint32 quest_id) const
{
	for ( uint16 i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
		if ( GetQuestSlotQuestId(i) == quest_id )
			return i;

	return MAX_QUEST_LOG_SIZE;
}

//打印任务槽所有任务
void QuestGetter::PintSlotQuests()
{
	for ( uint16 i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		if ( GetQuestSlotQuestId(i) != 0 )
		{
			tea_pdebug("PintSlotQuests  slot_off = %u	quest_id = %u", i, GetQuestSlotQuestId(i));
		}
	}
}

bool QuestGetter::CanSeeStartQuest( quest_template const *pQuest ) const
{
	if ( /*&&SatisfyQuestNextChain(pQuest, false) && */  SatisfyQuestPrevChain(pQuest, false) && SatisfyQuestFaction(pQuest,false)
		&& SatisfyQuestDay(pQuest, false) && SatisfyQuestWeek(pQuest, false) && SatisfyQuestReputation(pQuest, false))
	{
		return true/*GetLevel() + QUEST_LOW_LEVEL_HIDE_DIFF >= pQuest->min_level*/;		//这个判断对12j的任务没用
	}

	return false;
}

bool __not_has_giver_npc(uint32 questid)
{
	MemDB_Result<quest_wo_relation> result = quest_wo_relation_db.select(MEMDB_EQ(quest_wo_relation,quest_id,questid));
	
	if (result.size() == 0)
		return false;
	
	for (uint32 j = 0; j < result.size(); j++)
	{
		if(result[j]->relatetion_type == 0 && result[j]->wo_entry == 0)
			return false;
	}
	
	return true;
}

void QuestGetter::RefreshCanSeeQuests()
{	
	m_canSeeQuests.clear();

// 	uint32 maxlevel = GetLevel()+QUEST_LOW_LEVEL_HIDE_DIFF;
// 	quest_template_db.add_select_condition(MEMDB_LE(quest_template,quest_level,maxlevel));
	quest_template_db.add_select_condition(MEMDB_EQ(quest_template,enable,1));
	MemDB_Result<quest_template> result = quest_template_db.select();

	//大于最低等级的,并且任务等级不高于自身级别5级，符合阵营条件	
	for(uint32 i = 0; i < result.size() ; i++ )
	{
		if(result[i] == NULL)
			continue;

		//待接npc如果为空,则认为不出现在可接列表
		if(!__not_has_giver_npc(result[i]->id))
			continue;

		//如果不可以看到该任务
		if(CanSeeStartQuest(result[i]) && SatisfyQuestStatus(result[i]))
			m_canSeeQuests.push_back(result[i]->id);		
	}	

	//有刷新肯定要发送的啦
	if (m_obj.GetSession())
	{
		Call_questhelp_canaccept_list(m_obj.GetSession()->m_delegate_sendpkt,m_canSeeQuests);
	}	
}

void QuestGetter::Upgrade()
{
	//12j主要是在接受任务的时候卡等级,这边处理的是卡完成
	for( int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if ( questid == 0 )
			continue;		

		if ( GetQuestStatus(questid) != QUEST_STATUS_INCOMPLETE )
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if(!qInfo)
			continue;

		//等级验证
// 		if(qInfo->req_level == 0 )
// 			continue;
		
		if ( CanCompleteQuest( questid ) && !IsManualComplete(qInfo))
			CompleteQuest( questid );
	}
}

void QuestGetter::UpForce()
{
	for( int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if ( questid == 0 )
			continue;		

		if ( GetQuestStatus(questid) != QUEST_STATUS_INCOMPLETE )
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if(!qInfo)
			continue;

		//战斗力验证
		if(qInfo->suggested_players == 0 )
			continue;

		if ( CanCompleteQuest( questid ) )
			CompleteQuest( questid );
	}
}

bool QuestGetter::CanTakeQuest(quest_template const *pQuest, bool msg /* =false */) const
{
	return SatisfyQuestStatus(pQuest, msg) && SatisfyQuestLevel(pQuest, msg)
		 /*&& SatisfyQuestNextChain(pQuest, msg) */
		&& SatisfyQuestPrevChain(pQuest, msg) 
		&& SatisfyQuestFaction(pQuest,msg)
		&& SatisfyQuestReputation(pQuest, msg);
}

bool QuestGetter::CanAddQuest(quest_template const *pQuest, bool msg /* =false */) const
{
	if( !SatisfyQuestLog( msg ) )
		return false;

	return true;
}

void QuestGetter::AddQuest(quest_template const *pQuest)
{
	
}

//可否领取任务奖励
bool QuestGetter::CanRewardQuest(quest_template const *pQuest, bool msg /*=false*/) const
{
	if(!IsAutoComplete(pQuest) && GetQuestStatus(pQuest->id) != QUEST_STATUS_COMPLETE)
		return false;
	
	//不可重复领取奖励
	if(GetQuestRewardStatus(pQuest->id))
		return false;
	
	return true;
}

//任务奖励
void QuestGetter::RewardQuest( quest_template const *pQuest, uint32 reward )
{
	//去掉该任务
	uint16 slot = FindQuestSlot(pQuest->id);
	if(slot < MAX_QUEST_LOG_SIZE)
	{
		//清空任务槽
		SetQuestSlot(slot, 0);
		//从任务状态列表中删掉
		SetCompleteQuestID(pQuest->id);
	}		

	//奖励物品
	
	//固定奖励
	uint32 itemid;
	uint32 item_count;
	for(int i = 0; i < QUEST_REWARDS_COUNT; i++)
	{
		itemid = pQuest->rew_item_id[i];
		item_count = pQuest->rew_item_count[i];
		if(!itemid || !item_count)
			break;
		//如果是任务物品直接进任务包裹即可
		ASSERT(item_template_db[itemid]);
		if (item_template_db[itemid]->quest != 0)		
			PushQuestItemSlot(itemid);
		else
		{
			item_template const* item = item_template_db[itemid];
			if(item->sex != CHAR_GENDER_NONE && item->sex != m_obj.GetGender())
				continue;
			if(m_obj.GetSession())
				quest_add_rew_item(m_obj.GetSession()->GetGuid(),itemid,item_count,ITEM_BIND_GET,0,0,LOG_ITEM_OPER_TYPE_QUEST);
		}
	}
	
	//选择奖励
	itemid = pQuest->rew_choice_item_id[reward];
	item_count = pQuest->rew_choice_item_count[reward];
	if(itemid != 0 && item_count != 0)
	{
		ASSERT(item_template_db[itemid]);
		if (item_template_db[itemid]->quest != 0)		
			 PushQuestItemSlot(itemid);
		else if(m_obj.GetSession())
			quest_add_rew_item(m_obj.GetSession()->GetGuid(),itemid,item_count,ITEM_BIND_GET,0,0,LOG_ITEM_OPER_TYPE_QUEST);
	}
	
	//处理任务奖励，经验，银子，绑定元宝
	DoQuestRewardScript(&m_obj, pQuest->id, pQuest->rew_xp, pQuest->rew_silver, pQuest->rew_gift, pQuest->rew_gold, pQuest->quest_flags, pQuest->special_flags); 
		

#if 0
	if(!HasFlag(pQuest,_QUEST_FLAGS_FLAGS_REPEATABLE))
		SetQuestStatus(pQuest->id,QUEST_STATUS_COMPLETE);
	else
		SetQuestStatus(pQuest->id,QUEST_STATUS_NONE);			//如果是循环任务,完成后状态还是None，所以可以重复接
#else
//	SetQuestStatus(pQuest->id,QUEST_STATUS_COMPLETE);			//任务都清掉了为什么还要设置状态
#endif	

}


void QuestGetter::FailQuest(uint32 questId)
{
	if (quest_template const* pQuest = quest_template_db[questId])
	{		
		SetQuestStatus(questId, QUEST_STATUS_FAILED);

		uint16 log_slot = FindQuestSlot(questId);

		if (log_slot < MAX_QUEST_LOG_SIZE)
		{
			SetQuestSlotTimer(log_slot, 0);
			SetQuestSlotState(log_slot, QUEST_STATUS_FAILED);
		}

		if (HasFlag(pQuest,_QUEST_FLAGS_FLAGS_TIMED))
		{
			m_timedquests.erase(questId);
			if(m_obj.GetSession())
				Call_questupdate_faild(m_obj.GetSession()->m_delegate_sendpkt,1);
		}
		else if(m_obj.GetSession())
			Call_questupdate_faild(m_obj.GetSession()->m_delegate_sendpkt,0);			
	}
}


//能否完成任务
bool QuestGetter::CanCompleteQuest( uint32 quest_id ) const
{
	if(!quest_id)
		return false;

	uint16 slot = FindQuestSlot(quest_id);
	if(slot >= MAX_QUEST_LOG_SIZE)
		return false;
	
	QuestStatus const quest_state = (QuestStatus)GetQuestSlotState(slot);
	if(quest_state == QUEST_STATUS_COMPLETE)
		return false;	//不可重复完成
	quest_template const* qInfo = quest_template_db[quest_id];
	if(!qInfo)
		return false;
	//等级验证
	if(qInfo->req_level > 0 && m_obj.GetLevel() < (uint32)qInfo->req_level)
		return false;
	
	////战斗力验证
	//if (qInfo->suggested_players > 0 && m_obj.GetForce()<qInfo->suggested_players)
	//	return false;	

	// auto complete quest
	/*if (qInfo->IsAutoComplete() && CanTakeQuest(qInfo, false))
		return true;*/

	if(quest_state == QUEST_STATUS_INCOMPLETE)
	{
		int counter = 0;
		//需要收集物品
		if(HasFlag(qInfo,_QUEST_FLAGS_FLAGS_DELIVER))
		{
			for(int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i, ++counter)
			{
				if(qInfo->req_item_count[i]!=0 && GetQuestSlotCounter(slot, counter) < qInfo->req_item_count[i])
				{	
					return false;
				}				
			}
		}

		//需要杀死怪物或者与NPC对话的
		if(HasFlag(qInfo,_QUEST_FLAGS_FLAGS_KILL_OR_CAST | _QUEST_FLAGS_FLAGS_SPEAKTO))
		{
			for(int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i, ++counter)
			{
				if(qInfo->req_world_object_id[i] == 0)
					continue;

				if(qInfo->req_world_object_count[i] != 0 && GetQuestSlotCounter(slot, counter) < qInfo->req_world_object_count[i])
				{
					return false;
				}
			}
		}

		if(HasFlag(qInfo,_QUEST_FLAGS_FLAGS_EXPLORATION_OR_EVENT) && !GetQuestSlotFlag(slot, QUEST_SLOT_FLAGS_EXPLORE))
		{
			return false;
		}
		if(HasFlag(qInfo,_QUEST_FLAGS_FLAGS_TIMED) && GetQuestSlotTime(slot) == 0)
		{
			return false;
		}
		uint32 num = sizeof(qInfo->req_kill_level_info) / sizeof(float);
		if (num > 0 && (HasFlag(qInfo,QUEST_FLAGS_FLAGS_LEVEL_PLAYER) || HasFlag(qInfo,QUEST_FLAGS_FLAGS_LEVEL_CREATURE)))
		{
			for (uint32 t = 0; t < num; t+=2)
			{
				uint32 need_level = (uint32)qInfo->req_kill_level_info[t];
				uint32 need_count = (uint32)qInfo->req_kill_level_info[t+1];
				if (need_level == 0 || need_count == 0)
					break;			

				for(int j=0;j<QUEST_OBJECTIVES_COUNT;j++)
				{				
					uint32 curkillcount = GetQuestSlotCounter(slot, j);
					if(curkillcount < need_count)
						return false;
				}
			}
		}
		
	}	
	
	return true;
}

bool QuestGetter::CanCompleteRepeatableQuest(const quest_template* pQuest) const
{
	uint16 slot = FindQuestSlot(pQuest->id);
	if (slot == MAX_QUEST_LOG_SIZE)
		return false;

	if(!CanTakeQuest(pQuest,false))
		return false;

	//需要收集物品
	if(HasFlag(pQuest,_QUEST_FLAGS_FLAGS_DELIVER))
	{
		for(int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
		{
			if(pQuest->req_item_count[i]!=0 && GetQuestSlotCounter(slot, i) < pQuest->req_item_count[i])
			{				
				return false;
			}
		}
	}
	
	if(!CanRewardQuest(pQuest,false))
		return false;
	
	return true;
}

void QuestGetter::CompleteQuest(uint32 quest_id)
{
	if(!quest_id)
		return;
	
	SetQuestStatus(quest_id, QUEST_STATUS_COMPLETE);
	uint16 log_slot = FindQuestSlot(quest_id);
	if(log_slot < MAX_QUEST_LOG_SIZE)
		SetQuestSlotState(log_slot,QUEST_STATUS_COMPLETE);

	if(quest_template const* qInfo = quest_template_db[quest_id])
	{
		if(IsAutoReward(qInfo))
		{
			RewardQuest(qInfo,0); 		
			//执行完成脚本
			DoCompleteQuest(m_obj.GetMap(), &m_obj, quest_id);		
			RefreshCanSeeQuests();		//是自动完成任务的话，服务端主动下发下可见列表
		}
	
		SendQuestComplete(quest_id);
		if(IsAutoReward(qInfo) && m_obj.GetSession())
		{
			//通知客户端播放任务完成效果
			Call_questgetter_complete(m_obj.GetSession()->m_delegate_sendpkt, quest_id);
		}
		//记录完成任务日志
		if(m_obj.GetSession())
		{
			WriteTask(m_obj.GetSession()->GetAccount(),m_obj.GetSession()->GetGuid(), quest_id, m_obj.GetMapId(),0);
			//腾讯日志
			if(ScenedApp::g_app->GetPlatformID() == PLATFORM_QQ)
				WriteTXNewTaskLog(m_obj.GetSession()->GetAccount(),m_obj.GetSession()->GetGuid(),m_obj.GetSession()->GetName(),quest_id,1,0,(uint32)time(NULL));
		}
	}
}

void QuestGetter::SendQuestComplete(uint32 quest_id)
{
	if (m_obj.GetSession())
		Call_questupdate_complete(m_obj.GetSession()->m_delegate_sendpkt,quest_id);

}

void QuestGetter::SendQuestAccept(uint32 quest_id)
{	
	if (m_obj.GetSession())
		Call_questupdate_accept(m_obj.GetSession()->m_delegate_sendpkt,quest_id);
}

void QuestGetter::SendBookQuest(uint32 dynasty_id)
{
	quest_template_db.add_select_condition(MEMDB_EQ(quest_template,dynasty,dynasty_id));
	MemDB_Result<quest_template> result = quest_template_db.select();
	
	if (!result.size())
	{
		return;
	}

	//从结果集中剔除已经完成的
	vector<int> book_quests;
	for (uint32 i = 0; i < result.size(); i++)
	{
		if(GetQuestStatus(result[i]->id) == QUEST_STATUS_NONE)
			book_quests.push_back(result[i]->id);
	}	

	int questid = 0;
	if(!book_quests.empty())
		questid = book_quests[urand(0,book_quests.size()-1)];
	//从结果中查询未做的任务，如果全做过，则从全部中随机
	else
		questid = result[urand(0,result.size()-1)]->id;
	if(questid)
		Call_questhelp_book_quest(m_obj.GetSession()->m_delegate_sendpkt,questid);
}

bool QuestGetter::SatisfyQuestLog(bool msg/* =false */) const
{
	// exist free slot
	if( FindQuestSlot(0) < MAX_QUEST_LOG_SIZE )
		return true;

	if( msg )
		SendQuestLogFull();

	return false;
}

bool QuestGetter::SatisfyQuestStatus(quest_template const *qInfo, bool msg/* =false */) const
{
	QuestStatus status = GetQuestStatus(qInfo->id);
	if  (status != QUEST_STATUS_NONE)
	{
		//可重复接的如果已经完成并领取奖励了也可以接
		if(HasFlag(qInfo,_QUEST_FLAGS_FLAGS_REPEATABLE) && status == QUEST_STATUS_COMPLETE && GetQuestRewardStatus(qInfo->id))
			return true;

		if (msg)
			SendCanTakeQuestResponse(INVALIDREASON_QUEST_ALREADY_ON);
		return false;
	}
	return true;
}

bool QuestGetter::SatisfyQuestLevel(quest_template const *qInfo, bool msg/* =false */) const
{
	if (GetLevel() < qInfo->min_level)
	{
		if (msg)
			SendCanTakeQuestResponse(INVALIDREASON_QUEST_FAILED_LOW_LEVEL);
		return false;
	}
	//else if (qInfo->max() > 0 && GetLevel() > qInfo->GetMaxLevel())
	//{
	//	if (msg)
	//		SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ); // There doesn't seem to be a specific response for too high player level
	//	return false;
	//}
	return true;
}

bool QuestGetter::SatisfyQuestReputation(quest_template const *qInfo, bool msg/* =false */) const
{
	//TODO:声望先不处理
	return true;
}

bool QuestGetter::SatisfyQuestFaction(quest_template const *qInfo,bool msg/* =false */) const
{
	if (qInfo->faction == 0)
		return true;
	return qInfo->faction == m_obj.GetFaction();
}

bool QuestGetter::SatisfyQuestNextChain(quest_template const *qInfo, bool msg/* =false */) const
{
	quest_template_db.add_select_condition(MEMDB_EQ(quest_template,prev_quest,qInfo->id));
	MemDB_Result<quest_template> result = quest_template_db.select();
	if(!result.size())
		return true;
	
	//所有的以此任务为前置的都应该不能完成或者是未完成状态
	for (uint32 i=0;i<result.size();i++)
	{
		QuestStatus status = GetQuestStatus(result[i]->id);
		if(status == QUEST_STATUS_COMPLETE || status == QUEST_STATUS_INCOMPLETE)
		{

			//if(msg)
			//	SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
			return false;
		}
	}
	
	return true;
}

bool QuestGetter::SatisfyQuestPrevChain(quest_template const *qInfo, bool msg/* =false */) const
{
	/*quest_template_db.add_select_condition(MEMDB_EQ(quest_template,))*/
	//无前置任务
	if (qInfo->prev_quest == 0)
	{
		return true;
	}
	QuestStatus status = GetQuestStatus(qInfo->prev_quest);
	//只考虑前置任务，不考虑前置任务的前置任务...
	if(status == QUEST_STATUS_NONE )
		return false;
	if(status == QUEST_STATUS_INCOMPLETE )
		return false;
	if (status == QUEST_STATUS_COMPLETE && !GetQuestRewardStatus(qInfo->prev_quest))
		return false;
	return true;
}

QuestStatus QuestGetter::GetQuestStatus( uint32 quest_id ) const
 {
	if (quest_id)
	{
		//检查一下是否在已完成列表
		if(IsQuestIDComplete(quest_id))
			return QUEST_STATUS_COMPLETE;

		uint16 slot = FindQuestSlot(quest_id);
		if (slot < MAX_QUEST_LOG_SIZE)
		{
			return (QuestStatus)GetQuestSlotState(slot);
		}	
	}
	return QUEST_STATUS_NONE;
}

void QuestGetter::SetQuestStatus( uint32 quest_id, QuestStatus status )
{
	quest_template const* qInfo = quest_template_db[quest_id];
	if(qInfo)
	{
		uint16 slot = FindQuestSlot(quest_id);
		if (slot < MAX_QUEST_LOG_SIZE)
		{
			SetQuestSlotState(slot, status);
			if (status == QUEST_STATUS_NONE || status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE || status == QUEST_STATUS_FAILED)
			{
				if (HasFlag(qInfo, _QUEST_FLAGS_FLAGS_TIMED))
					m_timedquests.erase(qInfo->id);

				//如果已经在已完成列表的需要置掉
				if(IsQuestIDComplete(quest_id) && HasFlag(qInfo,_QUEST_FLAGS_FLAGS_REPEATABLE))
					SetCompleteQuestID(quest_id, false);
			}
		}
	}
}

bool QuestGetter::GetQuestRewardStatus( uint32 quest_id ) const
{
	quest_template *quest = quest_template_db[quest_id];
	if(quest)
	{
		//如果已经在已完成任务表中则认为已经完成
		if(IsQuestIDComplete(quest_id))
			return true;
	
		uint16 slot = FindQuestSlot(quest_id);
		if (slot < MAX_QUEST_LOG_SIZE)
		{
			return GetQuestSlotFlag(slot, QUEST_SLOT_FLAGS_REWARD);
		}		
	}
	return false;
}

void QuestGetter::SendQuestLogFull() const
{

}

void QuestGetter::SendCanTakeQuestResponse(uint32 msg) const
{
	if (msg == INVALIDREASON_QUEST_FAILED_LOW_LEVEL)
	{//等级不够
		m_obj.GetSession()->Call_Operation_Failed(OPERTE_TYPE_QUEST, INVALIDREASON_QUEST_FAILED_LOW_LEVEL);
	}

}

QuestGiverStatus QuestGetter::GetGiverStatus(quest_wo_relation* relation) const
{
	ASSERT(relation);
	
	QuestStatus status = GetQuestStatus(relation->quest_id);			//任务状态
	quest_template *quest = quest_template_db[relation->quest_id];	//任务模板

	if(relation->relatetion_type == QUEST_RELATION_TYPE_END)
	{
		if(IsAutoComplete(quest) && CanTakeQuest(quest) && HasFlag(quest,_QUEST_FLAGS_FLAGS_REPEATABLE))
			return DIALOG_STATUS_REWARD_REP;

		if(status == QUEST_STATUS_COMPLETE && !GetQuestRewardStatus(relation->quest_id))		
			return DIALOG_STATUS_REWARD;		//可领取奖励		

		if(status == QUEST_STATUS_INCOMPLETE)		
			return DIALOG_STATUS_INCOMPLETE;	//未完成		
	}
	else if (relation->relatetion_type == QUEST_RELATION_TYPE_START)
	{
		if(CanSeeStartQuest(quest) && status == QUEST_STATUS_NONE)
		{
			if(SatisfyQuestLevel(quest))
			{				
				if(IsAutoComplete(quest) || (HasFlag(quest,_QUEST_FLAGS_FLAGS_REPEATABLE) && GetQuestRewardStatus(quest->id)))
					return DIALOG_STATUS_REWARD_REP;			
				else if(m_obj.GetLevel() <= (GetQuestLevel(quest)+QUEST_LOW_LEVEL_HIDE_DIFF))
				{
					if(IsDaily(quest))
						return DIALOG_STATUS_AVAILABLE_REP;
					else
						return DIALOG_STATUS_AVAILABLE;
				}
				else
					return DIALOG_STATUS_AVAILABLE;	//可接任务		
			}					
			else 
				return DIALOG_STATUS_UNAVAILABLE;
		}		
	}
	else ASSERT(false);
	return DIALOG_STATUS_NONE;
}

bool QuestGetter::GetQuestList(Creature *questGiver,vector<quest_option>& quests) const
{
	ASSERT(questGiver);//TODO:距离校验？
	quests.clear();

	quest_wo_relation_db.add_select_condition(MEMDB_EQ(quest_wo_relation,wo_entry, questGiver->GetEntry())); 
	MemDB_Result<quest_wo_relation> result = quest_wo_relation_db.select();

	quest_option item;
	for (uint32 i=0; i < result.size(); i++)
	{
		quest_template *quest = quest_template_db[result[i]->quest_id];
		memset(&item,0,sizeof(item));
		item.quest_icon = GetGiverStatus(result[i]);
		if(item.quest_icon != DIALOG_STATUS_NONE)
		{
			item.quest_id = quest->id;
			strcpy(item.quest_title , quest->title);
			item.quest_level = GetQuestLevel(quest);
			quests.push_back(item);
		}		
	}

	return true;
}

bool  QuestGetter::GetQuestList_CanAccept(vector<quest_canaccept_info_t>& quests) const
{
	quests.clear();

	//for (int i=0; i < quest_wo_relation_db.size(); i++)
	//{
	//	quest_wo_relation *relation = quest_wo_relation_db[i];
	//	quest_template *quest = quest_template_db[relation->quest_id];
	//	if(GetGiverStatus(relation->quest_id) != DIALOG_STATUS_NONE)
	//	{
	//		quest_canaccept_info_t info;
	//		info.quest_id = quest->id;
	//		info.quest_level = quest->quest_level;
	//		info.quest_type = quest->quest_type;
	//		strcpy(info.title,quest->title);
	//		//info.npc_id = relation->;
	//		quests.push_back(info);
	//	}		
	//}

	return true;
}

//任务完成后发送任务链的下条任务
void QuestGetter::SendNextChainQuest(Creature *creature,uint32 finish_questid)
{
	//查询任务链的下条任务
	//quest_template_db.add_select_condition(MEMDB_EQ(quest_template,enable,uint32(1)));
	quest_template_db.add_select_condition(MEMDB_EQ(quest_template,prev_quest,finish_questid));	

	MemDB_Result<quest_template> result = quest_template_db.select();
	if(!result.size())
	{
		if(m_obj.GetSession())
			Call_questhelp_next(m_obj.GetSession()->m_delegate_sendpkt,finish_questid,0,creature->GetUIntGuid());
		return;
	}

	//查询该任务及接的NPC
	//quest_wo_relation_db.add_select_condition(MEMDB_EQ(quest_wo_relation,quest_id,finish_questid));
	quest_wo_relation_db.add_select_condition(MEMDB_EQ(quest_wo_relation,wo_entry,creature->GetEntry()));
	MemDB_Result<quest_wo_relation> result_wo = quest_wo_relation_db.select();
	if (!result_wo.size())
	{
		if(m_obj.GetSession())
			Call_questhelp_next(m_obj.GetSession()->m_delegate_sendpkt,finish_questid,0,creature->GetUIntGuid());
		return;
	}

	//恶心的嵌套循环	
	for (uint32 i = 0; i<result.size(); i++)
	{		
		if(result[i]->enable == 0)
			continue;
		for (uint32 j = 0; j < result_wo.size(); j++)
		{
			//if (result_wo[j]->relatetion_type == QUEST_RELATION_TYPE_START &&
			//	result_wo[j]->quest_id == result[i]->id && 
			//	CanSeeStartQuest(result[i]))			
			if (result_wo[j]->relatetion_type == QUEST_RELATION_TYPE_START &&
				result_wo[j]->quest_id == result[i]->id &&
				CanTakeQuest(quest_template_db[result[i]->id]))
			{
				if(m_obj.GetSession())
					Call_questhelp_next(m_obj.GetSession()->m_delegate_sendpkt,finish_questid,result_wo[j]->quest_id,creature->GetUIntGuid());
				return;
			}
		}		
	}
	Call_questhelp_next(m_obj.GetSession()->m_delegate_sendpkt,finish_questid,0,creature->GetUIntGuid());
}

//////////////////////////////////////////////////////////////////////////
//任务接口

bool QuestGetter::AddQuestItem(uint32 entry,uint32 count)
{
 	for( int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if ( questid == 0 )
			continue;

		//正常这里是可以拿到数据的
		if (GetQuestStatus(questid) != QUEST_STATUS_INCOMPLETE )
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if(!qInfo)
			continue;

		int questSlotIndex = -1;
		if (HasFlag(qInfo,_QUEST_FLAGS_FLAGS_DELIVER))
		{
			for (int j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
			{
				uint32 reqitem = qInfo->req_item_id[j];
				if (reqitem == 0)
					break;
				else 
					questSlotIndex++;

				if(reqitem != entry)
					continue;				

				uint32 reqitemcount = qInfo->req_item_count[j];		//要求的数量
				uint32 curitemcount = GetQuestSlotCounter(i, j);		//当前的数量
				if(curitemcount < reqitemcount)
				{
					count += curitemcount;		//将变化量加上去,如果超过最多
					if(count < 0) 
						count = 0;
					else if(count > reqitemcount)
						count = reqitemcount;
					
					SetQuestSlotCounter(i,questSlotIndex,count);		//设置任务要求物品数量					
				}
				
				if ( CanCompleteQuest( questid ) )
					CompleteQuest( questid );

				return true;
			}
		}		
	}
	return false;
}

//增加任务条件下标完成数只做计数用
bool QuestGetter::AddQuestItemBySlot(uint32 quest_id,uint32 slot_id,uint32 count)
{
	if ( quest_id == 0 )
		return false;	
	
	uint16 slot = FindQuestSlot(quest_id);
	if (slot == MAX_QUEST_LOG_SIZE)
		return false;
	
	if ( GetQuestStatus(quest_id) != QUEST_STATUS_INCOMPLETE )
		return false;

	quest_template const* qInfo = quest_template_db[quest_id];
	if(!qInfo)
		return false;;

	int questSlotIndex = -1;
	for (int j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
	{
		questSlotIndex++;

		if(questSlotIndex != slot_id)
			continue;				

		uint32 curitemcount = GetQuestSlotCounter(slot, j);
		SetQuestSlotCounter(slot, questSlotIndex, curitemcount + count);
		return true;
				
	}
	return false;
}

void QuestGetter::KilledMonster(creature_template const *creature)
{
	uint32 addkillcount = 1;
	for( int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if ( questid == 0 )
			continue;

		QuestStatus const quest_state = (QuestStatus)GetQuestSlotState(i);
		if(quest_state != QUEST_STATUS_INCOMPLETE)
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if( !qInfo )
			continue;

		//任务进度在下标的位置
		int questSlotIndex = -1;
		if (HasFlag(qInfo,_QUEST_FLAGS_FLAGS_DELIVER))
		{
			for(int j=0;j<QUEST_OBJECTIVES_COUNT;j++)
			{
				if (qInfo->req_item_id[j] == 0)
					break;
				else 
					questSlotIndex++;
			}
		}		

		if(HasFlag(qInfo,_QUEST_FLAGS_FLAGS_KILL_OR_CAST))
		{
			for(int j=0;j<QUEST_OBJECTIVES_COUNT;j++)
			{
				if (qInfo->req_world_object_id[j] == 0)
					break;
				else 
					questSlotIndex++;

				//跳过地图对象
				if(qInfo->req_world_object_id[j] < 0)
					continue;

				uint32 reqkill = qInfo->req_world_object_id[j];
				if(reqkill == creature->id)
				{
					uint32 reqkillcount = qInfo->req_world_object_count[j];
					uint32 curkillcount = GetQuestSlotCounter(i, j); 
					if(curkillcount < reqkillcount)
					{
						SetQuestSlotCounter(i,questSlotIndex, curkillcount + addkillcount);		//设置任务要求物品数量
					}
					if ( CanCompleteQuest( questid ) )
						CompleteQuest( questid );
				}				
			}
		}		
	}	
}

void QuestGetter::TalkedToCreature( uint32 entry, const string & /*guid*/ )
{
	uint32 addTalkCount = 1;
	for(int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if(!questid)
			continue;
		 
		QuestStatus const quest_state = (QuestStatus)GetQuestSlotState(i);
		if(quest_state != QUEST_STATUS_INCOMPLETE)
			continue;
		
		quest_template const* qInfo = quest_template_db[questid];
		//任务进度在下标的位置
		int questSlotIndex = -1;
		if (HasFlag(qInfo,_QUEST_FLAGS_FLAGS_DELIVER))
		{
			for(int j=0;j<QUEST_OBJECTIVES_COUNT;j++)
			{
				if (!qInfo->req_item_id[j])
					break;
				else 
					questSlotIndex++;
			}
		}
		
		if(HasFlag(qInfo,_QUEST_FLAGS_FLAGS_SPEAKTO | _QUEST_FLAGS_FLAGS_KILL_OR_CAST))
		{
			for(int j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
			{
				if (qInfo->req_world_object_id[j] == 0)
					break;
				else 
					questSlotIndex++;

				if(qInfo->req_world_object_count[j] <= 0)
					continue;

				if(qInfo->req_world_object_id[j] != entry)
					continue;

				uint32 reqTalkCount = qInfo->req_world_object_count[j];
				uint32 curTalkCount = GetQuestSlotCounter(i, j);
				if( curTalkCount < reqTalkCount)
				{
					SetQuestSlotCounter(i,questSlotIndex, curTalkCount + addTalkCount);		//设置任务要求物品数量
					if(CanCompleteQuest(questid))
						CompleteQuest(questid);		//这里不是可以直接返回了	
				}		
			}
		}
	}
}

void QuestGetter::CastedCreatureOrGO(Unit *unit,uint32 /*item_id*/)
{
	//根据guid的前缀判断是否是生物
	bool isCreature = unit->GetTypeId() == TYPEID_UNIT;

	uint32 addCastCount = 1;
	for( int i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if(!questid)
			continue;

		QuestStatus const quest_state = (QuestStatus)GetQuestSlotState(i);
		if ( quest_state != QUEST_STATUS_INCOMPLETE )
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if ( !qInfo  )
			continue;

		//任务进度在下标的位置
		int questSlotIndex = -1;
		if (HasFlag(qInfo,_QUEST_FLAGS_FLAGS_DELIVER))
		{
			for(int j=0;j<QUEST_OBJECTIVES_COUNT;j++)
			{
				if (!qInfo->req_item_id[j])
					break;
				else 
					questSlotIndex++;
			}
		}

		if(HasFlag(qInfo, _QUEST_FLAGS_FLAGS_KILL_OR_CAST ) )
		{
			for (int j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
			{
				if (qInfo->req_world_object_id[j] == 0)
					break;
				else 
					questSlotIndex++;

				int reqTarget = 0;

				if(isCreature)
				{
					// creature activate objectives
					if(qInfo->req_world_object_id[j] > 0)
						// checked at quest_template loading
						reqTarget = qInfo->req_world_object_id[j];
				}
				else
				{
					// GO activate objective
					if(qInfo->req_world_object_id[j] < 0)
						// checked at quest_template loading
						reqTarget = 0 - qInfo->req_world_object_id[j];
				}

				// other not this creature/GO related objectives
				if( reqTarget != unit->GetEntry() )
					continue;

				uint32 reqCastCount = qInfo->req_world_object_count[j];
				uint32 curCastCount = GetQuestSlotCounter(i, j);
				if ( curCastCount < reqCastCount )
				{
					//GetSession()->SendQuestUpdateAddCreatureOrGo( qInfo, guid, j, curCastCount, addCastCount);
					SetQuestSlotCounter(i,questSlotIndex, curCastCount + addCastCount);		//设置任务要求物品数量
					if ( CanCompleteQuest( questid ) )
						CompleteQuest( questid );
				}
				
				break;
			}
		}
	}
}

void QuestGetter::AreaExploredOrEventHappens( uint32 questId )
{
	if(!questId)
		return;

	uint16 log_slot = FindQuestSlot(questId);
	if(log_slot < MAX_QUEST_LOG_SIZE)
	{		
		if(!GetQuestSlotFlag(log_slot, QUEST_SLOT_FLAGS_EXPLORE))
		{
			SetQuestSlotFlag(log_slot, QUEST_SLOT_FLAGS_EXPLORE);	//标志位已探索		
		}
	}

	if( CanCompleteQuest( questId ) )
		CompleteQuest( questId );
}

void QuestGetter::AutoFailOfflineOrDie()
{
	for(int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if(!questid)
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if ( !qInfo  )
			continue;

		if(!IsAutoFailOff(qInfo))
			continue;

		FailQuest(questid);
	}
}

void QuestGetter::OnJoinMap()
{
	//这里可能会有刷怪，所以提前进入地图
	m_obj.GetMap()->Update(0);
}

void QuestGetter::OnLeaveMap()
{

}

//找到提交任务npc模板
uint32 QuestGetter::FindCommitNpcEntry(uint32 questid)
{
	quest_wo_relation_db.add_select_condition(MEMDB_EQ(quest_wo_relation,quest_id, questid));
	MemDB_Result<quest_wo_relation> result_wo = quest_wo_relation_db.select();
	if (!result_wo.size())
	{
		tea_pdebug("FindCommitNpcEntry:没有找到提交任务npc模板");
		return 0;
	}
	for (uint32 j = 0; j < result_wo.size(); j++)
	{
		if (result_wo[j]->relatetion_type == QUEST_RELATION_TYPE_END && result_wo[j]->wo_entry > 0)
		{
			return result_wo[j]->wo_entry;			
		}
	}		
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//获得物品										角色guid		物品模板			叠加数			绑定类型	是否元宝产出		鉴定类型   获取类型(是否必定得到) 放到系统包裹  强化值       有效期     获取渠道，对应LOG_ITEM_OPER_TYPE
int QuestGetter::quest_add_rew_item(const string &lowguid,uint32 item_id,uint32 item_count,uint8 bind_type, uint32 streng,uint32 fail_time, uint8 op_type)
{
	WorldPacket pkt (INTERNAL_OPT_QUEST_ADD_REW_ITEM);	
	pkt << lowguid;	
	pkt << item_id;
	pkt << item_count;
	pkt << bind_type;
	pkt << streng;
	pkt << fail_time;
	pkt << op_type;

	ScenedApp::g_app->SendToAppd(pkt);
	return 0;
}

//将该任务的前置全部完成
void QuestGetter::SetQuestIDPrevComplete(uint32 quest_id)
{
	quest_template const* qInfo = quest_template_db[quest_id];
	int32 prev_quest = qInfo->prev_quest;
	while(prev_quest != 0 && prev_quest != -1)
	{
		//将前置任务设置已完成
		SetCompleteQuestID(prev_quest);
		//清理下任务槽里
		for (int i = 0; i < MAX_QUEST_LOG_SIZE; i++)
		{
			if(GetQuestSlotQuestId(i) == prev_quest)
			{
				SetQuestSlot(i,0);
				break;
			}
		}
		//获取该任务的前置
		qInfo = quest_template_db[prev_quest];
		prev_quest = qInfo -> prev_quest;
	}
}

//杀等级生物
void QuestGetter::KilledLevelMonster(uint32 create_level)
{
	uint32 addkillcount = 1;
	for( int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if ( questid == 0 )
			continue;

		QuestStatus const quest_state = (QuestStatus)GetQuestSlotState(i);
		if(quest_state != QUEST_STATUS_INCOMPLETE)
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if( !qInfo )
			continue;

		//是否是杀等级怪物
		if (!HasFlag(qInfo,QUEST_FLAGS_FLAGS_LEVEL_CREATURE))
			continue;

		//杀等级怪和杀等级玩家时互斥的flag
		if (HasFlag(qInfo,QUEST_FLAGS_FLAGS_LEVEL_PLAYER))
			return;

		if (sizeof(qInfo->req_kill_level_info) == 0)
			return;

		uint32 num = sizeof(qInfo->req_kill_level_info) / sizeof(float);
		for (uint32 t = 0; t < num; t+=2)
		{
			uint32 need_level = (uint32)qInfo->req_kill_level_info[t];
			uint32 need_count = (uint32)qInfo->req_kill_level_info[t+1];
			if (need_level == 0 || need_count == 0)
				break;
			if (create_level < need_level)
				continue;

			for(int j=0;j<QUEST_OBJECTIVES_COUNT;j++)
			{				
				uint32 curkillcount = GetQuestSlotCounter(i, j); 
				if(curkillcount < need_count)
					SetQuestSlotCounter(i,j, curkillcount + addkillcount);		//添加已杀数量
				if (CanCompleteQuest(questid))
					CompleteQuest( questid );				
			}
		}
	}
}

//杀等级玩家
void QuestGetter::KilledLevelPlayer(uint32 bekiller_level)
{
	uint32 addkillcount = 1;
	for( int i = 0; i < MAX_QUEST_LOG_SIZE; ++i )
	{
		uint32 questid = GetQuestSlotQuestId(i);
		if ( questid == 0 )
			continue;

		QuestStatus const quest_state = (QuestStatus)GetQuestSlotState(i);
		if(quest_state != QUEST_STATUS_INCOMPLETE)
			continue;

		quest_template const* qInfo = quest_template_db[questid];
		if( !qInfo )
			continue;

		//是否是杀等级玩家
		if (!HasFlag(qInfo,QUEST_FLAGS_FLAGS_LEVEL_PLAYER))
			continue;

		//杀等级怪和杀等级玩家时互斥的flag
		if (HasFlag(qInfo,QUEST_FLAGS_FLAGS_LEVEL_CREATURE))
			return;

		uint32 num = sizeof(qInfo->req_kill_level_info) / sizeof(float);
		if (num == 0)
			return;
		for (uint32 t = 0; t < num; t+=2)
		{
			uint32 need_level = (uint32)qInfo->req_kill_level_info[t];
			uint32 need_count = (uint32)qInfo->req_kill_level_info[t+1];
			if (need_level == 0 || need_count == 0)
				break;
			if (bekiller_level < need_level)
				continue;

			for(int j=0;j<QUEST_OBJECTIVES_COUNT;j++)
			{				
				uint32 curkillcount = GetQuestSlotCounter(i, j);
				if(curkillcount < need_count)
					SetQuestSlotCounter(i,j, curkillcount + addkillcount);		//添加已杀数量
				if (CanCompleteQuest(questid))
					CompleteQuest( questid );				
			}
		}
	}
}


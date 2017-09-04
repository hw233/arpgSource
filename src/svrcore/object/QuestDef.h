#ifndef _QUEST_DEF_H_
#define _QUEST_DEF_H_

#include <comm/common_stl.h>
#include <shared/storage_strus.h>
#include <object/SharedDef.h>

//是否自动失败
static inline bool IsAutoFailOff(quest_template const *quest) 
{
	return (quest->quest_flags & QUEST_FLAGS_AUTO_FAIL_OFF) != 0;
}

//是否自动完成任务
static inline bool IsAutoComplete(quest_template const *quest) 
{
	ASSERT(quest);
#if 0
	return quest->method ? false : true;
#else
	return (quest->quest_flags & QUEST_FLAGS_AUTO_COMPLETE) != 0;
#endif	
}

//是否每日任务
static inline bool IsDaily(quest_template const *quest)
{
	ASSERT(quest);
	return (quest->quest_flags & QUEST_FLAGS_DAILY) != 0 ;
}

//是否自动接受
static inline bool IsAutoAccept(quest_template const *quest)
{
	ASSERT(quest);
	return (quest->quest_flags & QUEST_FLAGS_AUTO_ACCEPT) != 0;
}

//是否手动完成
static inline bool IsManualComplete(quest_template const *quest)
{
	ASSERT(quest);
	return (quest->quest_flags & QUEST_FLAGS_MANUAL) != 0;
}

//是否自动奖励
static inline bool IsAutoReward(quest_template const *quest)
{
	ASSERT(quest);
	return (quest->quest_flags & QUEST_FLAGS_AUTO_REWARDED) != 0 ;
}

//提交任务的时候是否校验提交任务npc，有以下两种情况不需要校验
//1、非主线任务不校验 2、主线任务、但是有勾选不需要校验标志
static inline bool IsCheckCommitNPC(quest_template const *quest)
{
	ASSERT(quest);
	return !(quest->quest_type != QUEST_TYPE_MAIN || (quest->quest_type == QUEST_TYPE_MAIN && (quest->quest_flags & QUEST_FLAGS_COMMIT_NOT_NEED_NPC)!=0 ));
}

//接受任务时是否需要校验接受任务npc
static inline bool IsCheckAcceptNPC(quest_template const *quest)
{
	ASSERT(quest);
	return !(quest->quest_type != QUEST_TYPE_MAIN || (quest->quest_type == QUEST_TYPE_MAIN && (quest->quest_flags & QUEST_FLAGS_ACCEPT_NOT_NEED_NPC)!=0 ));
}

//是否可放弃
static inline bool CanRemove(quest_template const *quest) 
{
	ASSERT(quest);
	return (quest->quest_flags & QUEST_FLAGS_CANREMOVE) != 0;
}

static inline bool HasFlag(quest_template const *quest,uint32 flags)
{
	ASSERT(quest);
	return (quest->special_flags & flags) != 0 ;
}

//任务状态
struct QuestStatusData
{
	QuestStatusData()
		: m_status(QUEST_STATUS_NONE),m_rewarded(false),
		m_explored(false), m_timer(0), uState(QUEST_NEW)
	{
		memset(m_itemcount, 0, QUEST_OBJECTIVES_COUNT * sizeof(uint32));
		memset(m_creatureOrGOcount, 0, QUEST_OBJECTIVES_COUNT * sizeof(uint32));
	}

	uint32 quest_id;			//任务id
	QuestStatus m_status;		//任务状态
	bool m_rewarded;			//是否取过任务奖励
	bool m_explored;			//是否已探索
	uint32 m_timer;
	QuestUpdateState uState;	//任务更新操作

	uint32 m_itemcount[ QUEST_OBJECTIVES_COUNT ];			//物品数量 数组(一个任务最多可以打四种物品)
	uint32 m_creatureOrGOcount[ QUEST_OBJECTIVES_COUNT ];	//杀死生物或者破坏地图对象(同上)
};

typedef std::map<uint32, QuestStatusData> QuestStatusMap;


#endif


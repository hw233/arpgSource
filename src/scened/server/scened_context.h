#ifndef _SCENED_CONTEXT_H_
#define _SCENED_CONTEXT_H_

#include "scened.h"
#include "scened_app.h"
#include <core_obj/ByteArray.h>
#include "Creature.h"
#include "Player.h"

struct quest_template;

using core_obj::ByteArray;

//场景服会话 上下文信息
class ScenedContext:public PlayerBase,public SvrContext
{
public:
	ScenedContext(int fd,ContextManager *mgr);	

	//函数路由,初始化
	void OpcodeHandlerInitor();

	const string &GetGuid() const {return m_lguid;}
	Player *GetPlayer(){return m_player;}
	void Teleport(float to_x,float to_y,int to_mapid, const string &teleport_id, uint32 line_no = 0,uint32 instance_id = 0);
	void On_Teleport_OK(uint32 fd, uint32 mapid, uint32 instanceid, float x, float y);
	void SetExp()
	{
		if(m_player)
			SetDouble(PLAYER_EXPAND_INT_XP, m_player->GetExp());
	}
	void AddExp(double exp)
	{
		AddDouble(PLAYER_EXPAND_INT_XP,exp);
	}
	void LeaveScened();
protected:
	void Handle_LUA(packet& pkt);
	void Handle_Teleport(packet& pkt);				//开始传送
	void Handle_Move_Stop(packet& pkt);				//停止移动
	void Handle_Unit_Move(packet& pkt);				//Unit移动
	void Handle_Jump(packet& pkt);					//跳
	
	void Handle_Use_Gameobject(packet& pkt);		//使用游戏对象
	
	void Handle_Set_Attack_Mode(packet& pkt);		//选择攻击模式
	void Handle_Select_Target(packet& pkt);			//选择目标
	void Handle_Set_Orient(packet& pkt);
	void Handle_Spell_Start(packet& pkt);			//技能开始
	void Handle_Spell_Stop(packet& pkt);			//技能停止
	void Handle_Resurrection(packet& pkt);			//复活

	void Handle_Chat_Near(packet& pkt);				//就近聊天
	void _Handle_GM_Command(const char *str);		//GM处理命令

	void Handle_Gossip_Hello(packet& pkt);				//NPC闲聊
	void Handle_Gossip_Select_Option(packet& pkt);		//选择闲聊选项

	void Handle_Taxi_Hello(packet& pkt);				//飞行员发起聊天
	void Handle_Taxi_Select_Station(packet& pkt);		//选择传送地点
	void Handle_Syns_Mstime(packet& pkt);			//同步时间
	//void 
	void _Handle_QuestObject_Use(Unit *go);		//使用任务对象时
	void Handle_QuestGiver_Status_Query(packet& pkt);	//任务NPC状态查询	
	void Handle_QuestGiver_Hello(packet& pkt);			//与任务NPC打招呼	
	void Handle_QuestGiver_Query_Quest(packet& pkt);	//查询任务明细
	void Handle_QuestGiver_Accept_Quest(packet& pkt);	//接受任务
	void Handle_QuestGiver_Remove_Quest(packet& pkt);	//放弃任务
	void Handle_QuestGiver_Request_Reward(packet& pkt);	//请求
	void Handle_QuestGiver_Complete_Quest(packet& pkt);	//完成任务
	void Handle_Questhelp_Get_CanAccept_List(packet& pkt);	//可接任务
	void Handle_QuestUpdate_Use_Item(packet& pkt);		//完成物品
	void Handle_QuestHelp_Complete(packet& pkt);		//用于新手教程的直接完成任务
	void Handle_QuestHelp_Update_Status(packet& pkt);	//用于新手教程的任务下标更新
	void Handle_QuestHelp_Query_Book(packet&pkt);		//用于玩家查询天书任务
	void Handle_QuestHelp_Talk_Option(packet&pkt);		//用于任务对话选项执行

	//副本
	void Handle_Instance_Enter(packet& pkt);
	void Handle_Instance_Exit(packet& pkt);
	
	void Handle_Instance_Next_State(packet &pkt);	//客户端发来副本进入下一状态指令
	void Handle_Query_Quest_Status(packet& pkt);
	void Handle_Client_Update_Scened(packet& pkt);
	void Handle_Loot_Select(packet& pkt);

	void Hanlde_Client_Subscription(packet& pkt);
public:
	void Call_Gossip_Message(uint32 npc_guid,uint32 option_id,string &key); //发送闲聊内容
	void Call_Taxi_Station_List(uint32 npc_guid); //发送闲聊内容
	
	void Call_Force_Spell_CD(vector<uint32> &cd_info);
	void Call_Combat_State_Update(uint8 stat); //t通知进入战斗状态或者脱离战斗状态
	void Call_Peace_Mode_CD(uint8 cd);
	void Call_Operation_Failed(uint8 type,uint8 reason);
	void Call_Get_Lottery_Bonus(uint8 reslut);
	void Call_Mount_Evolve_Reslut(uint8 reason);

	bool DelGayFormInviteList(uint32 gay);			//从邀请列表里删除
	void RefreshBookQuest();
	
	bool IsCanDisMove(uint16 map_id);

private:
	Player *m_player;
};

#endif


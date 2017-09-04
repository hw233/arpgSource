#include "appd_app.h"
#include "appd_context.h"
#include "appd_script.h"
#include "appd_object_manager.h"
#include "RankListManager.h"
#include <shared/log_handler.h>
#include <object/OprateResult.h>
#include <comm/b64.h>
#include <comm/crypt.h>

//////////////////////////////////////////////////////////////////////////
//一堆定时器

//更新在线玩家信息
void AppdApp::UpdateOnLInePlayerInfo(TimerHolder& th)
{
	ObjMgr.ForEachPlayer([](AppdContext *player){
		if (player->GetStatus() == STATUS_LOGGEDIN)	//在线玩家
		{
			player->AddOnlineTime(1);
			player->AddDailyOnlineTime(1);
			if (player->GetFCMLoginTime()!=(uint32)-1)
				player->AddFCMLoginTime(1);
		}		
	});

	th._next_time += g_Config.update_online_playerinfo_interval;
}

//每日刷新统计数据
void AppdApp::ResetDailyCounter(TimerHolder& th)
{
	g_app->SetDailyAddBindGold(0);
	g_app->SetDailySubBindGold(0);
	g_app->SetDailyAddGoldIngot(0);
	g_app->SetDailySubGoldIngot(0);
	g_app->SetDailyAddSilver(0);
	g_app->SetDailySubSilver(0);
	th._next_time += 86400;
}

//////////////////////////////////////////////////////////////////////////
//内部协议

//对象更新
int AppdApp::on_centd_guid_object_table(tcp_connection *,server_packet *pkt)
{ 
	static ByteArray bytes ;
	bytes.clear();
	bytes.writeBytes(pkt->content,pkt->wpos);
	bytes.position(0);
	ObjMgr.HandlePacket(0,pkt->head->optcode,bytes);
	return 0;
}

//离线数据来了
int AppdApp::on_offline_opert(tcp_connection *,server_packet *pkt)
{
	string callback_guid, callback_name;
	uint32 callback_id;
	*pkt >> callback_guid >> callback_id >> callback_name;
	AppdContext *player = ObjMgr.FindPlayer(callback_guid);
	if(player)
	{
		tea_pdebug("AppdApp::on_offline_opert player is login, %s  %u", callback_guid.c_str(), player->GetStatus());
		return 0;
	}

	vector<GuidObject*> vec;
	static ByteArray bytes, comp_bs;
	comp_bs.clear();
	comp_bs.writeBytes(pkt->content + pkt->rpos,pkt->wpos - pkt->rpos);
	comp_bs.position(0);
	bytes.clear();
	ObjMgr.UnCompress(comp_bs, bytes);
	bytes.position(0);
	while(bytes.bytesAvailable())
	{
		GuidObject *obj = new BinLogObject;
		int flags = bytes.readByte();
		string guid = bytes.readUTF();
		obj->ReadFrom(flags, bytes);
		obj->guid(guid);
		vec.push_back(obj);
	}
	//for (auto it:vec)
	//{
	//	ASSERT(!ObjMgr.Get(it->guid()));
	//	ObjMgr.AttachObject(it);
	//}
	DoGetObjectsCallBack(vec, callback_guid, callback_name);

	bytes.clear();
	for (auto it:vec)
	{
		it->WriteUpdateBlock(bytes);
		safe_delete(it);
		//ObjMgr.ReleaseObject(it->guid());
	}
	if(bytes.length())
	{
		WorldPacket wpkt(INTERNAL_OPT_OFFLINE_OPER_RESULT);
		bytes.position(0);
		wpkt << callback_guid << bytes;
		g_app->SendToLogind(wpkt);
	}
	return 0;
}

int AppdApp::on_del_char(tcp_connection *,server_packet *pkt)
{
	string player_id;
	*pkt >> player_id;
	AppdContext *player = AppdContext::FindPlayer(player_id);
	if(player)
	{
		player->SetDelete();
		for (int i = 0; i < MAX_RANK_TYPE; i++)
		{
			// 帮派的和他没关系
			if (i == RANK_TYPE_FACTION) {
				continue;
			}
			AppdApp::g_app->m_rank_list_mgr->InsertTask(player_id, (ERankTypes)i);
		}
	}
	return 0;
}

//场景服通知应用服增加money
int AppdApp::on_scened_player_addmoney(tcp_connection* coon, server_packet* pkt)
{
	string player_id;
	uint8 money_type, opt_type, p4, p5;;
	double val;
	string p1;
	int32 p2, p3;
	*pkt >> player_id >> money_type >> opt_type>> val >>p1 >> p2 >> p3 >> p4 >> p5;
	AppdContext *player = AppdContext::FindPlayer(player_id);
	if(player)
	{
		player->addMoney(money_type, opt_type, val, p1, p2, p3, p4, p5);
	}
	return 0;
}

//场景服通知应用服扣money
int AppdApp::on_scened_player_submoney(tcp_connection* coon, server_packet* pkt)
{
	string player_id;
	uint8 money_type, opt_type, p4, p5;;
	double val;
	string p1;
	int32 p2, p3;
	*pkt >> player_id >> money_type >> opt_type>> val >>p1 >> p2 >> p3 >> p4 >> p5;
	AppdContext *player = AppdContext::FindPlayer(player_id);
	if(player)
	{
		player->subMoney(money_type, opt_type, val, p1, p2, p3, p4, p5);
	}
	return 0;
}

//登录服同步guid最大累加数值
int AppdApp::on_logind_sync_maxguid(tcp_connection* coon, server_packet* pkt)
{
	uint32 player_max;
	*pkt >> player_max;
	g_GuidMgr.SyncMaxGuid(player_max);
	return 0;
}

//登陆服通知应用服合服了
int AppdApp::on_logind_merge_server(tcp_connection* coon, server_packet* pkt)
{
	string server_name;
	*pkt >> server_name;
	//如果发过来是本服的，那说明自己是要合服，不需要合并排行榜
	if(g_app->GetServerID() != server_name)
	{
		string data;
		*pkt >> data;
		g_app->MergeGlobalvalue(data);

		//应用服需要做的事情
		DoAppdMergeSomething();
	}
	
	return 0;
}

void AppdApp::MergeGlobalvalue(const string &str)
{
	Tokens token;
	StrSplit(token, str, '\n');	
	//ASSERT(token.size() % 3 == 0);
	for (uint32 i = 0; i + 3 <= token.size(); i+=3)
	{
		string guid = token[i];
		if(guid == GLOBAL_RANK_INFO)
		{
			g_app->m_rank_list_mgr->MergeResort(token[i + 1], token[i + 2]);
			continue;
		}
	}
}



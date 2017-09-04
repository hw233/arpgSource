#include "scened_context.h"
#include "Player.h"
#include "Map.h"
#include "BuffManager.h"
#include <math.h>
#include <object/GameObject.h>
#include <object/OprateResult.h>
#include <shared/log_handler.h>
#include "scened_scripts.h"
#include <shared/chat_analysis.h>
#include <shared/offset_path.h>
#include "GridUpdateMask.h"

//客户端要求传送
void ScenedContext::Handle_Teleport(packet& pkt)
{
	uint32 intGuid;
	if (unpack_teleport(&pkt, &intGuid))
	{
		WriteAttackPacker(GetAccount(), m_lguid, CMSG_TELEPORT, ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	if(!m_player->isCanMove())
	{
		tea_perror("玩家不可移动,还想传送!");
		return;
	}

	Map* currentMap = m_player->GetMap();
	if (!currentMap) {
		return;
	}
	//判断有无这个传送点guid
	char ch[23];
	sprintf(ch, "%c%u", ObjectTypeUnit, intGuid);
	string teleGuid = ch;
	auto it = currentMap->m_gameobjects.find(teleGuid);
	if (it == currentMap->m_gameobjects.end()) {
		return;
	}
	GameObject* tele = it->second;
	if (!tele) {
		tea_perror("why gameobject will ocurr nullptr object");
		return;
	}
	//判断是否在合法范围内传送
	uint16 currX = (uint16)tele->GetPositionX();
	uint16 currY = (uint16)tele->GetPositionY();
	if (!DoCheckTeleport(m_player, tele->GetEntry(), currX, currY)) {
		tea_perror("not on the teleport");
		return;
	}

	// 具体传送
	uint32 map_id = tele->GetToMapId();
	uint16 pos_x = (uint16)tele->GetToPositionX();
	uint16 pos_y = (uint16)tele->GetToPositionY();

	DOWorldMapTeleportScript(m_player,map_id,pos_x,pos_y);
}

void ScenedContext::Handle_Move_Stop(packet& pkt)
{
	uint32 guid;
	uint16 x,y;
	if(unpack_move_stop(&pkt,&guid,&x,&y))
	{
		WriteAttackPacker(GetAccount(),m_lguid,MSG_MOVE_STOP,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	const MapTemplate* map_temp =  m_player->GetMap()->GetMapTemp();
	//	ASSERT(map_temp);
	if (map_temp == NULL)
	{
		tea_perror("assert error: Handle_Unit_Move  ASSERT(map_temp)");
		Close(PLAYER_CLOSE_OPERTE_SCREND_ONE29,"");
		return;
	}

	auto dis = m_player->GetDistance(x,y);
	if (dis > g_Config.error_distance && !IsCanDisMove(m_player->GetMapId()))
	{
		tea_pdebug("ScenedContext::Handle_Unit_Move player [%s] g_Config.error_distance  = %f, from(%f,%f) newpos(%f,%f) dist:%f", m_player->GetGuid().c_str(), g_Config.error_distance, m_player->GetPositionX(),m_player->GetPositionY(),x,y,dis);
		m_player->StopMoving(true);
		return;
	}

	if(!map_temp->IsCanRun(x,y))
	{
		tea_pinfo("ScenedContext::Handle_Unit_Move sync pos, player[%s][%s], pos %d,%d", m_player->GetGuid().c_str(), m_player->GetName().c_str(), x, y);
		//检测下周边四个方向是否可达，如果四个方向都不可达，则服务端强制同步
		uint16 temp_posx = x;
		uint16 temp_posy = y;
		bool isRun = false;
		for (int i = 1; i < 8; i+=2)
		{
			temp_posx += MapTemplate::around[i][0];
			temp_posy += MapTemplate::around[i][1];
			if (map_temp->IsCanRun(temp_posx, temp_posy))
			{
				isRun = true;
				break;
			}			
		}
		if (!isRun)
		{
			m_player->StopMoving(x, y);
			return;
		}
	}

	//判断一下如果是坐标一样直接return了
	int now_x = uint16(m_player->GetPositionX());
	int now_y = uint16(m_player->GetPositionY());
	if(now_x == x && now_y == y)
	{
		tea_pdebug("guid = %s, igron move_stop [%d,%d]",GetGuid().c_str(),x,y);
		return;
	}

	m_player->StopMoving(x, y);
	return;
}

//对象移动
void ScenedContext::Handle_Unit_Move(packet& pkt)
{
	m_player->SetLastActionPktTime();
	uint32 guid;
	uint16 x,y;
	int8 *path;
	uint16 len_6;	
	
	if (unpack_unit_move(&pkt,&guid,&x,&y,&path , &len_6))
	{
		WriteAttackPacker(GetAccount(),m_lguid,MSG_UNIT_MOVE,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	if (m_player->GetBuffManager()->HasBuff(BUFF_JUMP_JUMP)) {
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//防止各种异常
	//必须事偶数
	if (len_6%2!=0 || len_6 == 0)
	{
		tea_perror("坐标点数组数量不对，必须为偶数或者停止包发到这里来了");
		return;
	}

	if (!m_player->isAlive()) {
		m_player->StopMoving(true);
		return;
	}

	//将相当坐标转换出来
	static offset_path of_path;
	of_path.set_origin(x,y);
	of_path.add(len_6,path);
	vector<float> new_path;
	of_path.get(new_path);

	const MapTemplate* map_temp =  m_player->GetMap()->GetMapTemp();
	//	ASSERT(map_temp);
	if (map_temp == NULL)
	{
		tea_perror("assert error: Handle_Unit_Move  ASSERT(map_temp)");
		Close(PLAYER_CLOSE_OPERTE_SCREND_ONE30,"");
		return;
	}

	//同步坐标
	//TODO:验证,第一点是否可以忽略，从数据流程上看，第一点永远是和客户端相同
	//停下来的点也相同，只是谁先到达的问题。dis
	//距离校验如果中途移动速度发生改变又该如何处理？
	auto dis = m_player->GetDistance(x,y);
	if (dis > g_Config.error_distance && !IsCanDisMove(m_player->GetMapId()))
	{
		tea_pdebug("player [%s] g_Config.error_distance  = %f, from(%f,%f) newpos(%f,%f) dist:%f", m_player->GetGuid().c_str(), g_Config.error_distance, m_player->GetPositionX(),m_player->GetPositionY(),path[0],path[1],dis);
		//m_player->MoveTo(m_player->GetPositionX(),m_player->GetPositionY());
		m_player->StopMoving(true);
		return;
	}
	
	//是否被限制移动
	if (!m_player->isCanMove())
	{
		tea_pdebug("ScenedContext::Handle_Unit_Move can not move, player[%s]", m_player->GetGuid().c_str());
		m_player->StopMoving(true);
		return;
	}	

#if PLATFORM == PLATFORM_WINDOWS
	stringstream ss;
	for (int i = 0; i < (int)new_path.size(); i += 2)
	{
		ss << new_path[i] << "," << new_path[i+1] << ";";
	}

	tea_pdebug("Handle_Unit_Move player(%s, %u)(%f,%f), path (%s)", m_player->GetGuid().c_str(), m_player->GetMapId(),
		m_player->GetPositionX(),m_player->GetPositionY(), ss.str().c_str());
#endif

	if (!map_temp->IsCanRun(x,y,new_path))
	{
		tea_pdebug("ScenedContext::Handle_Unit_Move path can not run, player[%s]", m_player->GetGuid().c_str());
		m_player->StopMoving(true);
		return;
	}

	//处理移动前需要处理的逻辑，比如移除打坐buff
	if(!OnMoveTo(m_player->GetMap(), m_player))
		return;

	m_player->SetPosition(x,y);
	m_player->MoveTo(new_path.size(),&new_path[0]);
	//m_player->MoveTo(len_6-2,path+2);

//	ASSERT(status_ == STATUS_LOGGEDIN);
	if (status_ != STATUS_LOGGEDIN)
	{
		tea_perror("assert error: Handle_Unit_Move  ASSERT(status_ == STATUS_LOGGEDIN)");
		Close(PLAYER_CLOSE_OPERTE_SCREND_ONE31,"");
		return;
	}
}

//就近聊天
void ScenedContext::Handle_Chat_Near(packet& pkt)
{
	WriteAttackPacker(GetAccount(),m_lguid,pkt.head->optcode,ACCACK_PACKET_TYPE_DISCARD,"");
	return;
}

//跳跃
void ScenedContext::Handle_Jump(packet& pkt)
{
	WriteAttackPacker(GetAccount(),m_lguid,pkt.head->optcode,ACCACK_PACKET_TYPE_DISCARD,"");
	return; //不能跳了
}

void ScenedContext::On_Teleport_OK(uint32 fd, uint32 mapid, uint32 instanceid, float x, float y)
{
	ASSERT(m_player == NULL);
	//if (m_player != NULL)
	//{
	//	tea_perror("assert error: On_Teleport_OK ASSERT(m_player == NULL)");
	//	Close();
	//	return;
	//}
	ScenedApp::g_app->ChangeSessionID(this, fd);
	m_lguid = guid();
	//生成玩家对象
	m_player = new Player();
	Map *instance = Map::FindInstance(instanceid, mapid);
	//系列地图内传送，实例在但是此子地图未被创建
	ASSERT(instance);

	SetTeleportInfo(0, 0, 0, 0, GetTeleportGuid());
	SetMapId(mapid);
	SetMapLineNo(instance->GetLineNo());
	SetPosition(x, y);
	//改变状态
	m_player->Create(this);
	SetStatus(STATUS_LOGGEDIN);
	//发个登录场景服完毕的包给客户端
	Call_join_or_leave_server(m_delegate_sendpkt, 0, SERVER_TYPE_SCENED, getpid(), ScenedApp::g_app->Get_Connection_ID(), uint32(time(nullptr)));
	instance->JoinPlayer(m_player);	
	//从玩家下标初始化场次id和跨服类型
	instance->InitWaridAndKuafuType(this);
	//发送属性重算 (新建角色才需要重算)
	if (this->GetForce() == 0) {
		ScenedApp::g_app->call_recalculate(guid());
	}

	//// 不是冒险世界才需要记录离开的信息
	//if (!DoIsRiskMap(mapid)) {
	//	// 从冒险地图出来才能累计
	//	if (m_player->GetSession()->GetPickRiskRewardFlag() == 1) {
	//		m_player->GetSession()->SetPickRiskRewardFlag(0);
	//		m_player->GetSession()->SetLastLeaveRiskTime((uint32)time(NULL));
	//	}
	//} else {
	//	// 没有领过的才能领取
	//	if(m_player->GetSession()->GetPickRiskRewardFlag() == 0) {
	//		DoPickRiskReward(m_player);
	//		m_player->GetSession()->SetLastLeaveRiskTime((uint32)time(NULL));
	//		m_player->GetSession()->SetPickRiskRewardFlag(1);
	//	}
	//}

	tea_pdebug("ScenedContext::On_Teleport_OK %s",m_player->GetGuid().c_str());
}

//传送师使用
void ScenedContext::Teleport(float to_x,float to_y,int to_mapid, const string &teleport_id, uint32 line_no,uint32 instance_id)
{
	tea_pdebug("player %s teleport to %d %u %f %f", GetGuid().c_str(), to_mapid, instance_id, to_x, to_y);
	if(status_ != STATUS_LOGGEDIN)
	{
		tea_pwarn("ScenedContext::Teleport status_ != STATUS_LOGGEDIN %s %u %u", guid().c_str(), GetMapId(), to_mapid);
		//Call_operation_failed(m_delegate_sendpkt,OPERTE_TYPE_CLOSE,PLAYER_CLOSE_OPERTE_SCREND_ONE32,"");
		//Close();这里还是不要close了
		return;
	}

	auto m = m_player->GetMap();
	//如果玩家未正确的加入地图，这个时间传送也是失败的
	if (!m || !MapTemplate::GetMapTempalte(m->GetMapId()))
	{
		tea_perror("!m_player->GetMap()[%p] || !MapTemplate::GetMapTempalte(m_player->GetMap()->GetMapId()[%d])",m_player->GetMap(),
			m_player->GetMap()?m_player->GetMap()->GetMapId():0);	
		Close(PLAYER_CLOSE_OPERTE_SCREND_ONE33,"");
		return;
	}
	//地图内传送
	if(to_mapid == m->GetMapId() && teleport_id == m->GetGeneralID() && (line_no == 0 || line_no == m->GetLineNo()))
	{
		const MapTemplate *mt = MapTemplate::GetMapTempalte(to_mapid);
		if (!mt || mt->IsNotValidPos(to_x,to_y))
		{
			tea_perror("!mt || mt->IsNotValidPos(to_x,to_y) %f,%f,%u,%u",to_x,to_y,to_mapid,instance_id);
			return;
		}
		m_player->SetPosition(to_x,to_y);
		m_player->StopMoving(true);			//同步到客户端
		m_player->SetHealth(m_player->GetHealth());
		return;
	}

	//---------------------------------------------------------------------
	//正式的开始传送
	WriteMap(GetAccount(), GetGuid(), GetLevel(), GetMapId(), to_mapid);
	//清掉地图ID,把要前往的地图和坐标写入下标，并发送
	SetTeleportInfo(to_mapid, to_x, to_y, line_no, teleport_id);
	//同步缓存属性
	m_player->SyncCacheData();
	//设置登录状态
	SetStatus(STATUS_DELETE);
}

void ScenedContext::LeaveScened()
{
	//释放unit begin
	ASSERT(m_player);
	m_player->GetMap()->LeavePlayer(m_player);
	//释放前同步下数据
	m_player->SyncCacheData();
	m_player->SyncUnitToPlayerData();
	safe_delete(m_player);
	//释放unit end

	//取消更新事件
	after_update(nullptr);
	//发个离开场景服包给客户端
	Call_join_or_leave_server(m_delegate_sendpkt, 1, SERVER_TYPE_SCENED, getpid(), ScenedApp::g_app->Get_Connection_ID(), uint32(time(nullptr)));
	//fd置为0
	ScenedApp::g_app->ChangeSessionID(this, 0);
}

void ScenedContext::Hanlde_Client_Subscription(packet& pkt)
{
	uint32 guid;
	if(packet_read(&pkt,(char*)&guid,sizeof(guid)))
	{
		WriteAttackPacker(GetAccount(),m_lguid,MSG_CLIENTSUBSCRIPTION,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	if(!m_player || !m_player->GetMap())
		return;

	auto* unit = m_player->GetMap()->FindUnit(guid);
	if(!unit)
	{
		tea_pwarn("Hanlde_Client_Subscription guid:%u unfound!",guid);
		return;
	}

	UpdateMask mask;
	UpdateMask& oldmask = *(gGridUpdateMask.client_subscription_mask_);
	for (int i = 0; i < oldmask.GetCount();i++)	
		if(unit->GetUInt32(i) && oldmask.GetBit(i))
			mask.SetBit(i);
	
	auto *ret_pkt = ObjMgr.GridMallocByteArray();
	ret_pkt->writeShort(MSG_CLIENTSUBSCRIPTION);
	ret_pkt->writeUnsignedInt(guid);
	mask.WriteTo(*ret_pkt);

	for (int i = 0; i < oldmask.GetCount();i++)	
		if(mask.GetBit(i))
			*ret_pkt << unit->GetUInt32(i);
	ScenedApp::g_app->SendPacket(GetFD(),ret_pkt);
	ObjMgr.GridFreeByteArray(ret_pkt);
}

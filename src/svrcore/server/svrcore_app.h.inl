
#include <iostream>
#include <fstream>
#include "shared/progress_bar.h"
#include <protocol/external.h>
#include "shared/lua_packet_binding.h"

//声明
void svrcore_init();
void svrcore_free();
int external_protocol_free_packet(packet *pkt);


//////////////////////////////////////////////////////////////////////////
//for SvrCoreApp

template<class T>
T *SvrCoreApp<T>::g_app = NULL;

//static int __free_packet(struct list_head *p)
//{
//    packet *pkt = list_entry(p, packet, s);
//    external_protocol_free_packet(pkt);
//	return 0;
//}

template<class T>
SvrCoreApp<T>::SvrCoreApp(SvrCoreParams &params, SvrCoreConfig &config):SvrCoreAppImpl(params,config)
{
	for (int i = 0; i < sizeof(m_netgd_request_map)/sizeof(netgd_handle_ptr); i++)
	{
	    m_netgd_request_map[i] = &T::on_netgd_lua;
	}

	m_netgd_request_map[INTERNAL_OPT_REG_SERVER] = &T::on_netgd_reg_server;
	m_netgd_request_map[INTERNAL_OPT_PACKET_GATE] = &T::on_netgd_packet_gate;
	//m_netgd_request_map[INTERNAL_OPT_PACKET_GATE_LIST] = &T::on_netgd_packet_gate_list;
	
	m_netgd_request_map[INTERNAL_OPT_PING_PONG] = &T::on_netgd_ping_pong;

	m_netgd_request_map[INTERNAL_OPT_COMMMAND] = &T::on_commands;

	m_netgd_request_map[INTERNAL_OPT_SPLICE] = &T::on_splice;
	m_netgd_request_map[INTERNAL_OPT_CALLBACK] = &T::on_call_back_result;
}

template<class T>
bool SvrCoreApp<T>::Open()
{
	//校验对象更新的内部协议号与外部协议号
	ASSERT(int(INTERNAL_OPT_UD_OBJECT) == SMSG_UD_OBJECT 
		&& int(INTERNAL_OPT_UD_CONTROL) == CMSG_UD_CONTROL 
		&& int(INTERNAL_OPT_UD_CONTROL_RESULT) == SMSG_UD_CONTROL_RESULT);
	
    out_str(m_logger, "this server type:%u", T::SERVER_TYPE);
    out_str(m_logger, "connect netgd host:%s,port:%u", m_config.netgd_host.c_str(), m_config.netgd_port);

	//连接到网关服
    if(tcp_connector_open(m_rea,
                          m_config.netgd_host.c_str(),
                          m_config.netgd_port,
                          tcp_connection_handle_init,
                          tcp_connection_handle_read,
                          tcp_connection_handle_write,
                          &T::netgd_handle_close,
                          &T::netgd_handle_accept))
    {
        out_error(m_logger, "connect to netgd failed\n");
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
//信号
#define SIGHUP 1
template<class T>
void SvrCoreApp<T>::HookSignals()
{	
	signal(SIGINT, OnSignal);
	signal(SIGTERM, OnSignal);
	signal(SIGHUP, OnSignal);
#ifdef _WIN32
	signal(SIGBREAK, OnSignal);
#else
	signal(SIGPIPE,SIG_IGN);
#endif
}

template<class T>
void SvrCoreApp<T>::UnHookSinals()
{
	signal(SIGINT, 0);
	signal(SIGTERM, 0);
	signal(SIGHUP, 0);
#ifdef _WIN32
	signal(SIGBREAK, 0);
#endif
}

template<class T>
void SvrCoreApp<T>::OnSignal(int s)
{
	switch (s)
	{
	case SIGINT:
	case SIGTERM:
		g_app->TryClose();
		break;
	case SIGHUP:
		g_app->m_config.ReLoad();
		if(g_app->m_logger)
			g_app->m_logger->level = g_app->m_config.log_level;
		break;
#ifdef _WIN32

	case SIGBREAK:
		g_app->TryClose();
		break;
#endif
	}

	if(g_app->m_logger)
		out_str(g_app->m_logger,"catch signal %d",s);

	signal(s, OnSignal);
}

//////////////////////////////////////////////////////////////////////////
//反应堆

//template<class T>
//void *SvrCoreApp<T>::ReactorLoop(void *_app)
//{
//	SvrCoreApp *app = (SvrCoreApp *)_app;
//	while(!app->m_stoping)
//	{
//		if(reactor_loop(app->m_rea,50))
//			return (void*)-1;
//	}
//	out_str(g_app->m_logger,"ReactorLoop over!");
//	return NULL;
//}

//当与网关服连接以后发生
template<class T>
int SvrCoreApp<T>::netgd_handle_accept(io_watcher *wth, EV_FLAGS )
{
    ASSERT(g_app);
    out_str(g_app->m_logger, "SvrCoreApp<T>::netgd_handle_accept");

	//((tcp_connection *)p)->handle_request = tcp_connection_add_recv_packet;//默认包处理函数加入包队列
    ((tcp_connection *)wth->arg)->handle_request = &T::netgd_handle_request;
    ((tcp_connection *)wth->arg)->packet_alloc = internal_protocol_new_packet;
    ((tcp_connection *)wth->arg)->packet_dealloc = internal_protocol_free_packet;
    //((tcp_connection *)p)->tc.arg = p;

    return 0;
}

template<class T>
int SvrCoreApp<T>::netgd_handle_close(io_watcher *iw, EV_FLAGS )
{
	if(!g_app) return 0;
	out_str(g_app->m_logger, "netgd_handle_close");	
	ASSERT((void*)g_app->m_netgd_connection == (void*)iw->arg);	
	g_app->m_netgd_connection = NULL;	
	tcp_connection_free((tcp_connection *)iw->arg);
	return 0;
}

template<class T>
int SvrCoreApp<T>::centd_handle_close(io_watcher *iw, EV_FLAGS )
{
	if(!g_app) return 0;
    out_str(g_app->m_logger, "centd_handle_close");
    g_app->TryClose();		//关闭服务器
	ASSERT((void*)g_app->m_centd_connection == (void*)iw->arg);	
    ASSERT((void*)g_app->m_netgd_connection != (void*)iw->arg);
	g_app->m_centd_connection = NULL;
    tcp_connection_free((tcp_connection *)iw->arg);
    return 0;
}

template<class T>
int SvrCoreApp<T>::netgd_handle_request(tcp_connection *conn, server_packet *pkt)
{
    if(pkt->head->optcode >= MAX_INTERNAL_OPT)
    {
        out_error(g_app->m_logger, "pkt->optcode:%d \n");
        return -1;
    }
	
	if(g_app->GetStatus() == APP_STATUS_CLOSE)
	{
		out_debug(g_app->m_logger, "status is close, pkt->optcode:%d \n");
		return -1;
	}

	if (pkt->head->optcode != INTERNAL_OPT_UD_OBJECT && pkt->head->optcode != INTERNAL_OPT_PING_PONG) {
		out_debug(g_app->m_logger, "netgd_handle_request len:%u optcode:%u", pkt->head->len, pkt->head->optcode);
	}

	int ret = 0;
	ret = g_app->m_netgd_request_map[pkt->head->optcode](conn, pkt);
	conn->packet_dealloc(pkt);

    return ret;
} 

//////////////////////////////////////////////////////////////////////////
//以下为协议

/*服务器进行注册*/
template<class T>
int SvrCoreApp<T>::on_netgd_reg_server(tcp_connection *conn, server_packet *pkt)
{
	int ret = 0, remote_server_type = 0, connection_id = 0, pid = 0;
	ret = unpacket_reg_server(pkt, &connection_id, &remote_server_type, &pid);

	/*解包失败*/
	if(ret != 0)
		return -1;

	if(remote_server_type == SERVER_TYPE_NETGD)
	{
		g_app->m_connection_id = connection_id;
		out_str(g_app->m_logger, "reg server from netgd,this server connid:%d,pid:%d", connection_id,pid);
		g_app->m_netgd_connection = conn;
		ASSERT(g_app->m_connection_id > 0);
		
		out_str(g_app->m_logger, "conn to centd, centd_host:%s, centd_port:%d", g_app->m_config.centd_host.c_str(), g_app->m_config.centd_port);
		if(tcp_connector_open(g_app->get_reactor(),
			g_app->m_config.centd_host.c_str(),
			g_app->m_config.centd_port,
			tcp_connection_handle_init,
			tcp_connection_handle_read,
			tcp_connection_handle_write,
			&T::centd_handle_close,
			&T::netgd_handle_accept))
		{
			out_error(g_app->m_logger, "connect to centd ,failed!");
			g_app->TryClose();
			return false;
		}
	}
	else if(remote_server_type == SERVER_TYPE_CENTD)
	{
		out_str(g_app->m_logger, "reg server from centd,the centd connid:%d,pid:%d", connection_id,pid);
		ASSERT(connection_id > 0);
		g_app->m_centd_connection = conn;
		g_app->SetStatus(APP_STATUS_CONN_CENTD);
	}

	out_str(g_app->m_logger, "reg server in remote server connection_id:%d server_type:%u pid:%d", g_app->m_connection_id, T::SERVER_TYPE, getpid());

	//在网关服上面注册连接
	server_packet *tosend = NULL;
	pack_reg_server(&tosend, g_app->m_connection_id, T::SERVER_TYPE, getpid(), T::SERVER_TYPE == SERVER_TYPE_SCENED ? SERVER_REG_TYPE_PLURAL : SERVER_REG_TYPE_SINGLETON);
	*tosend << g_app->m_params.generic_id;
	update_packet_len(tosend);

	ASSERT(g_app->m_connection_id > 0);
	tcp_connection_send_packet(conn, tosend);		//先是给网关服发,然后是给中心服发

	//回收数据包
	internal_protocol_free_packet(tosend);

	return 0;
}

template<class T>
int SvrCoreApp<T>::on_netgd_lua(tcp_connection *conn, server_packet *pkt)
{
	return do_on_internal_packet(0, pkt);
}

//单包转发
template<class T>
int SvrCoreApp<T>::on_netgd_packet_gate(tcp_connection *, server_packet *pkt)
{	
    packet *to_send = external_protocol_new_packet(0);
//    tcp_client *tc = NULL;
    int ret;// = unpack_packet_gate(pkt, to_send);
	if(ret = unpack_packet_gate(pkt, to_send))
	{
		out_error(g_app->m_logger,"严重错误SvrCoreApp<T>::on_netgd_packet_gate,ret:%d,head[%u,%u],wpos:%u",ret,pkt->head->len,pkt->head->optcode,pkt->wpos);
		return -1;
	}
	ASSERT(to_send->cid);
	packet_list_push(&g_app->m_recv_pkt_list,to_send);
    return ret;
}

//
template<class T>
int SvrCoreApp<T>::on_netgd_ping_pong(tcp_connection *conn, server_packet *pkt)
{
	//tcp_connection_send_packet(conn,pkt);
	return 0;
}

template<class T>
int SvrCoreApp<T>::on_commands(tcp_connection *conn, server_packet *pkt)
{
	uint32 guid;			//命令来源，从客户端过来
	uint32 connectid;		//内部管理器过来
	string cmd;

	*pkt >> guid >> connectid >> cmd;
	Tokens tokens;
	StrSplit(tokens,cmd,',');	

	Command_Handler_Map::iterator it = g_app->m_command_hanlders.find(tokens[0]);
	//如果没有找到相应的处理函数
	if(it == g_app->m_command_hanlders.end())
		return -1;
	return it->second(conn,pkt,guid,connectid,tokens);	
}

template<class T>
int SvrCoreApp<T>::on_call_back_result(tcp_connection *, server_packet *pkt)
{
	EventMgr::EventID id;
	uint8 result;
	*pkt >> id >> result;
	g_app->DispatchCallback(id, result != 0);
	return 0;
}

//当收到切割包的时候。
template<class T>
int SvrCoreApp<T>::on_splice(tcp_connection *conn, server_packet *pkt)
{
	uint32 pkt_conn_id;
	uint16 optcode, index, len;
	uint32 max_size;
	splice_pkt *s_pkt = nullptr;
	*pkt >> pkt_conn_id >> optcode >> max_size >> index >> len;
	time_t t = time(NULL);
	//理论上，只有一个存在，所以不用map，这样更快
	for (auto it = g_app->m_splice_pkt_map.begin(); it != g_app->m_splice_pkt_map.end();)
	{
		splice_pkt *sp = *it;
		//超过五分钟没收到过这包了
		//等于0是本包已经处理过
		//非0是是对方服务器发一半不发了，等待超时
		if(t - sp->t > 300)
		{//这里有个问题，要把这个判断放到服务器的update里吗？没必要吧？？所以有可能释放的时候已经是猴年马月了
			it = g_app->m_splice_pkt_map.erase(it);
			internal_protocol_free_packet(sp->pkt);
			delete sp;
		}
		else
		{
			if(sp->id == pkt_conn_id)	//找到了
			{
				ASSERT(!s_pkt);
				s_pkt = sp;
				ASSERT(max_size == s_pkt->max_size);
			}
			++it;
		}
	}

	//没找到new一个
	if(!s_pkt)
	{
		ASSERT(index == 0);
		s_pkt = new splice_pkt;
		s_pkt->id = pkt_conn_id;
		s_pkt->max_size = max_size;
		s_pkt->pkt = internal_protocol_new_packet(optcode);
		g_app->m_splice_pkt_map.push_back(s_pkt);
	}
	ASSERT(pkt->wpos - pkt->rpos == len);
	packet_write(s_pkt->pkt, (const char *)(pkt->content + pkt->rpos), len);
	update_packet_len(s_pkt->pkt);
	ASSERT(s_pkt->pkt->wpos <= max_size);
	if(s_pkt->pkt->wpos == max_size)
	{
		g_app->m_netgd_request_map[optcode](conn, s_pkt->pkt);
		s_pkt->t = 0;//最后一次收包时间设为0，下次有分包过来，我就会被释放了
	}
	else
	{
		s_pkt->t = t;//最后一次收包时间也更新一下
	}
	return 0;
}

#define COMMAND_CHECK_LEN(l) if(tokens.size() < l)\
	COMMAND_CHECK_LEN1(1)\
	int32 temp_int;\

#define COMMAND_CHECK_LEN1(l) if(tokens.size() < l)\
{\
	SendCommandResult(conn, connectid, "参数不足%u个", l);\
	return 0;\
}\

#define COMMAND_ATOI(t,a,i) t i = 0;\
	if(tokens.size() > a)\
	{\
		if(!isNumeric(tokens[a].c_str()))\
		{\
			SendCommandResult(conn, connectid, "%s不是一个有效的数字", tokens[a].c_str());\
			return 1;\
		}\
		sscanf(tokens[a].c_str(), "%d", &temp_int);\
		i = t(temp_int);\
	}\

#define COMMAND_FIND_PLAYER(t,p,g) t *p = GetPlayerObject(g);\
	if(p == NULL)\
{\
	SendCommandResult(conn, connectid,"ID为 %s 的玩家不在线", g.c_str());\
	return 0;\
}\

#define COMMAND_SEND_SUCCEED SendCommandResult(conn, connectid, "succeed")



template<class T>
int SvrCoreApp<T>::on_print_object(tcp_connection *conn, server_packet *,uint32, uint32 connectid, Tokens& tokens)
{
	COMMAND_CHECK_LEN(3);
	string reg_str = tokens[1];
	COMMAND_ATOI(uint32, 2, print_data);
	g_app->GetObjMgr()->PrintObjectByRegex(reg_str, print_data != 0);
	COMMAND_SEND_SUCCEED;
	return 0;
}

template<class T>
int SvrCoreApp<T>::on_command_memory_recovery(tcp_connection *conn, server_packet *,uint32, uint32 connectid, Tokens&)
{
	//释放包缓存
	svrcore_free();
	svrcore_init();

	//释放byteArray
	g_app->GetObjMgr()->ByteArrayPoolClear();

	COMMAND_SEND_SUCCEED;
	return 0;
}

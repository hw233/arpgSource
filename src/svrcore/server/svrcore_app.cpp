#include <comm/configloader.h>
#include "server/svrcore_app.h"
#include <shared/map_template.h>
#include "object/ServerConnList.h"
#include <comm/crypt.h>

int match_handler(MATCH *m, void *param);

safe_delete_t safe_delete;

//////////////////////////////////////////////////////////////////////////
//for SvrCoreParams
bool SvrCoreParams::Load(int argc,const char **argv,usage_ptr __usage)
{
	int c = 1;

	while(c < argc) 
	{
		if (strcmp(argv[c],"-c") == 0) 
		{
			if (++c >= argc) 
			{
				perror("Runtime-Error: -c option requires an input argument");
				if(__usage)
					__usage(argv[0]);
				return false;
			} 
			else 
			{
				config_file = argv[c];
			}
		}
		else if(strcmp(argv[c],"-id") == 0)
		{
			if (++c >= argc) 
			{
				perror("Runtime-Error: -c option requires an input argument");
				if(__usage)
					__usage(argv[0]);
				return false;
			} 
			else 
			{
				generic_id = atoi(argv[c]);
			}
		}
		else if(strcmp(argv[c],"-run_test") == 0)
		{
			if (++c >= argc) 
			{
				perror("Runtime-Error: -run_test option requires an input argument");
				if(__usage)
					__usage(argv[0]);
				return false;
			} 
			else 
			{
				run_test = atoi(argv[c]);
			}
		}
		else if(strcmp(argv[c], "-centd_port") == 0)
		{
			if (++c >= argc) 
			{
				perror("Runtime-Error: -centd_port option requires an input argument");
				if(__usage)
					__usage(argv[0]);
				return false;
			} 
			else 
			{
				centd_port = (uint16)atoi(argv[c]);
			}
		}
		else if(strcmp(argv[c], "--web_port") == 0)
		{
			if (++c >= argc) 
			{
				perror("Runtime-Error: --web_port option requires an input argument");
				if(__usage)
					__usage(argv[0]);
				return false;
			}
			else 
			{
				web_port = argv[c];
			}
		}
		else if(strcmp(argv[c], "--context_file_name") == 0)
		{
			if (++c >= argc) 
			{
				perror("Runtime-Error: --context_file_name option requires an input argument");
				if(__usage)
					__usage(argv[0]);
				return false;
			}
			else 
			{
				context_file_name = argv[c];
			}
		}
		++c;
	}

	if(config_file=="")
	{
		string str = argv[0];
		string::size_type pos = str.find_last_of('.');
		if(pos != string::npos)
			config_file = str.substr(0,pos);		
		else
			config_file = str;

		config_file += ".conf";
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//for SvrCoreConfig
bool SvrCoreConfig::Load(string file, uint32 centd_port_para)
{
	centd_port = centd_port_para;

	m_config_file_path = file;
	bool ret = ReLoad();

	return ret;
}

bool SvrCoreConfig::ReLoad()
{
	config_loader *loader;
	config_loader_init(&loader);

	if(config_loader_load(loader, m_config_file_path.c_str()))
	{
		config_loader_free(loader);
		return false;
	}

	bool ret = _Load(loader);
	config_loader_free(loader);
	return ret;
}

bool SvrCoreConfig::_Load(config_loader *loader)
{
	char *_value;
	_value = config_loader_get_char(loader, "netgd_host", NETGD_HOST);
	netgd_host = _value;
	free(_value);

	netgd_port = config_loader_get_int(loader, "netgd_port", NETGD_PORT);

	_value = config_loader_get_char(loader, "centd_host", CENTD_HOST);
	centd_host = _value;
	free(_value);

	if(!centd_port)
		centd_port = config_loader_get_int(loader, "centd_port", CENTD_PORT);
	
	_value = config_loader_get_char(loader, "log_name","svrcore");
	log_name = _value;
	free(_value);

	_value = config_loader_get_char(loader,"log_folder","log");
	log_folder = _value;
	free(_value);

	backup_hdd_path = log_folder + "/../var";
	player_data_hdd_path = log_folder + "/../data";
	player_data_hdd_path_recycle = player_data_hdd_path + "/recycle";

	log_level = config_loader_get_int(loader, "log_level", 0);

	_value = config_loader_get_char(loader, "data_folder","../../../contrib/data/");
	storage_path = _value;
	free(_value);

	//是否载入屏蔽词
	_value = config_loader_get_char(loader, "load_pingbi", "N");
	load_pingbi = (_value[0] == 'y' || _value[0] == 'Y');
	free(_value);

	return true;
}

//////////////////////////////////////////////////////////////////////////
//for GameConfig
bool GameConfig::_Load(config_loader *loader)
{
	char *_value;
	_value = config_loader_get_char(loader,"script_folder","../../../contrib/scripts/");
	script_folder = _value;
	free(_value);

	_value = config_loader_get_char(loader,"map_folder","../../../contrib/maps/");
	map_folder = _value;
	free(_value);
	
	return SvrCoreConfig::_Load(loader);
}

//////////////////////////////////////////////////////////////////////////
//for DbSvrCoreConfig
bool DbSvrCoreConfig::_Load(config_loader *loader)
{
	char *_value;

	_value = config_loader_get_char(loader,"db_chars_log","");
	db_chars_log = _value;
	free(_value);

	_value = config_loader_get_char(loader,"db_character","");
	db_character = _value;
	free(_value);

	_value = config_loader_get_char(loader,"logdb_log","");
	logdb_log = _value;
	free(_value);

	_value = config_loader_get_char(loader,"logdb_character",db_character.c_str());
	logdb_character = _value;
	free(_value);

	return GameConfig::_Load(loader);
}

//////////////////////////////////////////////////////////////////////////
//for SvrCoreAppImpl

void *SvrCoreAppImpl::WorldTick(void *arg)
{
	//处理网络
	SvrCoreAppImpl *app = static_cast<SvrCoreAppImpl *>(arg);

	uint32 realCurrTime = 0;
	uint32 realPrevTime = ms_time();

	uint32 diff = 0;
	uint32 prevSleepTime = 0;                               // used for balanced full tick time length near WORLD_SLEEP_CONST

	while(app->m_status != APP_STATUS_CLOSE)
	{
		realCurrTime = ms_time();
		diff = get_time_diff(realPrevTime, realCurrTime);

		app->m_tick_timers++;
		//app->template DoAllThings<CONTEXT_T>(diff);		
		//如果diff超大,则使用50ms
		app->Update(diff >= 2147483647 ? 50 : diff);

		realPrevTime = realCurrTime;

		// diff (D0) include time of previous sleep (d0) + tick time (t0)
		// we want that next d1 + t1 == WORLD_SLEEP_CONST
		// we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
		// d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
		if (diff <= app->WORLD_SLEEP_CONST_ + prevSleepTime)
		{
			prevSleepTime = app->WORLD_SLEEP_CONST_ + prevSleepTime - diff;
			//nd_sleep(prevSleepTime);
			reactor_loop(app->get_reactor(),prevSleepTime);
		}
		else
			prevSleepTime = 0;

	}

	if(app->m_logger)
		out_str(app->m_logger,"SvrCoreApp stop!");

	return NULL;
}

SvrCoreAppImpl::SvrCoreAppImpl(SvrCoreParams &params, SvrCoreConfig &config)
	:m_inter_cb(0),m_params(params), m_config(config), m_status(APP_STATUS_INIT),
	m_logger(0), m_netgd_connection(0), m_centd_connection(0),m_connection_id(-1), 
	m_gid(0),m_sid(0),m_oldsid(0),m_fuck_pingbi(NULL),m_callback_timer(0),m_signal_no(0),
	m_pingpont(0)
{
	WORLD_SLEEP_CONST_ = 50;

	m_tick_timers = 0;	//心跳计数
	//初始化日志
	char *log_file_name = log_filename_helper(config.log_name.c_str(),config.log_folder.c_str());
	log_init(&m_logger, log_file_name, config.log_level);	

	//通讯协议初始化
	svrcore_init();
	out_str(m_logger, "init protocol packet pool!");

	//初始化反应堆及包处理函数
	reactor_init(&m_rea);

	//收到的数据包队列的初始化
	INIT_LIST_HEAD(&m_recv_pkt_list);

	//初始化AC自动机
	m_aca = new AC_AUTOMATA;
	ac_automata_init (m_aca, &match_handler);
}

SvrCoreAppImpl::~SvrCoreAppImpl()
{
	if(m_rea)
	{
		reactor_free(m_rea);
		m_rea = NULL;
	}

	//释放协议池
	out_str(m_logger, "free protocol packet pool!");
	svrcore_free();

	if(m_logger)
	{
		log_free(m_logger);
		m_logger = NULL;
	}

	//释放
	ac_automata_release(m_aca);
	delete m_aca;
	if(m_fuck_pingbi != NULL)
	{
		free(m_fuck_pingbi);
		m_fuck_pingbi = NULL;
	}
}

void SvrCoreAppImpl::Update(uint32 diff)
{
	//处理客户端数据包
	packet *pkt = NULL;
	while(m_status == APP_STATUS_READY_OK && 0 == packet_list_pop(&m_recv_pkt_list, &pkt))
	{		
		ASSERT(pkt->cid);
		auto *context = FindContext(pkt->cid);
		if(context)
			context->HandleRequest(*pkt);
		else	
			out_error(m_logger,"unfind sessionid[%d],optcode[%d]",pkt->cid,pkt->head->optcode );
		external_protocol_free_packet(pkt);
	}

	//跑一下定时器回调
	UpdateTimer(diff);
}

void SvrCoreAppImpl::SendToNetgd(server_packet *pkt)
{
	ASSERT(m_netgd_connection);		
	update_packet_len(pkt);
	tcp_connection_send_packet(m_netgd_connection,pkt);
}

void SvrCoreAppImpl::SendToCentd(server_packet *pkt)
{
	if(!m_centd_connection)
		return;
	update_packet_len(pkt);
	SendSplicePkt(m_centd_connection,pkt);
}

bool SvrCoreAppImpl::SendToServerByFd(WorldPacket &pkt, uint32 fd)
{
	if(fd == 0)
	{
		out_debug(this->m_logger, "SvrCoreAppImpl::SendToFD fd = 0, %u", pkt.GetPkt().head->optcode);
		return  false;
	}

	pkt.GetPkt().cid = fd;
	update_packet_len(pkt.GetPktPtr());
	WorldPacket dst(INTERNAL_OPT_PACKET_CENTD_ROUTER);
	ASSERT(dst.GetPktPtr() != pkt.GetPktPtr());
	//__write_packet(dst.GetPktPtr(),pkt.GetPktPtr());
	packet_write(dst.GetPktPtr(),(char*)&pkt.GetPktPtr()->cid,sizeof(pkt.GetPktPtr()->cid));
	packet_write(dst.GetPktPtr(),(char*)pkt.GetPktPtr()->head,sizeof(packet_head)+pkt.GetPktPtr()->wpos);	
	//不允许包太大太大，如果出现了，请检查相关业务内容。
	if(dst.GetPkt().wpos >= uint16(-1))
	{
		out_str(this->m_logger, "SvrCoreAppImpl pkt size to big fd is %u,optcode[%d] size is %u",fd,pkt.GetPkt().head->optcode,pkt.GetPkt().wpos);
		return false;
	}
	SendToCentd(dst.GetPktPtr());
	return true;
}

bool SvrCoreAppImpl::SendToAppd(WorldPacket &pkt)
{
	return SendToServerByFd(pkt, m_server_conn_list->GetAppdFd());
}

bool SvrCoreAppImpl::SendToPoliced(WorldPacket &pkt)
{
	return SendToServerByFd(pkt, m_server_conn_list->GetPolicedFd());
}

bool SvrCoreAppImpl::SendToLogind(WorldPacket &pkt)
{
	return SendToServerByFd(pkt,  m_server_conn_list->GetLogindFd());
}

bool SvrCoreAppImpl::LoadFuckPingbi(string path)
{
	std::cout << "Load  _fuck_pingbi.txt ING" << std::endl;
	if(m_fuck_pingbi != NULL)
	{
		free(m_fuck_pingbi);
		ac_automata_release(m_aca);
		delete m_aca;
		//初始化AC自动机
		m_aca = new AC_AUTOMATA;
		ac_automata_init (m_aca, &match_handler);
	}

	path += "_fuck_pingbi.txt";
	std::ifstream file(path.c_str(),std::ios::in | std::ios::binary);
	if(!file)
	{
		std::cout << "LoadFuckPingbi fail" << std::endl;
		return false;
	}

	unsigned int i = 0, id = 1, start = 0, count  =0, len;	
	std::string str;
	STRING tmp_str;	
	std::stringstream buffer;

	buffer << file.rdbuf();   
	str = buffer.str();  

	file.close(); 
	len = str.length() + 1;
	m_fuck_pingbi = (char *)malloc(len * sizeof(char *));
	//先分割一下，并统计个数
	for(i = 0; i < len; ++i)
	{
		if(strchr("\r\n", str.c_str()[i]))
		{
			m_fuck_pingbi[i] = 0;
			count++;
		}
		else
			m_fuck_pingbi[i] = str.c_str()[i];
	}
	//这个--该不该加呢？可能后面还多一行空行。
	//不过加了也不影响结果
	barGoLink bg(--count);
	//加到屏蔽组件中。
	for (i = 0; i < len; ++i)
	{
		if(m_fuck_pingbi[i] == 0)
		{
			if(start != i)
			{
				tmp_str.id = id++;
				tmp_str.str = (ALPHA *)(m_fuck_pingbi + start);
				tmp_str.length = i - start;
				ac_automata_add_string(m_aca, &tmp_str);
				bg.step();
			}
			start = i + 1;
		}
	}
	ac_automata_locate_failure (m_aca);
	//std::cout << "Load  _fuck_pingbi.txt OK!" << std::endl;
	return true;
}

bool SvrCoreAppImpl::FuckPingbi(char* str)
{
	ASSERT(m_config.load_pingbi);
	STRING tmp_str;
	tmp_str.str = str;
	tmp_str.length = strlen(tmp_str.str);
	ac_automata_param param;
	param.str = str;
	param.ret = FALSE;
	ac_automata_search (m_aca, &tmp_str, &param);
	ac_automata_reset(m_aca);
	return param.ret != 0;
}

void SvrCoreAppImpl::RegisterTimer(Timer_Callback cb,const char* t,void *param/*=NULL*/)
{
	TimerHolder h;
	memset(&h,0,sizeof(h));	
	h.cb = cb;
	strcpy(h.sTimer,t);
	h.param = param;
	m_timerHolder_toadd.push_back(h);	
}

void SvrCoreAppImpl::RegisterTimer(Timer_Callback cb,uint32 t,void *param/* =NULL */)
{
	TimerHolder h;
	memset(&h,0,sizeof(h));	
	h.cb = cb;
	h._next_time = t;		//如果这个时间比现在晚呢？
	ASSERT(t != 0);	
	h.param = param;
	m_timerHolder_toadd.push_back(h);
}

void SvrCoreAppImpl::CancleTimer(Timer_Callback cb)
{

#ifdef _DEBUG
	/*for (TimerHolderArr::iterator it = m_timerHolder_toadd.begin();
	it != m_timerHolder_toadd.end();++it)
	{
	ASSERT(it->cb != cb);
	}*/
#endif // _DEBUG

	for (TimerHolderArr::iterator it = m_timerHolder.begin();
		it != m_timerHolder.end();++it)
	{
		if(it->cb == cb)
		{
			it->deleted = TRUE;
		}
	}
}

void SvrCoreAppImpl::UpdateTimer(uint32 diff)
{	
	//每分钟去校验一下就好
	m_callback_timer.Update(diff);
	if(!m_callback_timer.Passed())
		return;	
	m_callback_timer.Reset(1000*1);				//1秒检测一次

	uint32 t_now = (uint32)time(NULL);					//现在的标准时间，要加上当前时区的小时数
	uint32 t_day = t_now - t_now % DAY_SECONDS;	//今天0点的时间	

    //这里借用一下，如果没有给网关服发ping包可能会被网关服干掉
    if(m_pingpont < t_now)
    {
		m_pingpont = t_now + 4;
        server_packet *spkt = internal_protocol_new_packet(INTERNAL_OPT_PING_PONG);
        packet_write(spkt,(char*)&t_now,sizeof(t_now));
        update_packet_len(spkt);
        if(m_netgd_connection)
            tcp_connection_send_packet(m_netgd_connection,spkt);
        internal_protocol_free_packet(spkt);
    }

	//将待加入回调加入
	if(!m_timerHolder_toadd.empty())
	{
		m_timerHolder.resize(m_timerHolder.size()+m_timerHolder_toadd.size());
		std::copy(m_timerHolder_toadd.begin(),m_timerHolder_toadd.end(),m_timerHolder.end()-m_timerHolder_toadd.size());
		m_timerHolder_toadd.clear();
	}	
	for (TimerHolderArr::iterator it = m_timerHolder.begin();
		it != m_timerHolder.end();)
	{
		uint32 _next_time_bak = it->_next_time;
		//已经超时
		if(it->_next_time < t_now)
		{
			//如果等于0说明是第一次进，重新算
            if(it->_next_time != 0 && !it->deleted)
				it->cb(*it);

			//计算下一次触发的时机
			if(strlen(it->sTimer)>0)
			{
				uint32 h,m;
				uint32 t_time;
				sscanf(it->sTimer,"%u:%u",&h,&m);
				h = (h + 24 - TIME_ZONE) % 24;
				t_time = 60*(h*60+m);
				//如果今天已经过了就触发明天的
				t_time += (t_now % DAY_SECONDS) > t_time ? DAY_SECONDS : 0;
				//加上天数的起点
				t_time = t_day + t_time;
				it->_next_time = t_time;
			}

			//执行过,并且不需要重新开始
			if(_next_time_bak == it->_next_time)
				it->deleted = TRUE;		
		}

		//如果时间已经过了,则移除
		if(it->deleted)
			it = m_timerHolder.erase(it);
		else ++it;
	}
}


//发包给某链接，如果包太大，则分包发送
#define MAX_SPLICE_PKT_SIZE 20480	//超过这个大小的包就得分包20480
//发送的时候，依据是连接id
//所以网关服分配的每个服务器的连接id必须唯一
void SvrCoreAppImpl::SendSplicePkt(tcp_connection *conn,packet *pkt)
{
	if(pkt->wpos > MAX_SPLICE_PKT_SIZE)
	{
		ASSERT(pkt->wpos < 1076887552);//超过1G，我们的系统是要逆天了吗？
		out_str(m_logger,"Packet size exceeds the MAX_SPLICE_PKT_SIZE,size is %u, opcode:%u , cid:%u", pkt->wpos, pkt->head->optcode, pkt->cid);
		uint32 pos = 0;
		uint16 index = 0, len = 0;
		packet *_pkt = internal_protocol_new_packet(INTERNAL_OPT_SPLICE);
		while (pos < pkt->wpos)
		{
			//算一下本次分包长度
			len = (pos + MAX_SPLICE_PKT_SIZE > pkt->wpos ? pkt->wpos - pos : MAX_SPLICE_PKT_SIZE);
			*_pkt << m_connection_id << pkt->head->optcode << uint32(pkt->wpos) << index << len;
			packet_write(_pkt, (const char *)(pkt->content + pos), len);
			update_packet_len(_pkt);
			tcp_connection_send_packet(conn,_pkt);

			pos += MAX_SPLICE_PKT_SIZE;
			index++;
			packet_clear(_pkt);
		}
		internal_protocol_free_packet(_pkt);
	}
	else
	{
		tcp_connection_send_packet((tcp_connection*)conn,pkt);
	}
}

void SvrCoreAppImpl::RegSessionOpts(uint32 fd)
{
	if(m_opts.size() == 0)
		return;
	if(IsGameServer())
	{
		server_packet *pkt_netgd = NULL;
		pack_reg_to_fd(&pkt_netgd, fd, 
			m_connection_id, m_opts.size(),
			&m_opts[0]);		
		SendToNetgd(pkt_netgd);
		internal_protocol_free_packet(pkt_netgd);
	}
	if((IsPKServer()) && m_pk_opts.size() > 0)
	{
		server_packet *pkt_netgd = NULL;
		pack_reg_to_fd(&pkt_netgd, fd, 
			m_connection_id, m_pk_opts.size(),
			&m_pk_opts[0]);		
		SendToNetgd(pkt_netgd);
		internal_protocol_free_packet(pkt_netgd);
	}
}

void SvrCoreAppImpl::RegSessionOpts(uint32 session_fd,uint16 optcode,uint32 to_fd)
{
	//如果待转向的连接ID为0则转到自身
	if(to_fd == 0)
		to_fd = m_connection_id;
	uint16 opts[] = {optcode};
	server_packet *pkt_netgd = NULL;
	pack_reg_to_fd(&pkt_netgd, session_fd, to_fd, 
		sizeof(opts)/sizeof(opts[0]),opts);		
	SendToNetgd(pkt_netgd);
	internal_protocol_free_packet(pkt_netgd);
}

void SvrCoreAppImpl::RegSessionOpts()
{
	for (auto it = context_map_.begin(); it != context_map_.end(); ++it)
	{
		if(it->second->GetStatus() == STATUS_LOGGEDIN)
			RegSessionOpts(it->second->GetFD());
	}
}

//产生回调
SvrCoreAppImpl::EventMgr::EventID SvrCoreAppImpl::AddCallback(std::function<void(bool)> func)
{
	return m_event_mgr.AddCallback(func);
}

//触发回调
void SvrCoreAppImpl::DispatchCallback(SvrCoreAppImpl::EventMgr::EventID id, bool result)
{
	m_event_mgr.Dispatch(id, result);
	m_event_mgr.DelCallback(id);
}

string SvrCoreAppImpl::Md5WorldWarResult(string guid, uint32 type1, uint32 type2, uint32 result, uint32 data1, uint32 data2, uint32 data3, uint32 data4, time_t t)
{
	stringstream ss;
	ss <<  guid
		<< type1
		<< type2
		<< result
		<< data1
		<< data2
		<< data3
		<< data4
		<< t
		<< KFZ_RESULT_KEY;
	static char m_sign_md5[10000];
	MD5CryptToStr32(ss.str().c_str(), ss.str().length(), m_sign_md5);
	return m_sign_md5;
}

//////////////////////////////////////////////////////////////////////////
//for gobal function

int match_handler(MATCH *m, void *param)
{
	int pos;
	pos = m->position - m->matched_strings->length;	
	ac_automata_param *p = (ac_automata_param*)param;
	for(;pos < m->position; pos++){		
		p->str[pos] = '*';
	}
	p->ret = TRUE;

	return 0;
}


void SendCommandResult(tcp_connection *conn, uint32 connectid, const char *result,...)
{
	ASSERT(conn);
	ASSERT(connectid);
	if (!result)
		return;

	va_list ap;
	char szQuery[1000];
	va_start(ap, result);
	vsnprintf(szQuery, 1000, result, ap);
	va_end(ap);

	string centent = szQuery;

	server_packet *s_pkt = internal_protocol_new_packet(INTERNAL_OPT_COMMMAND_RESULTS);
	*s_pkt << connectid;		//给个ID让他找到返回者即可
	*s_pkt << centent;

	update_packet_len(s_pkt);
	tcp_connection_send_packet(conn,s_pkt);			//不用看了这个连接肯定是中心服的
	internal_protocol_free_packet(s_pkt);
}

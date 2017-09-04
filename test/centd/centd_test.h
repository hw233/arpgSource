#ifndef _CENTD_TEST_H_
#define _CENTD_TEST_H_

#include <gtest/gtest.h>
#include "centd.h"

struct GlobalTEST
{
	 GlobalTEST()
	 {
		svrcore_init();
	 }
	 ~ GlobalTEST()
	 {
		svrcore_free();
	 }
};

extern GlobalTEST ___GlobalTEST;

#if 0
CPPAPI static int centd_test_handle_request(tcp_connection *session,packet *pkt)
{
	session->packet_dealloc(pkt);
	return 0;
}

CPPAPI static int centd_test_hander_accept(io_event* ev,EV_FLAGS flag,void* p)
{
	tcp_connection *conn = (tcp_connection *)p;
	conn->handle_request = NULL;
//	conn->handle_close = centd_test_tcp_connection_free;
	conn->packet_alloc = tcp_connection_default_new_packet;
	conn->packet_dealloc = tcp_connection_default_free_packet;
	conn->tc.arg = conn;
	return 0;
}

struct CentdTest:testing::Test 
{
	CentdTest():testing::Test(),m_netgd_callback(0)
	{	
		m_ready = false;
		m_stoping = false;
		const char *str = 
"\n\
#网关服务IP\n\
netgd_host = 127.0.0.1\n\
#网关服务端口\n\
netgd_port = 10086\n\
\n\
#客户端IP\n\
centd_host = 127.0.0.1\n\
#客户端端口\n\
centd_port = 10087\n\
\n\
#登录数据库连接字符串\n\
db_realmd = 192.168.3.2;3306;dev;asdf;realmd\n\
#角色数据库连接字符串\n\
db_character = 192.168.3.2;3306;dev;asdf;char\n\
\n\
#日志文件名称\n\
log_file = centd_log.txt\n\
#日志等级\n\
log_level = 0\n\
";
		FILE* fp = fopen("my.conf", "w");
		fwrite(str, strlen(str), 1, fp);
		fclose(fp);

		reactor_init(&m_reactor);

		if(m_netgd_callback == NULL)
			m_netgd_callback = centd_test_hander_accept;
	}

	~CentdTest()
	{		
		remove("my.conf");
		tcp_acceptor_close(&netgd_acc);
		
		if(m_reactor){
			reactor_free(m_reactor);
			m_reactor = NULL;
		}
	}

	static void*  RunAcceptServer(void *p)
	{
		CentdTest *ct = static_cast<CentdTest*>(p);

		while(!ct->m_stoping){
			if (reactor_loop(ct->m_reactor,20))
				break;
		}
		return 0;
	}

	void open_netgd()
	{		
		tcp_acceptor_open(&netgd_acc,m_reactor,"0.0.0.0",10086,1,
			tcp_connection_handle_init,
			tcp_connection_handle_read,tcp_connection_handle_write,tcp_connection_handle_close,
			m_netgd_callback);
		
		ndthread_t t;
		m_acc_handle = nd_createthread(CentdTest::RunAcceptServer,this,&t,0);
	}

	virtual void SetUp() 
	{
		centd_params app_params;
		centd_config app_config;

		const char * argv[] = {
			"",
			"-c",
			"my.conf"
		};

		//载入命令行参数
		if(centd_params_parse(&app_params,sizeof(argv)/sizeof(char*),argv)){
			EXPECT_EQ(false,true);
			return;
		}

		//载入配置文件
		if(centd_config_load(&app_config,app_params.config_file)){
			EXPECT_EQ(false,true);
			return;
		}

		//初始化运行环境
		if(centd_env_init(&g_app_env,&app_config)){
			EXPECT_EQ(false,true);
			return;
		}

		//启动网关
		open_netgd();

		//启动服务
		if(centd_service_start(g_app_env)){
			EXPECT_EQ(false,true);
			return;
		}
		
		m_ready = true;

		centd_config_unload(g_app_env->config);
		g_app_env->config = NULL;
	}

	bool m_ready;
	bool m_stoping;
	tcp_acceptor netgd_acc;
	reactor *m_reactor;
	io_event_callback m_netgd_callback;
	ndth_handle m_acc_handle;

	// 在析构前调用 
	virtual void TearDown() 
	{ 
		if(g_app_env != NULL){
			centd_env_free(g_app_env);
			g_app_env = NULL;
		}

		m_stoping = true;
		nd_waitthread(m_acc_handle);
		nd_close_handle(m_acc_handle);
	}		
};
#endif
#endif


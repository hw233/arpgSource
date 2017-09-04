#include "centd_test.h"
#if 0
const char *_test_host = "0.0.0.0";
const unsigned short test_port = 12580;
const int TEST_BUFFER_SIZE = 4096;
const char *_con_host = "127.0.0.1";
extern std::string g_mysql_char_info;

extern int __netgd_handle_request(tcp_connection *conn,packet *pkt);

int __handle_request(tcp_connection *session,packet *pkt)
{
	uint32 a;
	int fd;	
	packet *_pkt;
	packet_init(&_pkt);
	packet_head head_small;
	*pkt>>fd;
	*pkt>>head_small;
	*pkt>>a;
	EXPECT_EQ(a,6);
	EXPECT_EQ(head_small.optcode,2);
	EXPECT_EQ(head_small.len,8);
	g_app_env->stoping=1;
	packet_free(_pkt);
	//tcp_client_free(session->tc);
	//tcp_connection_free(session);
	return 0;
}

int __close_ptr(tcp_connection *session,int error_code)
{
	tcp_client_free(session->tc);
	tcp_connection_free(session);
	return 0;
}

void __Packet_gate(	tcp_connection *conn)
{
	//tcp_connection_init(&conn);
	{
		//创建包
		packet *pkt_create;
		pack_create_conn(&pkt_create,5);
		//__netgd_handle_request(conn,pkt_create);
		tcp_connection_send_packet(conn,pkt_create);
		packet_free(pkt_create);
	}

	{
		//搞成转发包
		packet *pkt_gate;

		//小包
		packet *pkt_info;
		packet_init(&pkt_info);
		pkt_info->head.optcode = 1;
		pkt_info->fd = 5;
		int a = 5;
		*pkt_info<<a;
		update_packet_len(pkt_info);

		pack_packet_gate(&pkt_gate,pkt_info);
		packet_free(pkt_info);
		tcp_connection_send_packet(conn,pkt_gate);
		//__netgd_handle_request(conn,pkt_gate);
		packet_free(pkt_gate);
	}	
}

int __hander_accept(io_event* ev,EV_FLAGS flag,void* p)
{
	tcp_connection *conn;
	tcp_connection_init(&conn);	
	conn->tc = (tcp_client*)p;
	conn->handle_request = __handle_request;
	conn->handle_close = __close_ptr;
	conn->packet_alloc = tcp_connection_default_new_packet;
	conn->packet_dealloc = tcp_connection_default_free_packet;
	tcp_client *cli = (tcp_client*)p;
	cli->arg = conn;
	__Packet_gate(conn);
	return 0;
}


void __init_config(centd_config *config)
{
	//为方便测试生成文本
	FILE* fp = fopen("my.conf", "w");
	if(fp==NULL) //如果失败了 　　
	{
		printf("打开文件失败！");
		return ;
	}
	char *t ="netgd_host = 127.0.0.1\n\
			 netgd_port = 12580\n\
			 client_host = 127.0.0.1\n\
			 client_port = 12580\n\
			 f=1.003\n\
			 db_realmd = 192.168.3.2;3306;dev;asdf;realmd\n\
			 db_character = 192.168.3.2;3306;dev;asdf;char\n";
	fwrite(t, strlen(t), 1, fp);
	fclose(fp);


	centd_config_load(config, "my.conf");
	remove("my.conf");
}


TEST(internal_handle_ptr,中心服务综合测试)
{

	tcp_acceptor acc;
	memset(&acc,0,sizeof(acc));
	reactor *rea;
	reactor_init(&rea);

#ifdef _WIN32
	wsa_initializer(TRUE);
#endif // _DEBUG	

	int ret = tcp_acceptor_open(&acc,rea,_test_host,test_port,4096,
		tcp_connection_handle_read,tcp_connection_handle_write,
		__hander_accept);

	//centd_env *env;
	centd_config config;
	__init_config(&config);
	if(centd_env_init(&g_app_env,&config)){
		EXPECT_EQ(0,1);
		return;
	}

	//__guid_test();


	while(!g_app_env->stoping){
		if (reactor_loop(rea,20))
			break;
	}
	//reactor_loop(rea,20);
	centd_wait(g_app_env);

	if(g_app_env != NULL){
		centd_env_free(g_app_env);
		g_app_env = NULL;
	}
	reactor_free(rea);
	centd_config_unload(&config);
#ifdef _WIN32	
	wsa_initializer(FALSE);
#endif // _DEBUG	
}

struct CreateCharTest: CentdTest {
	CreateCharTest(){}
	~CreateCharTest(){}
};


//GUID生成测试
TEST_F(CreateCharTest,create_guid_test) 
{
	MySQLConnection mysqlConn;
	bool re = mysqlConn.Open(g_mysql_char_info);
	ASSERT_TRUE(re);

	QueryResult *result =  mysqlConn.Query("SELECT MAX(guid) FROM chars");
	if (!result){
		g_guids[NEW_GUID_PLAYER] = 1;
		return;
	}
	Field *fields = result->Fetch();
	uint32 __guid = fields[0].GetUInt32()+1;

	init_new_guid();
	new_guid_load(0);
	EXPECT_EQ(__guid,new_guid(0));
	EXPECT_EQ(__guid,get_new_guid(0));
	for (uint32 i = 0;i<10000;i++)
	{
		__guid++;
		ASSERT_EQ(__guid,get_new_guid(0));
	}
	//还原之前生成的10001个GUID
	new_guid_load(0);
	free_new_guid();
	delete result;
}


//角色创建测试
TEST_F(CreateCharTest,create_char_test)
{
	packet *pkt;
	packet_init(&pkt);
	char_create_info info;
	info.gender  = 0;
	info.faction = 1;
	info.head_id = 5;
	stringstream ss;
	uint32 id = new_guid(0);
	ss<<"菜刀"<<id;
	std::string __name =ss.str();
	strcpy(info.name,__name.c_str());
	*pkt<<info;
	Context *test = new  Context(2);
	test->Handle_Char_Create(*pkt);
	delete test;
	packet_free(pkt);
}
#endif

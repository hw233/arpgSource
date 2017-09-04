#include "centd_test.h"
#include <server/map_manager.h>
#include <handler/cent_player.h>
#include <server/context.h>
#include <handler/scentd_handle.h>

GlobalTEST ___GlobalTEST;

struct MapManagerTest:testing::Test 
{
	MapManager *manager;
	tcp_connection *con_neg;
	cent_player *player;

	MapManagerTest()
	{
		//初始化一些信息
		tcp_connection_init(&con_neg);
		manager = new MapManager(con_neg);
		player = new cent_player;
	}

	virtual ~MapManagerTest()
	{
		delete manager;
		manager = NULL;
		tcp_connection_free(con_neg);
		con_neg = NULL;
		delete player;
		player = NULL;
	}

	virtual void SetUp() 
	{
		player->Create(110,"菜刀哥",1,1,1,1);
		Context::PlayerMaps[110] = player;	
	}

	virtual void TearDown() 
	{
		Context::PlayerMaps[110] = NULL;
	}
};

TEST_F(MapManagerTest,NoticeNetgd)
{
	int fd = 10;
	int scened_fd = 50;
	manager->NoticeNetgd(fd,scened_fd);
	
	packet *recv_pkt = NULL;
	packet_init(&recv_pkt);

	int ret = packet_handle_read(recv_pkt,con_neg->tc.w_msg);
	EXPECT_EQ(ret,0);

	//解包看数据
	int player_fd;
	int to_fd;
	uint8 num;
	uint16 *nums = NULL;
	unpack_reg_to_fd(recv_pkt,&player_fd,&to_fd,&num,&nums);

	int ret2 = memcmp((void*)nums,(void*)&SCENED_OPT[0],sizeof(uint16)*num);
	EXPECT_EQ(ret2,0);

	for (int i = 0;i < num;i++)
	{
		EXPECT_EQ(nums[i],SCENED_OPT[i]);
	}

	free(nums);
	packet_free(recv_pkt);
}

TEST(MapManager,PlayerJoin2)
{	
	//
	//
	//scentd_connection	*info_test[3];
	//map_instance	instance_test[3];
	//tcp_connection *con[3];
	//for(uint8 i = 0 ;i<3;i++)
	//{
	//	tcp_connection_init(&con[i]);
	//	info_test[i] = (scentd_connection*)malloc(sizeof(scentd_connection));
	//	info_test[i]->conn = *(con[i]);
	//	info_test[i]->lineno		= i+1;
	//	info_test[i]->connection_id	= i+1;
	//	g_scentd_map[i+1] = info_test[i];

	//	instance_test[i].scened_conn_id  = info_test[i]->connection_id;
	//	instance_test[i].instanceid	= manager->GetNewInstanceID();
	//	instance_test[i].lineno		= info_test[i]->lineno;
	//	instance_test[i].mapid		= i+1;
	//	manager->m_instances.push_back(instance_test[i]);
	//}

	////情况一：加入指定实例ID的地图
	//uint32 ins = 2;
	//manager->PlayerJoin(110,15,0,0,ins);
	//ins = 0;
	////情况二 有现成地图可用
	//manager->PlayerJoin(110,15,2,2,ins);
	//EXPECT_EQ(ins,1);
	////情况三 需要创建一张新的地图
	//ins = 0;
	//manager->PlayerJoin(110,15,4,2,ins);
	//EXPECT_EQ(ins,3);
	////TODO副本地图
	//
	////释放内存
	//tcp_connection_free(con_neg);
	//for(uint8 i = 0 ;i<3;i++)
	//{
	//	tcp_connection_free(con[i]);
	//	free(info_test[i]);
	//}
}

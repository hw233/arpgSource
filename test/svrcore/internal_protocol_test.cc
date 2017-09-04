#include <gtest/gtest.h>
#include <comm/common.h>
#include <net/netstd.h>
#include <svrcore.h>
#include "protocol/external.h"
//#include <object/PlayerData.h>
//#include <object/UpdateBlock.h>
//
//TEST(internal_protocol,create_conn)
//{
//	svrcore_init();
//	int fd = 1234;
//	packet *_packet;
//	int ret1 = pack_create_conn(&_packet,fd);
//	ASSERT_EQ(_packet->head.optcode,INTERNAL_OPT_CREATE_CONN);
//	ASSERT_EQ(ret1,0);
//
//	int fd2 = 0;
//	int ret2 = unpack_create_conn(_packet,&fd2);
//	ASSERT_EQ(ret2,0);
//
//	ASSERT_EQ(fd,fd2);
//
//	internal_protocol_free_packet(_packet);
//	svrcore_free();
//}
//
//TEST(internal_protocol,destory_conn)
//{
//	svrcore_init();
//	int fd = 1234;
//	packet *_packet;
//	int ret1 = pack_destory_conn(&_packet,fd);
//	ASSERT_EQ(_packet->head.optcode,INTERNAL_OPT_DESTORY_CONN);
//	ASSERT_EQ(ret1,0);
//
//	int fd2 = 0;
//	int ret2 = unpack_destory_conn(_packet,&fd2);
//	ASSERT_EQ(ret2,0);
//
//	ASSERT_EQ(fd,fd2);
//	internal_protocol_free_packet(_packet);
//	svrcore_free();
//}
//
//TEST(internal_protocol,dulicate_conn)
//{
//	svrcore_init();
//	int first_fd = 1234;
//	int second_fd = 5678;
//
//	packet *_packet;
//	int ret1 = pack_dulicate_conn(&_packet,first_fd,second_fd);
//	ASSERT_EQ(_packet->head.optcode,INTERNAL_OPT_DUPLICATE_CONN);
//	ASSERT_EQ(ret1,0);
//
//	int first_fd2 = 0;
//	int second_fd2 = 0;
//	int ret2 = unpack_dulicate_conn(_packet,&first_fd2,&second_fd2);
//	ASSERT_EQ(ret2,0);
//
//	ASSERT_EQ(first_fd,first_fd2);
//	ASSERT_EQ(second_fd,second_fd2);
//	internal_protocol_free_packet(_packet);
//	svrcore_free();
//}
//
//包转发
TEST(internal_protocol,packet_gate)
{
	svrcore_init();
	for (int i = 0; i < 1000000; i ++)
	{
		//先定义一下货物,即要转发的包
		packet *pkt_obj = NULL;
		pkt_obj = external_protocol_new_packet(i%uint16(-1));
		pkt_obj->cid = i;

		//写内容
		const char *hello = "hello";
		packet_write(pkt_obj,hello,strlen(hello)+1);
		update_packet_len(pkt_obj);

		//打包
		server_packet *pkt_dst = NULL;
		pack_packet_gate(&pkt_dst,pkt_obj);	
		ASSERT_NE(pkt_dst,pkt_obj);
		ASSERT_EQ(pkt_dst->head->optcode,INTERNAL_OPT_PACKET_GATE);		

		//解包
		packet *pkt_to = external_protocol_new_packet(0);
		int ret = unpack_packet_gate(pkt_dst,pkt_to);
		ASSERT_NE(pkt_dst,pkt_to);
		ASSERT_EQ(pkt_to->cid , pkt_obj->cid);
		ASSERT_EQ(pkt_to->head->optcode, pkt_obj->head->optcode);
		ASSERT_EQ(pkt_to->wpos, pkt_obj->wpos);			//写入的长度是否相等
		ASSERT_STREQ((char*)pkt_to->content,(char*)pkt_obj->content);	
		external_protocol_free_packet(pkt_to);

		//打包完成还不释放，等验证过了后回收
		external_protocol_free_packet(pkt_obj);
		//external_protocol_free_packet(pkt_obj);

		//回收内存	
		internal_protocol_free_packet(pkt_dst);
		internal_protocol_free_packet(pkt_dst);
	}
	
	svrcore_free();
}

//void build_test_packet_list(list_t *head)
//{
//	for (int i=0;i<1000;i++)
//	{
//		packet *_packet = NULL;
//		_packet = external_protocol_new_packet(CMSG_DELETE_CHAR);
//		_packet->cid = i;
//		packet_write(_packet,(char*)&i,sizeof(i));
//		update_packet_len(_packet);
//
//		list_add_tail(&_packet->s,head);
//	}
//}
//
//void free_test_packet_list(list_t *head)
//{
//	while(!list_empty(head)){
//		list_t *pos = head->next;
//		packet *pkt = list_entry(pos, packet, s) ;
//		list_del(pos);
//		packet_free(pkt);
//	}
//}
//
//int compare_packet_list(list_t *left,list_t *right)
//{
//	int _eq = -1;
//	struct list_head *pos_left,*pos_right;
//
//	pos_left = left->next;prefetch(pos_left->next);
//	list_for_each(pos_right,right){
//		_eq = -1;	//至少比较一次
//
//		packet *pkt_pos_left = list_entry(pos_left,packet,s);
//		packet *pkt_pos_right = list_entry(pos_right,packet,s);
//
//		//比较两个包
//		EXPECT_EQ(pkt_pos_left->head.optcode , pkt_pos_right->head.optcode);
//		EXPECT_EQ(pkt_pos_left->head.len , pkt_pos_right->head.len);
//		EXPECT_EQ(pkt_pos_left->wpos , pkt_pos_right->wpos);
//		EXPECT_EQ(pkt_pos_left->cid , pkt_pos_right->cid);
//
//		int value_left,value_right;
//		int ret = packet_read(pkt_pos_left,(char*)&value_left,sizeof(value_left));
//		EXPECT_EQ(ret,0);
//		ret = packet_read(pkt_pos_right,(char*)&value_right,sizeof(value_right));
//		EXPECT_EQ(ret,0);
//
//		EXPECT_EQ(value_left,value_right);
//
//		pos_left = pos_left->next;
//		prefetch(pos_left->next);
//
//		_eq = 0;
//	}
//	return _eq;
//}
//
////包队列转发
//TEST(internal_protocol,packet_gate_list)
//{
//	svrcore_init();
//	list_t left,right;
//	INIT_LIST_HEAD(&left);
//	INIT_LIST_HEAD(&right);
//
//	//生成测试数据
//	build_test_packet_list(&left);
//
//	packet *dst;
//	pack_packet_gate_list(&dst,&left);
//
//	int ret = unpack_packet_gate_list(dst,&right);
//
//	ASSERT_EQ(0,compare_packet_list(&left,&right));
//
//	//释放内存
//	free_test_packet_list(&left);
//	free_test_packet_list(&right);
//	internal_protocol_free_packet(dst);
//
//	svrcore_free();
//}
//
//
////void __build_test_block_list(list_t *head)
////{
////	for (int i=0;i<1;i++)
////	{
////		Object *obj  = new PlayerData; 
////		string name = "菜刀";
////		static_cast<PlayerData*>(obj)->Create(168,name,i,i,i);
////		//初始气血能量
////		static_cast<PlayerData*>(obj)->SetMaxHealth(i);					
////		static_cast<PlayerData*>(obj)->SetHealth(i);
////		UpdateBlock *update_block = new UpdateBlock(obj,obj);
////		update_block->BuildCreate();
////		list_add_tail(&update_block->s,head);
////	}
////}
//
//
////加入地图
//TEST(internal_protocol,pack_join_map)
//{
//	//svrcore_init();	
//
//	//packet *dst;	
//	//pack_join_map(&dst,1,2,3,4);
//
//	//int fd;
//	//uint32 mapid,instranceid;
//	//uint64 guid;
//	//int ret = unpack_join_map(dst,&fd,&mapid,&instranceid, &guid);
//	//ASSERT_EQ(ret,0);
//
//	//ASSERT_EQ(fd,1);
//	//ASSERT_EQ(mapid,2);
//	//ASSERT_EQ(instranceid,3);
//	//ASSERT_EQ(guid,4);
//
//	////释放内存
//	//internal_protocol_free_packet(dst);
//	//svrcore_free();
//}
////
////
//////加入地图
////TEST(internal_protocol,pack_leave_map)
////{
////	svrcore_init();
////	uint32 mapid,charid;
////	packet *pkt;
////	pack_leave_map(&pkt,50,110);
////	unpack_leave_map(pkt,&mapid,&charid);
////	EXPECT_EQ(mapid,50);
////	EXPECT_EQ(charid,110);
////	internal_protocol_free_packet(pkt);
////	svrcore_free();
////}
//
//TEST(internal_protocol,pack_reg_to_fd)
//{
//	svrcore_init();
//	server_packet *pkt;
//	//pack_reg_to_fd(server_packet **dst,uint32 player_fd,int to_fd,uint8 num,uint16 *opts)
//	uint16 opt[3] ={1,2,3};
//
//	pack_reg_to_fd(&pkt,5,10,3,opt);
//	int player_fd;
//	int to_fd;
//	uint16 num;
//	uint16 *opts;
//	unpack_reg_to_fd(pkt,&player_fd,&to_fd,&num,&opts);
//	ASSERT_EQ(player_fd,5);
//	ASSERT_EQ(to_fd,10);
//	ASSERT_EQ(num,3);
//	uint16 i = *opts;
//	for(uint16 i=1;i<4;i++)
//	{
//		ASSERT_EQ(opts[i-1],i);
//	}
//	packet_free(pkt);
//	free(opts);
//	svrcore_free();
//}
//
//
//TEST(internal_protocol,packet_reg_server)
//{
//	svrcore_init();
//	int fd,type;
//	packet *pkt;
//	pack_reg_server(&pkt,16,10);
//	unpacket_reg_server(pkt,&fd,&type);
//	EXPECT_EQ(fd,16);
//	EXPECT_EQ(type,10);
//	internal_protocol_free_packet(pkt);
//	svrcore_free();
//}
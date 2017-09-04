/************************************************************************
 * file	:svrcore.h                                                                     
 * desc	:该文件引用了svrcore所有相关的头文件
 *		如 通讯协议 对象实体定义 对象更新协议
 *
 *
 * author:	linbc
 * date:	20110429
 * version:
 ************************************************************************/
#ifndef _SVRCORE_H_
#define _SVRCORE_H_

#include <svrcore-internal.h>
#include <net/packet.h>

list_t svrcore_packet_pool;

void svrcore_init()
{
	INIT_LIST_HEAD(&svrcore_packet_pool);
}

void svrcore_free()
{
	packet_list_free(&svrcore_packet_pool);
}

/*
 *	external_protocol_new_packet - 从内部通讯缓存区中取得数据包
 *	@optcode : 命令号
 *
 *	@return	:新的数据包
 *  
 */
packet_ptr external_protocol_new_packet(int optcode)
{
	packet_ptr pkt;
	if(packet_list_pop(&svrcore_packet_pool,&pkt) && packet_init(&pkt)){		
		return NULL;
	}
	packet_clear(pkt);
	pkt->head->optcode = optcode;	
	return pkt;
}

/*
 *	external_protocol_free_packet - 回收数据包
 *	@pkt : 要回收的包指针
 *
 *	@return	:新的数据包
 *  
 */
int external_protocol_free_packet(packet_ptr pkt)
{
	if(pkt){
		packet_list_push(&svrcore_packet_pool,pkt);
		//packet_free(pkt);
	}
	return 0;	
}

#endif


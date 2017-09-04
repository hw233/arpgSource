
#include "internal.h"

/*************************************************************************************/
/*以下为通讯协议*/
/*************************************************************************************/


int pack_packet_gate(server_packet **dst,packet *pkt)
{
	ASSERT(dst);
	ASSERT(pkt->wpos + sizeof(packet_head) == pkt->head->len);
	*dst = internal_protocol_new_packet(INTERNAL_OPT_PACKET_GATE);
	__write_packet(*dst,pkt);
	ASSERT((*dst)->wpos == (sizeof(pkt->cid)+sizeof(packet_head)+pkt->wpos));
	update_packet_len(*dst);
	return 0;
}

int unpack_packet_gate(server_packet *src,packet *ppkt)
{	
	return __read_packet(src,ppkt);
}

int pack_reg_to_fd(server_packet **dst,int player_fd,int to_fd,uint16 num,const uint16 *opts)
{
	*dst = internal_protocol_new_packet(INTERNAL_OPT_REG_TOFD);
	packet_write(*dst,(char*)&player_fd,sizeof(int));
	packet_write(*dst,(char*)&to_fd,sizeof(int));
	packet_write(*dst,(char*)&num,sizeof(uint16));
	packet_write(*dst,(char*)opts,sizeof(uint16)*num);

	update_packet_len(*dst);
	return 0;
}

int unpack_reg_to_fd(server_packet *src,int *player_fd,int *to_fd,uint16 *num,uint16 **opts)
{
	packet_read(src,(char*)player_fd,sizeof(int));
	packet_read(src,(char*)to_fd,sizeof(int));
	packet_read(src,(char*)num,sizeof(uint16));
	(*opts) = (uint16*)malloc(sizeof(uint16)*(*num));			//TODO:这里内存是不是需要处理一下
	packet_read(src,(char*)(*opts),sizeof(uint16)*(*num));

	return 0;
}


int pack_reg_server(server_packet **dst,int fd,int server_type,int pid, int reg_type)
{
	*dst = internal_protocol_new_packet(INTERNAL_OPT_REG_SERVER);
	packet_write(*dst,(char*)&fd,sizeof(uint32));
	packet_write(*dst,(char*)&server_type,sizeof(int));
	packet_write(*dst,(char*)&pid,sizeof(int));
	packet_write(*dst,(char*)&reg_type,sizeof(int));
	update_packet_len(*dst);
	return 0;
}

int unpacket_reg_server(server_packet *src,int* fd,int *server_type,int *pid)
{
	packet_read(src,(char*)fd,sizeof(int));
	packet_read(src,(char*)server_type,sizeof(int));
	packet_read(src,(char*)pid,sizeof(int));
	return 0;
}

int pack_internal_open_time(packet **dst,uint32 open_time,struct timeval *tv)
{
	*dst = internal_protocol_new_packet(INTERNAL_OPT_OPEN_TIME);
	packet_write(*dst,(char*)&open_time,sizeof(uint32));
	packet_write(*dst,(char*)tv,sizeof(struct timeval));
	update_packet_len(*dst);
	return 0;
}

int unpack_internal_open_time(packet *src,uint32 *open_time,struct timeval *tv)
{
	packet_read(src,(char*)open_time,sizeof(uint32));
	packet_read(src,(char*)tv,sizeof(struct timeval));
	return 0;
}

TEA_SVRCORE_API int unpack_internal_write_log(packet *src, uint32 *log_type, time_t *sec, time_t *usec, char **account, char **player_id)
{
	packet_read(src,(char*)log_type,sizeof(uint32));
	packet_read(src,(char*)sec,sizeof(time_t));
	packet_read(src,(char*)usec,sizeof(time_t));
	packet_read_str(src,account);
	packet_read_str(src,player_id);
	return 0;
}

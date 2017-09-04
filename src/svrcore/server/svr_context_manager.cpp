#include "svr_context_manager.h"
#include "protocol/internal.h"
#include <core_obj/ByteArray.h>
#include "protocol/external.h"
#include "object/OprateResult.h"

//////////////////////////////////////////////////////////////////////////
//for IContext
SvrContext::SvrContext(int fd, ContextManager *mgr):fd_(fd),status_(0),contextMgr_(mgr)
{
	m_delegate_sendpkt = fastdelegate::MakeDelegate(this,&SvrContext::SendPacket);
	for (int i=0;i<NUM_MSG_TYPES;i++)
	{
		opcode_handler_[i].name = GetOpcodeName(i);
		opcode_handler_[i].status = STATUS_NEVER;
		opcode_handler_[i].handler = std::bind(&SvrContext::Handle_Null, this, std::placeholders::_1);
		opcode_handler_[i].cooldown = 1;
	}
}

void SvrContext::SendPacket(packet& pkt)
{	
	if(!fd_) return;
	pkt.cid = fd_;
	ASSERT(pkt.head->len>=4);
	ASSERT(pkt.head->len == (pkt.wpos+sizeof(packet_head)));
	ASSERT(pkt.head->optcode > 0 /*&& pkt.head->optcode < NUM_MSG_TYPES*/);
	//out_debug(APP::g_app->get_logger(),"send packet:%d",pkt.head->optcode);
	server_packet *dst = NULL;
	pack_packet_gate(&dst,&pkt);
	contextMgr_->SendToNetgd(dst);
	internal_protocol_free_packet(dst);
}

void SvrContext::Close(int16 type, const string &str, bool is_forced /* = false */)
{
	if(type)
	{
		packet *dst;
		pack_operation_failed(&dst, OPERTE_TYPE_CLOSE, type, str.c_str());
		update_packet_len(dst);
		SendPacket(*dst);
		external_protocol_free_packet(dst);
	}
	contextMgr_->CloseSession(fd_, is_forced);
}

void SvrContext::HandleRequest(packet& pkt)
{
	if(pkt.head->optcode >= NUM_MSG_TYPES)
	{
		out_error(contextMgr_->get_logger(),"handle packet[fd:%u,cmd:%u,len:%u]		invalid command",
			fd_, pkt.head->optcode, pkt.head->len);
		return;
	}

	OpcodeHandler& op_handle = opcode_handler_[pkt.head->optcode];

	//验证会话状态是否符合要求
	if(op_handle.status != status_ &&op_handle.status !=STATUS_NEVER)
	{
		out_error(contextMgr_->get_logger(),
			"handle packet[fd:%u,cmd:%u,len:%u]:%s		require status:%d, but:%d",
			fd_, pkt.head->optcode, pkt.head->len, GetOpcodeName(pkt.head->optcode), op_handle.status, status_);
		return;
	}

	//验证数据包频度
	uint32 now = ms_time();
	if(op_handle.cooldown != 0)
	{		
		uint32 diff = get_time_diff(op_handle.last_msg_time, now);

		if(diff < op_handle.cooldown)		
		{
			out_warn(contextMgr_->get_logger(),
				"handle packet[fd:%u,cmd:%u,len:%u]:%s		cooldown:%u diff:%u",
				fd_, pkt.head->optcode, pkt.head->len, GetOpcodeName(pkt.head->optcode), op_handle.cooldown, diff);	
			return;
		}
		else
			op_handle.last_msg_time = now;
	}

	if(pkt.head->optcode != MSG_PING_PONG)
	{
		out_debug(contextMgr_->get_logger(),
			"handle packet[fd:%u,cmd:%u,len:%u]:%s",
			fd_,pkt.head->optcode,pkt.head->len,GetOpcodeName(pkt.head->optcode));
	}
	op_handle.handler(pkt);
	//context.m_last_packet_ms_time = now;
}
//////////////////////////////////////////////////////////////////////////
//for ContextManager

ContextManager::ContextManager()
{

}

ContextManager::~ContextManager()
{
	RemoveAllContext();
}

void ContextManager::InsertContext(SvrContext *s)
{
	context_map_.insert(std::make_pair(s->GetFD(),s));
}

SvrContext *ContextManager::FindContext(int fd)
{
	auto it = context_map_.find(fd);
	if(it == context_map_.end())
		return NULL;
	return  it->second;
}

void ContextManager::CloseSession(int fd, bool is_forced /* = false */)
{
	server_packet *pkt = internal_protocol_new_packet(INTERNAL_OPT_CENTD_DESTORY_CONN);
	packet_write(pkt,(char*)&fd,sizeof(fd));
	int iBool = is_forced?1:0;
	packet_write(pkt,(char*)&iBool,sizeof(iBool));
	update_packet_len(pkt);
	SendToCentd(pkt);
	internal_protocol_free_packet(pkt);
}

void ContextManager::RomoveContext(int fd)
{
	auto it = context_map_.find(fd);
	if(it != context_map_.end())
		context_map_.erase(it);
}

void ContextManager::RomoveContext(SvrContext *s)
{
	RomoveContext(s->GetFD());
}

void ContextManager::RemoveAllContext()
{
	for(auto it = context_map_.begin();it != context_map_.end();++it)
	{
		delete it->second;			//TODO:做缓存策略			
	}
	context_map_.clear();
}

void ContextManager::ChangeSessionID(SvrContext *s,uint32 newfd)
{
	context_map_.erase(s->GetFD());
	s->fd_ = newfd;
	if(newfd)
		context_map_[newfd] = s;
}

void ContextManager::SendPacket(uint32_t fd,core_obj::ByteArray *pkt)
{
	server_packet *dst = internal_protocol_new_packet(INTERNAL_OPT_PACKET_GATE);
	packet_write(dst,(char*)&fd,sizeof(fd));
	int pkt_head_len = pkt->length()+sizeof(short);
	packet_write(dst,(char*)&pkt_head_len, sizeof(short));
	packet_write(dst,(char*)pkt->data(),pkt->length());			//写入包
	SendToNetgd(dst);
	internal_protocol_free_packet(dst);
}

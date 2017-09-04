#ifndef _SVR_CONTEXT_MANAGER_
#define _SVR_CONTEXT_MANAGER_

#include <svrcore-internal.h>
#include <protocol/external.h>
#include <protocol/internal.h>
#include <functional>

namespace core_obj{class ByteArray;}

class ContextManager;

enum SessionStatus
{	
	STATUS_NEVER		= 0x001,					
	STATUS_AUTHED		= 0x002,					//身份验证通过
	STATUS_LOGGEDIN		= 0x004,					//登录成功
	STATUS_TRANSFER		= 0x008,					//传送中	
	STATUS_TRANSFER2	= 0x010,			
	STATUS_DELETE		= 0x020,					//等待删除
	STATUS_PUT			= 0x040,					//提交数据中
	STATUS_PUT_OK		= 0x080,					//提交数据完毕
};

class SvrContext
{
	friend class ContextManager;
public:
	struct OpcodeHandler
	{
		char 	const* name;
		int 	status;
		std::function<void(packet& pkt)> handler;		
		uint32	cooldown;
		uint32  last_msg_time;
		OpcodeHandler():name(0),status(0)
			,cooldown(0),last_msg_time(0)	
		{
			handler = nullptr;
		};		
	};
public:
	SvrContext(int fd, ContextManager *_contextMgr);
	virtual ~SvrContext(){}

	//处理请求
	virtual void Update(uint32)
	{}

	void SetStatus(int stat){status_ = stat;}
	int GetStatus(){return status_;}
	int GetFD(){return fd_;}

	//发包 
	void SendPacket(packet& pkt);	
	//向网关服请求关闭本连接
	void Close(int16 type, const string &str, bool is_forced = false);
	//处理请求		
	void HandleRequest(packet& pkt);

	//////////////////////////////////////////////////////////////////////////
	//协议
	void Handle_Null(packet& pkt){};

	Delegate_Sendpackt m_delegate_sendpkt;	
protected:
	string m_lguid;			//存储GUID
	int fd_;
	int status_;
	ContextManager *contextMgr_;
	//函数路由表
	OpcodeHandler opcode_handler_[NUM_MSG_TYPES+1];
};

class ContextManager
{
public:
	typedef UNORDERED_MAP<uint32,SvrContext*> ContextMap;
	typedef UNORDERED_MAP<string,SvrContext*> GuidMap;

	ContextManager();
	virtual ~ContextManager();
	
	virtual void SendToNetgd(server_packet *pkt) = 0;
	virtual void SendToCentd(server_packet *pkt) = 0;
	void SendPacket(uint32_t fd,core_obj::ByteArray *pkt);
	virtual log_holder *get_logger() = 0;
	void CloseSession(int fd, bool is_forced = false);
	
	//从容器中移除
	void InsertContext(SvrContext *s);
	void RomoveContext(int fd);
	void RomoveContext(SvrContext *s);
	void RemoveAllContext();
	SvrContext *FindContext(int fd);

	void ChangeSessionID(SvrContext *s,uint32 fd);

	ContextMap context_map_;
	GuidMap guid_map_;
};

#endif // !_SVR_CONTEXT_MANAGER_

#include "ServerConnList.h"
#include <protocol/internal.h>
#include <shared/map_template.h>

bool ServerConnList::HasScenedFd(uint32 conn_id)
{
	return ForEachScened([&conn_id](uint32 fd){
		return fd == conn_id;
	}) > 0;
}

void ServerConnList::SetServerReadyOKFlag(uint32 conn_id)
{
	int32 index = ForEach([&conn_id](uint32 fd, uint32){
		return fd == conn_id;
	});
	ASSERT(index >= 0);		//todo lhs
	SetBit(SERVER_CONN_LIST_FIELD_SERVER_INFO_START + index * MAX_SERVER_CONN_INFO + SERVER_CONN_INFO_FLAG, SERVER_CONN_INFO_FLAG_READY_OK);
	AddUInt32(SERVER_CONN_LIST_FIELD_CONN_NUM + GetUInt32(SERVER_CONN_LIST_FIELD_SERVER_INFO_START + index * MAX_SERVER_CONN_INFO + SERVER_CONN_INFO_TYPE), 1);
}

bool ServerConnList::GetServerReadyOKFlag(uint32 conn_id)
{
	ASSERT(conn_id);
	uint32 index = ForEach([&conn_id](uint32 fd, uint32){
		return fd == conn_id;
	});
	return GetBit(SERVER_CONN_LIST_FIELD_SERVER_INFO_START + index * MAX_SERVER_CONN_INFO + SERVER_CONN_INFO_FLAG, SERVER_CONN_INFO_FLAG_READY_OK);
}

int32 ServerConnList::ForEach(std::function<bool(uint32, uint32)> f)
{
	uint32 index = 0;
	for (uint32 i = SERVER_CONN_LIST_FIELD_SERVER_INFO_START; i < uint32_values_.size(); i += MAX_SERVER_CONN_INFO, index++)
	{
		if(GetUInt32(i + SERVER_CONN_INFO_ID) != 0 && f(GetUInt32(i + SERVER_CONN_INFO_ID), GetUInt32(i + SERVER_CONN_INFO_TYPE)))
			return index;	
	}
	
	return -1;
}

//遍历场景服
int32 ServerConnList::ForEachScened(std::function<bool(uint32)> f)
{
	return ForEach([&](uint32 fd, uint32 type){
		if(type == SERVER_TYPE_SCENED && f(fd))
			return true;
		return false;
	});
}


uint32 ServerConnList::GetServerFd(uint32 server_type, bool must_readok)
{
	ASSERT(server_type != SERVER_TYPE_SCENED);//不支持场景服

	uint32 cid = 0;
	ForEach([&](uint32 fd, uint32 type){
		if(type == server_type && (!must_readok || GetServerReadyOKFlag(fd)))
		{
			cid = fd;
			return true;
		}
		return false;
	});
	
	return cid;
}


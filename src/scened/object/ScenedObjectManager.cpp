#include "scened_app.h"
#include "ScenedObjectManager.h"
#include "Player.h"
#include "Map.h"
#include "scened_context.h"

ScenedObjectManager::ScenedObjectManager(SvrCoreAppImpl *app):SvrObjectManager(app)
{	
	//建立队伍和玩家的索引
	indexer_.CreateIndex(REGEX_PLAYER);
	m_compress_tool.set_send_delegate([&](uint32_t cid,ByteArray *pkt){
		ScenedApp::g_app->SendPacket(cid,pkt);
		Free(*pkt);
	});
}

ScenedObjectManager::~ScenedObjectManager()
{

}

void ScenedObjectManager::Update()
{
	SvrObjectManager::Update();
}

//对象工厂
GuidObject *ScenedObjectManager::ObjectFactor(const string &guid)
{
	GuidObject *ptr = NULL;
	switch (GuidManager::GetPrefix(guid))
	{
	case ObjectTypePlayer:
		{
			ptr = new ScenedContext(0, ScenedApp::g_app);
		}
		break;
	case ObjectTypeUnit:
		ASSERT(false);
		break;
	default:
		ptr = ScenedApp::g_app->CreateGlobalValues(this, guid);
		if(!ptr)
			ptr = new BinLogObject;
		break;
	}
	return ptr;
}

GuidObject *ScenedObjectManager::CreateObject(string guid)
{
	tea_pdebug("ScenedObjectManager::CreateObject guid = '%s' ", guid.c_str());
	GuidObject *ptr = ObjectFactor(guid);
	ASSERT(ptr);	
	ptr->guid(guid);
	AttachObject(ptr);
	return ptr;
}

void ScenedObjectManager::ReleaseObject(string guid)
{
	BinLogObject *binlog = dynamic_cast<BinLogObject *>(Get(guid));
	if (binlog)
	{
		tea_pdebug("ScenedObjectManager::ReleaseObject guid = '%s' ", guid.c_str());
		DelOwnerMap(binlog->GetOwner(), binlog);
		if (binlog->GetOwner() == binlog->guid()) {
			ScenedContext *session = dynamic_cast<ScenedContext *>(binlog);
			if (session) {
				ASSERT(!session->GetPlayer());
			}
		}
	}

	SvrObjectManager::ReleaseObject(guid);
}

//发包函数直接到中心服
void ScenedObjectManager::Send(uint32_t connid,ByteArray& bytes)
{
	bytes.position(0);
	WorldPacket pkt(0);
	WriteTo(bytes,pkt.GetPkt());
	ScenedApp::SendToCentd(pkt);
}

//打包玩家操作日志到中心服
void ScenedObjectManager::SendPlayerBinlog(ScenedContext *player)
{
	//从库肯定是往主库更新
	ByteArray& bytes = MallocByteArray(opt_update_data_);
	player->WriteUpdateBlock(bytes);
	//发送数据包
	if(bytes.length() > sizeof(OptionID))
		CallUpdateObject(connection_id(),bytes);
	//释放数据包
	Free(bytes);
	player->Clear();
}

//取得玩家unit
Player *ScenedObjectManager::FindPlayer(string guid)
{
	//如果是玩家需要验证一下是否存在会话并且
	ASSERT(GuidManager::GetPrefix(guid) == ObjectTypePlayer);
	
	ScenedContext *player_data = dynamic_cast<ScenedContext*>(Get(guid));
	if(!player_data)
		return NULL;
	return player_data->GetPlayer();
}

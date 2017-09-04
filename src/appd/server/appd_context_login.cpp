#include "appd_app.h"
#include "appd_context.h"
#include "appd_script.h"
#include "appd_object_manager.h"
#include <svrcore/server/Storage.h>
#include <shared/storage_loader.h>
#include <shared/log_handler.h>
#include <object/OprateResult.h>

void AppdContext::Logout()
{
	tea_pdebug("AppdContext::Logout %s %u", guid().c_str(), fd_);
	if(GetStatus() == STATUS_LOGGEDIN)
	{
		
		//玩家下线以后，做点什么。
		DoPlayerLogout();

		//从账号map中删除
		RemovePlayerMap();

		//状态置为下线
		SetStatus(STATUS_DELETE);

		//还不需要释放，所以置0就好
		AppdApp::g_app->ChangeSessionID(this,0);

		// 把自己从等级列表清除
		this->offline();
	}
	else
	{
		tea_pdebug("AppdContext::Logout %s status != logind", guid().c_str());
	}	
	
	//释放数据
	string lguid = guid();
	ObjMgr.CallDelTagWatch(guid());
	//todo，是否先释放？
}

bool AppdContext::Login(uint32 fd)
{
	tea_pdebug("AppdContext::Login %s %u", guid().c_str(), fd);
	if(fd_ != 0)
	{
		//上一个还没退出，是因为同步问题造成的，关了，几率较小
		tea_pwarn("AppdContext::Login fd_ != 0  %s  %u  %u", guid().c_str(), fd, fd_);
		//Logout();
		contextMgr_->CloseSession(fd);
		return false;
	}
	AppdApp::g_app->ChangeSessionID(this,fd);
	m_lguid = guid();
	SetStatus(STATUS_AUTHED);		//到这里的都是验证通过的

	//初始化金钱
	for (uint32 i = 0; i < MAX_MONEY_TYPE; i++)
	{
		m_all_money[i] = GetDouble(PLAYER_EXPAND_INT_MONEY + i*2);
	}
	if(!AppdApp::g_app->IsPKServer())
	{
		NewOtherBinlog();
	}
	//插入账号信息查询库
	InsertPlayerMap();

	AppdApp::g_app->RegSessionOpts(fd_);
	//登录完毕
	SetStatus(STATUS_LOGGEDIN);
	//发个登录应用服完毕的包给客户端
	Call_join_or_leave_server(m_delegate_sendpkt, 0, SERVER_TYPE_APPD, getpid(), AppdApp::g_app->Get_Connection_ID(), uint32(time(nullptr)));

	//玩家登录以后，做点什么。 TODO: 这里决定是否需要处理邀请的玩家
	DoPlayerLogin();
	if(!AppdApp::g_app->IsPKServer())
	{
		AppdApp::g_app->login_account_guid(this);
	}

	// 把自己加到等级列表中
	this->online();

	//同步时间
	packet pkt;
	Handle_Syns_Mstime(pkt);

	return true;
}


void AppdContext::firstLoginAndInitQuestGuidObject() {
	this->OnAddFirstQuest();
}

void AppdContext::firstLoginAndCopySpellInfoToGuidObject(GuidObject* obj) {
	//应该是遍历的时候槽位 >6 的算特殊技能
	uint32 count = 0;
	for (int i = PLAYER_INT_FIELD_SPELL_START; i <	PLAYER_INT_FIELD_SPELL_END; ++ i) {

		uint32 val = this->GetUInt32(i);
		if (val > 0) {

			uint16 id = this->GetUInt16(i, SHORT_SLOT_SPELL_ID);
			uint8  lv = this->GetByte(i, BYTE_SLOT_SPELL_LV);
			uint8  st = this->GetByte(i, BYTE_SLOT);
			if (st <= SLOT_ANGER) {

				obj->SetUInt16(count + SPELL_INT_FIELD_BASE_SPELL_START, SHORT_SPELL_ID, id);
				obj->SetUInt16(count + SPELL_INT_FIELD_BASE_SPELL_START, SHORT_SPELL_LV, lv);
				count ++;
			}
		}
	}
	obj->SetUInt32(SPELL_BASE_COUNT, count);

	this->generateTempSpellLevelInfo(obj);
}

void AppdContext::generateTempSpellLevelInfo(GuidObject* obj) {
	uint32 sizeIndex = obj->GetUInt32(SPELL_BASE_COUNT) + SPELL_INT_FIELD_BASE_SPELL_START;
	for (uint32 i = SPELL_INT_FIELD_BASE_SPELL_START; i < sizeIndex; ++ i) {
		uint16 spellId = obj->GetUInt16(i, SHORT_SPELL_ID);
		uint8 spellLv = (uint8)obj->GetUInt16(i, SHORT_SPELL_LV);
		this->m_SpellLevel[spellId] = spellLv;
		//基础技能的辅助技能
		this->mapSupportSpell(spellId, spellLv);
	}

	/************************************************************************/
	/* 坐骑的辅助技能                                                       */
	/************************************************************************/
	// 坐骑进阶技能
	for (uint32 i = SPELL_INT_FIELD_MOUNT_SPELL_START; i < SPELL_INT_FIELD_MOUNT_SPELL_END; ++ i) {
		uint16 spellId = obj->GetUInt16(i, SHORT_SPELL_ID);
		if (spellId == 0) {
			break;
		}
		uint8 spellLv = (uint8)obj->GetUInt16(i, SHORT_SPELL_LV);
		this->m_SpellLevel[spellId] = spellLv;
		this->mapSupportSpell(spellId, spellLv);
	}

	//坐骑幻化技能
	for (uint32 i = SPELL_INT_FIELD_MOUNT_ILLUSION_START; i < SPELL_INT_FIELD_MOUNT_ILLUSION_END; i += MAX_ILLUSION_ATTR_COUNT) {
		
		uint32 offs = i + ILLUSION_ATTR_SPELL_START;
		uint32 offt = i + ILLUSION_ATTR_SPELL_END;
		for (uint32 j = offs; j < offt; ++ j) {

			uint16 spellId = obj->GetUInt16(j, SHORT_SPELL_ID);
			if (spellId == 0) {
				break;
			}
			uint8 spellLv = (uint8)obj->GetUInt16(j, SHORT_SPELL_LV);
			this->m_SpellLevel[spellId] = spellLv;
			this->mapSupportSpell(spellId, spellLv);
		}
	}

	// 神兵的辅助技能
	for (uint32 i = SPELL_DIVINE_START; i < SPELL_DIVINE_END;  i+= MAX_DIVINE_COUNT ){
		if (obj->GetByte(i,0) != 0){ 
			uint16 sid = obj->GetUInt16(i+DIVINE_SKILL,0);
			uint8 lev = (uint8)obj->GetUInt16(i+DIVINE_SKILL,1);

			if (sid != 0){
				this->m_SpellLevel[sid] = lev;
				this->mapSupportSpell(sid, lev);
			}

			for (uint32 j=0;j<3;j++){
				sid = obj->GetUInt16(i+DIVINE_PASSIVE_SKILL+j,0);
				lev = (uint8)obj->GetUInt16(i+DIVINE_PASSIVE_SKILL+j,1);
				if (sid != 0){
					this->m_SpellLevel[sid] = lev;
					this->mapSupportSpell(sid, lev);
				}
			}
		}
	}


	this->generatePassiveIndexInfo();
}

void AppdContext::mapSupportSpell(uint16 spellId, uint8 spellLv) {
	skill_base* base = skill_base_db[spellId];
	if (base && base->is_initiative == SPELL_SUPPORT) {
		this->l_supportSpell.push_back(spellId);
	}
}

void AppdContext::generatePassiveIndexInfo() {
	uint16 size = (uint16)this->uint32_values_size();
	for (uint16 i = PLAYER_INT_FIELD_PASSIVE_SPELL_START; i < size; ++ i) {
		uint16 spellId = this->GetUInt16(i, SHORT_SPELL_ID);
		if (spellId == 0) {
			continue;
		}
		uint8 spellLv = (uint8)this->GetUInt16(i, SHORT_SPELL_LV);
		this->m_PassiveIndex[spellId] = i;
		this->m_SpellLevel[spellId] = spellLv;
	}
}

void AppdContext::NewOtherBinlog()
{

	/*NewOtherBinlog(ObjectTypeSocial, PLAYER_APPD_INT_FIELD_FLAGS_SOCIAL_CREATE,[](){
		BinLogObject *binlog = new BinLogObject;
		binlog->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_4);
		return binlog;
	});*/
	NewOtherBinlog(ObjectTypeGiftPacks, PLAYER_APPD_INT_FIELD_FLAGS_GIFT_CREATE,[](){
		BinLogObject *binlog = new BinLogObject;
		binlog->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_3);
		return binlog;
	});
	// NewOtherBinlog(ObjectTypeLimit, PLAYER_APPD_INT_FIELD_FLAGS_LIMIT_CREATE);
	NewOtherBinlog(ObjectTypeItemMgr, PLAYER_APPD_INT_FIELD_FLAGS_ITEM_CREATE);
	// 玩家逻辑
	NewOtherBinlog(ObjectTypeLogical, PLAYER_APPD_INT_FIELD_FLAGS_LOGICAL_CREATE);
	// 玩家技能
	NewOtherBinlog(ObjectTypeSpell, PLAYER_APPD_INT_FIELD_FLAGS_SPELL_CREATE);
	// 玩家副本
	NewOtherBinlog(ObjectTypeInstance, PLAYER_APPD_INT_FIELD_FLAGS_INSTANCE_CREATE);
	// 社交信息ObjectTypeSocial
	NewOtherBinlog(ObjectTypeSocial,PLAYER_APPD_INT_FIELD_FLAGS_SOCIAL_CREATE);
	// 任务
	NewOtherBinlog(ObjectTypeQuest, PLAYER_APPD_INT_FIELD_FLAGS_QUEST_CREATE);	
}

void AppdContext::CreateOtherBinlog(const string new_guid, uint32 flag, std::function<BinLogObject*()> create_fun)
{
	BinLogObject *binlog = nullptr;
	if(create_fun == nullptr)
	{
		binlog = new BinLogObject(core_obj::SYNC_SLAVE | core_obj::SYNC_LOCAL);
	}
	else
	{
		binlog = create_fun();
	}
	ASSERT(binlog);
	binlog->SetGuid(new_guid);
	binlog->SetOwner(guid());
	SetFlags(flag);
	vector<GuidObject*> vec;
	vec.push_back(binlog);
	ObjMgr.CallPutObjects(guid(), vec);

	// 如果创建的是技能对象那就让技能槽的对象拷贝过去
	if (GuidManager::GetPrefix(new_guid) == ObjectTypeSpell) {
		this->firstLoginAndCopySpellInfoToGuidObject(binlog);
		// 第一次初始化任务对象
	} else if (GuidManager::GetPrefix(new_guid) == ObjectTypeQuest) {
		this->firstLoginAndInitQuestGuidObject();
	}
}

void AppdContext::NewOtherBinlog(const string new_guid, uint32 flag, std::function<BinLogObject*()> create_fun)
{
	if (!ObjMgr.Get(new_guid))//如果不存在，重新建一个
	{
		tea_pdebug("AppdContext::NewOtherBinlog %s %s", guid().c_str(), new_guid.c_str());
		if(GetFlags(flag))
		{
			tea_pinfo("AppdContext::NewOtherBinlog err,player->GetFlags(%u), %s %s", flag, guid().c_str(), new_guid.c_str());
			string player_guid = guid();
			ObjMgr.CallAddWatch(new_guid, [player_guid, new_guid, flag, create_fun](bool b){
				AppdContext *player = ObjMgr.FindPlayer(player_guid);
				if(b)
				{
					tea_pinfo("AppdContext::NewOtherBinlog callback ok player->GetFlags(%u), %s", flag, player_guid.c_str());
					ObjMgr.CallSetTag(new_guid, player_guid);
					if(player) {
						ObjMgr.InsertObjOwner(new_guid);
						// 技能的话生成临时数据
						if (GuidManager::GetPrefix(new_guid) == ObjectTypeSpell) {
							player->generateTempSpellLevelInfo(ObjMgr.Get(new_guid));
						}
					}
					else
						ObjMgr.CallDelWatch(new_guid);
				}
				else
				{
					tea_pinfo("AppdContext::NewOtherBinlog callback err player->GetFlags(%u), %s", flag, player_guid.c_str());
					if(player && player->GetStatus() == STATUS_LOGGEDIN)
					{
						stringstream ss;
						ss << flag;
						player->Close(PLAYER_CLOSE_OPERTE_APPD_ONE2,"");
					}
				}
			});
			return;
		}
		//没有取到，就new个
		CreateOtherBinlog(new_guid, flag, create_fun);
	} else {
		// 技能的话生成临时数据
		if (GuidManager::GetPrefix(new_guid) == ObjectTypeSpell) {
			string player_guid = guid();
			AppdContext *player = ObjMgr.FindPlayer(player_guid);
			if (player) {
				player->generateTempSpellLevelInfo(ObjMgr.Get(new_guid));
			}
		}
	}
}

void AppdContext::NewOtherBinlog(EObjectTypePrefix t, uint32 flag, std::function<BinLogObject*()> create_fun)
{
	string new_guid = GuidManager::ReplaceSuffix(guid(), t);
	NewOtherBinlog(new_guid, flag, create_fun);
}

//玩家登录做点什么
int AppdContext::DoPlayerLogin()
{
	LuaStackAutoPopup autoPopup(L);
	LuaGetObjFunc(L, GetGuid().c_str(), "Login");
	if(LUA_PCALL(L, 1, 0, 0))
	{
		tea_perror("lua error:DoPlayerLogin %s", GetGuid().c_str());
	}
	return 0;
}

//玩家下线做点什么
int AppdContext::DoPlayerLogout()
{

	LuaStackAutoPopup autoPopup(L);
	LuaGetObjFunc(L, GetGuid().c_str(), "Logout");
	if(LUA_PCALL(L, 1, 0, 0))
	{
		tea_perror("lua error:DoPlayerLogout %s", GetGuid().c_str());
	}
	return 0;
}


//战力改变
int AppdContext::OnForceChanged() {
	
	LuaGetObjFunc(L, this->GetGuid().c_str(), "OnForceChanged");
	if(LUA_PCALL(L, 1, 0, 0))
	{
		tea_perror("lua error:LuaOnForceChanged player guid: %s", this->GetGuid().c_str());
		return 0;
	}

	return 0;
}
//等级改变
int AppdContext::OnLevelChanged() {
	LuaGetObjFunc(L, this->GetGuid().c_str(), "OnLevelChanged");
	if(LUA_PCALL(L, 1, 0, 0))
	{
		tea_perror("lua error:LuaOnLevelChanged player guid: %s", this->GetGuid().c_str());
		return 0;
	}

	return 0;
}

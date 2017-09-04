#include "policed_mongodb.h"
#include <object/SharedDef.h>

PolicedMongoDB::PolicedMongoDB(log_holder_t *log):MongoDB(log)
{
	server_name_ = PolicedApp::g_app->GetConfig().server_id;
}

PolicedMongoDB::~PolicedMongoDB()
{
}

account_info *PolicedMongoDB::LoadAccountByUserName(const char *username)
{
	Results results;
	Map wheres;
	wheres["name"]=username;
	wheres["server_name"] = GetServerNameFromAccount(username);
	if(Query(GetServerNS("account"),wheres,results) == MONGO_RES_ERROR)
		ASSERT(0);
	account_info *info = (account_info *)malloc(sizeof(account_info));
	memset(info, 0, sizeof(account_info));
	for (auto result: results)
	{
		string sid, uid;
		fromString(sid, result["sid"]);
		fromString(uid, result["s_uid"]);
		strcpy(info->id, username);

		if(sid.length()>0)
			strncpy(info->sid, sid.c_str(), sizeof(info->sid));
		else
			strcpy(info->sid, "");
		if(uid.length()>0)
			strncpy(info->uid, uid.c_str(), sizeof(info->uid));
		else
			strcpy(info->uid, "");
	}
	return info;
}

account_info *PolicedMongoDB::LoadAccountByUid(const char *uid)
{
	account_info *info = NULL;
	Results results;
	Map wheres;
	wheres["s_uid"]=uid;
	wheres["server_name"] = server_name_;
	if(Query(GetServerNS("account"),wheres,results) == MONGO_RES_ERROR)
		ASSERT(0);
	for (auto result: results)
	{
		string sid, s_uid, username;
		fromString(sid, result["sid"]);
		fromString(s_uid, result["uid"]);
		fromString(username, result["s_username"]);

		account_info *info = (account_info *)malloc(sizeof(account_info));
		memset(info, 0, sizeof(info));

		if(sid.length()>0)
			strncpy(info->sid, sid.c_str(), sizeof(info->sid));
		else
			strcpy(info->sid, "");
		if(s_uid.length()>0)
			strncpy(info->uid, s_uid.c_str(), sizeof(info->uid));
		else
			strcpy(info->uid, "");
		strcpy(info->id , username.c_str());
	}
	return info;
}

//根据玩家GUID获取account
string PolicedMongoDB::GetAccountFromGuid(string player_id)
{
	Map wheres,result;
	wheres["guid"]=toString(player_id);
	wheres["server_name"] = GetServerName(player_id);
	if (!Load(GetPlayerInfo("chars", GetServerName(player_id)),wheres,result) == MONGO_RES_SUCCESS)
	{
		tea_perror("GetAccountFromGuid id[%u] error!",player_id.c_str());	
	}
	return result["account"];
}

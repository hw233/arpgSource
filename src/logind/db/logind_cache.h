#ifndef _LOGIND_CACHE_H_
#define _LOGIND_CACHE_H_

#include "logind_app.h"
#include <svrcore/server/Storage.h>
#include <comm/common_stl.h>
#include <object/BinLogObject.h>
#include <object/GuidManager.h>


//登录服账号角色物品等数据的缓存
class LogindCache
{
	typedef map<string, string> CharNameMap;
	typedef map<string, account_table*> AccountMap;
public:
	LogindCache();
	virtual ~LogindCache();
public:
	//获取账号信息
	account_table *GetAccount(const string &account);
	//增加一个账号信息
	void AddAccount(account_table *info);
	//获取账号下角色列表
	void GetCharList(const string &account,vector<char_create_info>&);
	//通过玩家名字获取guid
	const string &FindGuidByCharName(const string &name);
	//添加一条玩家名字
	void AddCharName(const string &guid, const string &name);
	//读取所有的角色名
	void LoadAllCharName();
	
	//保存活动帐号与角色的对应表
	void AddAccountToChar(const string& account,char_create_info& info);
	void AddAccountToChar(const string& account,string guid);

	//保存合服标志文件
	void SaveFileToMergeServer(const string& file_name, const string& now_time);

	//释放登出的玩家缓存
	void FreeLogoutPlayer(bool immediately = false);
	//添加一个登出玩家缓存
	void AddLogoutPlayer(const string &guid);
	//删除一个登出玩家缓存
	void DelLogoutPlayer(const string &guid);
public:
	//直接保存数据到硬盘,覆盖文件
	bool SaveData(const string &file_name, const string &content);
	//通过玩家guid获取系列化字符串
	string GetPlayerDataStr(const string &guid);
	//玩家自动保存心跳
	void Update(logind_player *player);
	//读取玩家对象
	bool LoadPlayer(const string &guid, logind_player **player, vector<GuidObject*> &vec);
	//保存玩家对象
	void SavePlayerData(const string &guid);
	//保存玩家对象
	void SavePlayerData(const string &guid, vector<GuidObject*> &vec, uint32 level);
	//备份玩家数据
	void Backup(const string &gm_path = "");
	//保存有变化的玩家到数据库
	void SaveChangePlayerToDB(bool is_merge = false);
	//从本地硬盘把所有账号信息读出来
	void LoadAllAccountInfo();
	//保存账号信息到硬盘中
	void SaveAccountInfo(account_table *info);
	//从本地硬盘把所有账号角色信息读出来
	void LoadAllAccountCharInfo();
	//保存账号角色信息到硬盘中
	void SaveAccountCharInfo(const string& account,const string& guid);
	//回档指定玩家
	bool HuidangPlayerInfos(const string player_guid, const string backupfilename);
	//读取世界变量对象
	bool LoadGlobalValue();
	//读取帮派变量对象
	bool LoadFractionValue();
	bool LoadFractionData();
protected:
	//读取对象集合,记得释放内存喔亲
	bool LoadDataSet(const string &guid, vector<GuidObject*> &vec);
	//把玩家数据序列化成字符串
	string PlayerDataToString(const string &guid, vector<GuidObject*> &vec);
private:
	core_obj::Storage			*m_storage;				//本地硬盘缓存
	CharNameMap					m_charNameMaps;			//这里存的名称全部是小写加半角,搜索的时候记得也要小写加半角
	AccountMap					m_accountMaps;			//所有账号
	std::map<string,char_create_info>		m_charListHD;			//本地硬盘缓存--角色列表

	std::map<string,time_t>		m_logoutChar;			//已经登出的缓存玩家信息
};

#endif



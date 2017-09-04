#ifndef _HTTP_HANLDER_H
#define _HTTP_HANLDER_H

#include "logind_app.h"

namespace http{
namespace client{class client;}
namespace server{ class server; struct reply; struct request;}
}

class HttpHandler
{
public:
	typedef http::server::request Request;
	typedef  http::server::reply Reply;
	typedef map<string, string> ServerNameHostPortMap;
	typedef void (HttpHandler::*Handler)(const Request& req, Reply& rep);

	HttpHandler();
	~HttpHandler();

	void Init();

	//发送本地监听web接口给远程服务器
	void RegisterWeb(bool force = false);
	void ReloadWeb();						//主动要求web重新下发本服关心的数据
	void AddHandler(string uri, Handler handler);
	string MakeResponse(int ret, string msg);

	void Update();

	//G2G 推送数据到主服
	bool PostMergeData(const string &guid, const string &to_server_name);
	//G2G 推送合服完成到主服
	bool PostMergeOK(const string &to_server_name);

	//G2W 从远程Web取得配置信息
	string GetWebConfig(const string& key,const string& defaultValue = "");

	void Handle_Query_Char_Info(const Request& req, Reply& rep);		//查询玩家角色信息
	void Handle_Merge_Postdata(const Request& req, Reply& rep);			//合服推送玩家数据
	void Handle_Merge_Ok(const Request& req, Reply& rep);				//合服完毕
	void Handle_SetGmlevel(const Request& req, Reply& rep);				//设置gm等级
	void Handle_Kicking(const Request& req, Reply& rep);				//踢人
	void Handle_Gag(const Request& req, Reply& rep);					//禁言、解禁
	void Handle_QQ_Config(const Request& req, Reply& rep);				//设置gmQQ配置
	void Handle_Lock_Account(const Request& req, Reply& rep);			//封号

public:
	void GetHostPortByServerName(const string& server_name, string& host, string& port);		//获得服务器的ip/port
	void RegBattleSvrConn();																	//向web注册战斗服的状态
protected:
	void LoadPlayer(const string &player_guid, std::function<void(bool,const string &,vector<GuidObject*>&)> fun);
	string GetLogindPostUrl(const string &server_name);
private:
	http::server::server *m_server;
	http::client::client *m_client;

	//注册状态, 0:初始化 1:准备注册 2:注册成功 -1:不需要注册
	int m_reg_state;
	//重load次数
	int m_reload_count;
	ServerNameHostPortMap m_serverHostportInfo;	//服务器的ip/port缓存
};


#endif

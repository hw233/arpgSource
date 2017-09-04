#include "mongo_db.h"
#include <comm/log.h>
#include <comm/str_util.h>
#include <comm/b64.h>
#include <comm/crypt.h>

#include "worker_pool.h"
#include <mutex>

//////////////////////////////////////////////////////////////////////////
//for MongoDB
MongoDB::MongoDB(log_holder *log):log_(log),async_db_(nullptr)
{
}

MongoDB::~MongoDB()
{
	if(async_db_)
	{	
		delete async_db_;
		async_db_ = nullptr;
	}
}

bool MongoDB::DecodeConnectionString(string conn_str,string& host,string& port,string& user, string& passwd, string &db_prefix, log_holder_t *logger/* = nullptr*/)
{
	Tokens	tokensCharacter;
	//先尝试不解密看能否解得开，解不开再调用解密 
	StrSplit(tokensCharacter, conn_str, ';');
	if(tokensCharacter.size() < 4)
	{
		//开始解密
		char db_conn_str[1024];
		memset(db_conn_str, 0, sizeof(db_conn_str));
		if(base64_decode(db_conn_str,sizeof(db_conn_str),conn_str.c_str(),conn_str.size()) != 0)
		{
			if(logger)
				out_error(logger,"open mongodb base64_decode fail!");
			return false;
		}
		tea_v *p_tv = (tea_v*)(db_conn_str + sizeof(int));
		int str_size = *(int*)db_conn_str;
		str_size = str_size >> 3;
		tea_k tk_dec;
		tk_dec.k[0] = 78237478;
		tk_dec.k[1] = 9978445;
		tk_dec.k[2] = 45344231;
		tk_dec.k[3] = 78943894;
		for (int i = 0; i < str_size; i++)
			tean((TEAint32*)&tk_dec, (TEAint32*)(p_tv+i), -16);
		//out_str(log_,"open mongodb conn:[%s]",db_conn_str + sizeof(int));
		//解密完成
		StrSplit(tokensCharacter, db_conn_str + sizeof(int), ';');

		//解密后连接字符串还是有误
		if(tokensCharacter.size() < 4)
			return false;
	}
	host = tokensCharacter[0];
	port = tokensCharacter[1];
	user = tokensCharacter[2];
	passwd = tokensCharacter[3];
	if (tokensCharacter.size() >= 6)
		db_prefix = tokensCharacter[5]; 

	return true;
}


bool MongoDB::Open(string conn_str,int log_level, string log_path,int worker_num)
{
	string port;
	if(!DecodeConnectionString(conn_str,db_host_,port,db_user_,db_passwd_,db_prefix_,log_))
	{
		out_error(log_,"decode connection string faild!");
		return false;
	}
	db_port_ = atoi(port.c_str());
	if(!MongoWrap::Open(db_host_, db_port_, db_user_,db_passwd_, "admin",log_level,log_path))
	{
		out_error(log_,"open mongodb conn faild!");
		return false;
	}

	//在此构造异步的数据库操作
	//权限验证只要在admin库完成即可
	if(worker_num)
	{
		/*auto *async_conn = new MongoWrap();
		if(!async_conn->Open(db_host_, db_port_, db_user_,db_passwd_, "admin",log_level,log_path))
		{
			out_error(log_,"open async mongodb conn faild!");
			return false;
		}*/

		async_db_ = new AsyncMongoDB(this);	
		if(!async_db_->Start())		//只允许支持一个异步线程
		{
			out_error(log_,"open async mongodb conn faild!");
			return false;
		}
	}
	
	return true;
}

string MongoDB::GetServerName(string char_guid)
{
	size_t pos = char_guid.rfind('.');
	if(pos == string::npos)
		return "";

	//得到帐号了
	char_guid = char_guid.substr(pos+1,char_guid.length()-pos);

	return char_guid;
}

string MongoDB::GetServerNameFromCharName(string name)
{
	string server_name = "";
	//倒数第一个,号的后面存储着玩家真正的名称
	size_t pos = name.find(',');
	if(pos == string::npos)
		return "";
	//第一个逗号分割着pid
	server_name += name.substr(0,pos);	
	server_name += '_';

	//第二个逗号存着sid
	size_t pos2 = name.find(',',pos+1);//跳过,
	if(pos2 == string::npos)
		return "";
	
	server_name += name.substr(pos+1,pos2-pos-1);
	return server_name;
}

string MongoDB::GetServerNameFromAccount(string account)
{
	string server_name = "";
	size_t pos = account.find('_');
	if(pos == string::npos)
		return "";
	server_name = account.substr(0,pos);
	server_name += "_";
	//第二个逗号存着sid
	size_t pos2 = account.find('_',pos+1);//跳过,
	if(pos2 == string::npos)
		return "";
	server_name += account.substr(pos+1,pos2-pos-1);
	return server_name;
}

string MongoDB::GetLastNameFromCharName(string name)
{
	size_t pos = name.rfind(',');
	if(pos == string::npos)
		return name;
	return name.substr(pos+1,name.length());
}

void MongoDB::AsyncUpdate(string ns,const MongoDB::Map& where,MongoDB::Map& values)
{
	if(async_db_)
		return async_db_->AsyncUpdate(ns,where,values);
	else
		Update(ns,where,values);
}

void MongoDB::AsyncInsert(string ns,MongoDB::Map& values)
{
	if(async_db_)
		return async_db_->AsyncInsert(ns,values);
	else
		Insert(ns,values);
}

void MongoDB::AsyncUpsert(string ns,const MongoDB::Map& where,MongoDB::Map& values)
{
	if(async_db_)
		return async_db_->AsyncUpsert(ns,where,values);
	else
		Upsert(ns,where,values);
}

void MongoDB::AsyncQuery(string ns,const MongoDB::Map& where, AsyncMongoDBTask::CB cb,string orderKey, int32_t bySort,int32_t limitReslut)
{
	ASSERT(async_db_);
	async_db_->AsyncQuery(ns,where,cb,orderKey,bySort,limitReslut);
}

void MongoDB::AsyncLoad(string ns,const MongoDB::Map& where, AsyncMongoDBTask::CB cb)
{
	ASSERT(async_db_);
	async_db_->AsyncLoad(ns,where,cb);
}

Mongo_Result MongoDB::QueryPlayerInfo(string ns,string server_name,MongoDB::Map& where,Results& results)
{
	Mongo_Result result = Query(GetPlayerInfo(ns,server_name),where,results);
	//if(result == MONGO_RES_NULL)
	//{
	//	result = Query(GetCharNS(ns),where,results);
	//	if(result == MONGO_RES_SUCCESS)	//有数据，数据迁移下
	//	{
	//		for(auto values:results)
	//		{
	//			Insert(GetPlayerInfo(ns,server_name),values,true);
	//		}
	//	}
	//}
	return result;
}

Mongo_Result MongoDB:: QueryPlayerInfo(string ns,string server_name,MongoDB::Map& where,Results& results,MongoDB::Map& outfield)
{
	Mongo_Result result = Query(GetPlayerInfo(ns,server_name),where,results,outfield);
	//if(result == MONGO_RES_NULL)
	//{
	//	//outfield如果只有两个字段可能导致字段丢失
	//	result =  Query(GetCharNS(ns),where,results);
	//	if(result == MONGO_RES_SUCCESS)	//有数据，数据迁移下
	//	{
	//		for(auto values:results)
	//		{
	//			Insert(GetPlayerInfo(ns,server_name),values,true);
	//		}
	//	}
	//}
	return result;
}

Mongo_Result MongoDB:: QueryPlayerInfo(string ns,string server_name,MongoDB::Map& where,Results& results,string orderKey, int32_t bySort,int32_t limitReslut)
{
	Mongo_Result result = Query(GetPlayerInfo(ns,server_name),where,results,orderKey,bySort,limitReslut);
	//if(result == MONGO_RES_NULL)
	//{
	//	//TODO:能保证查询chars表的时候一定是单条记录吗？
	//	result =  Query(GetCharNS(ns),where,results,orderKey,bySort,limitReslut);
	//	if(result == MONGO_RES_SUCCESS)	//有数据，数据迁移下
	//	{
	//		for(auto values:results)
	//		{
	//			Insert(GetPlayerInfo(ns,server_name),values);
	//		}
	//	}
	//}
	return result;
}

Mongo_Result MongoDB::LoadPlayerInfo(string ns,string server_name,MongoDB::Map& where,MongoDB::Map& values)
{
	Mongo_Result result = Load(GetPlayerInfo(ns,server_name),where,values);
	//if(result == MONGO_RES_NULL)
	//{
	//	//从总库查询
	//	result = Load(GetCharNS(ns),where,values);
	//	if(result == MONGO_RES_SUCCESS)	//有数据，数据迁移下
	//	{
	//		Insert(GetPlayerInfo(ns,server_name),values,true);
	//	}
	//}
	return result;
}

void MongoDB::UpdateAsync()
{
	if(async_db_)
		async_db_->Update();
}

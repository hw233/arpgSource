
#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32

#include "db_wrap/mongo3_wrap.h"
#include <comm/common_stl.h>
//#include "mongo.h"
#include <sstream>

//static std::stringstream  tmp_ss;

#include "MogoWrapLogImpl.h"
#include <mongo/client/dbclient.h>

struct MongoDBPtr
{
	mongo::DBClientConnection *conn;
};

static void ToMap(const mongo::BSONObj& from,MongoWrap3::Map& to)
{
	std::set<string> names;
	from.getFieldNames(names);

	for (auto s:names)
	{
		auto ele = from.getField(s);
		auto type = ele.type();
		//由于可能被转成科学计数法,所以碰到u开头的，直接转换类型
		if(type == mongo::NumberDouble && s[0] == 'u')
			type = mongo::NumberLong;

		switch (type)
		{
		
		case mongo::NumberInt:
			to.insert(std::make_pair(s,toString(ele.numberInt())));
			break;
		case mongo::NumberDouble:
			to.insert(std::make_pair(s,toString(ele.numberDouble())));
			break;
		case mongo::NumberLong:
			to.insert(std::make_pair(s,toString(ele.numberLong())));
			break;
		case mongo::jstOID:
			to.insert(std::make_pair(s,toString(ele.OID())));
			break;
		case mongo::String:
			to.insert(std::make_pair(s,toString(ele.str())));
			break;
		case mongo::MinKey:
		//case mongo::EOO:
		case mongo::Object:
		case mongo::Array:
		case mongo::BinData:
		case mongo::Undefined:		
		case mongo::Bool:
		case mongo::Date:
		case mongo::jstNULL:
		case mongo::RegEx:
		case mongo::DBRef:
		case mongo::Code:
		case mongo::Symbol:
		case mongo::CodeWScope:
		case mongo::Timestamp:
			/*case mongo::JSTypeMax:
			break;*/
		case mongo::MaxKey:
			to.insert(std::make_pair(s,toString(ele.value())));
			break;
		default:			
			break;
		}		
	}
}

static void ToBson(const MongoWrap3::Map& m,mongo::BSONObjBuilder& to)
{
	string strQuery;
	for (auto it: m)
	{
		string keyStr = it.first;
		string valueStr = it.second;
		char markType = keyStr.at(0);
		switch (markType)
		{
		case 'u':		//TODO:这个准备64
		case 'i':
			{
				int nValue = 0;
				fromString(nValue, valueStr);
				to.append(keyStr,nValue);
				break;
			}
		default:
			to.append(keyStr,valueStr);
		}
		//to << keyStr << valueStr;
	}
}

//////////////////////////////////////////////////////////////////////////
//for MongoWrap3
MongoWrap3::MongoWrap3():port_(0)
{	
	mongo::client::initialize();
	mongo_ = new MongoDBPtr;
	mongo_->conn = new mongo::DBClientConnection(true);
	//conn_ = (mongo *)malloc(sizeof(mongo));
	//mongo_init(conn_);

	log_ = new MogoWrapLogImpl;
}

MongoWrap3::~MongoWrap3()
{
	if(mongo_->conn )
	{
		delete mongo_->conn ;
		mongo_->conn  = nullptr;
	}	

	if(mongo_)
	{
		delete mongo_;
		mongo_ = nullptr;
	}

	if(log_)
	{
		delete log_;
		log_ = nullptr;
	}
	
}

bool MongoWrap3::Open(string host,unsigned short port,string user,string pwd,string dbname,int level, string logpath)
{
	//打开日志

	host_ = host;
	port_ = port == 0 ? 27017 : port;
	dbname_ = dbname.empty()?"admin":dbname;
	user_ = user;
	password_ = pwd;

	std::stringstream ss; 
	ss << host_ << ":" << port_;

	try
	{
		mongo_->conn->connect(ss.str());
#if 0
		mongo_->conn->auth(BSON("user" << user <<
			"db" << dbname <<
			// "mechanism" << "MONGODB-CR" <<		//mongodb2的默认验证方式
			"mechanism" << "SCRAM-SHA-1" <<			//mongodb3.0的验证方式
			"pwd" << pwd));
#else
		if(user.empty() && password_.empty())
			return true;
		string errmsg;
		if(!mongo_->conn->auth("admin",user,password_,errmsg))
			return false;
#endif

	}
	catch (const mongo::DBException &e)
	{
		log_->DoLog(e.what(),0);
		return false;
	}
	
	uint32_t before=ms_time();	
	if(mongo_->conn->isFailed())
	{
		return false;
	}

	log_->Open(level,logpath);
	log_->WriteLog(_DEBUG_L,before,"Open();From="+dbname_+";host="+host+";port="+toString(port));
	
	return true;
}

bool MongoWrap3::Insert(string ns,Map& values,bool /*fsync*/)
{
	if(!CheckNS(ns)) return false;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	uint32_t before=ms_time();

	mongo::BSONObjBuilder b;
	ToBson(values,b);
	mongo_->conn->insert(ns,b.obj());

	//if(fsync)
	//	string mongo::DBClientWithCommands::getLastError();
	
	log_->WriteLog(_DEBUG_L,before,"Insert();From="+ns,values,"values");
	
	return true;
}


bool MongoWrap3::Upsert(string ns,const Map& where,Map& values)
{	
	return Update(ns, where, values, true);
}


//保存单条记录根据条件保存所给值
bool MongoWrap3::Update(string ns,const Map& where,Map& values,bool upsert,bool /*fsync*/)
{
	//if(!CheckNS(ns)) return MONGO_RES_ERROR;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	uint32_t before=ms_time();
	mongo::BSONObjBuilder bsonWhere,bsonValues;
	ToBson(where,bsonWhere);
	ToBson(values,bsonValues);
	mongo::BSONObjBuilder setBson;
	setBson.append("$set",bsonValues.obj());
	//DataTranslateMapToBson(values,bsonValues.obj()); 

	mongo_->conn->update(ns,bsonWhere.obj(),setBson.obj(),upsert,true);

	log_->WriteLog(_DEBUG_L,before,"Update();From="+ns,values,"values");
	return true;
}

bool MongoWrap3::Update(string ns,int _id,Map& values)
{
	//if(!CheckNS(ns)) return MONGO_RES_ERROR;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	//uint32_t before=ms_time();
	mongo::BSONObjBuilder bsonWhere,bsonValues;
	ToBson(values,bsonValues);

	bsonWhere.append("_id",_id);	
	bool ret = false;

	mongo_->conn->update(ns,bsonWhere.obj(),
		mongo::BSONObjBuilder().append("$set",bsonValues.obj()).obj(),false,true);

	//log_->WriteLog(_DEBUG_L,before,"Update();From="+ns,where,"where",values,"values");
	return ret;
}

Mongo_Result MongoWrap3::Load( string ns,const MongoWrap3::Map& where,Map& result )
{
	if(!CheckNS(ns)) return MONGO_RES_ERROR;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	mongo::BSONObjBuilder b;
	result.clear();

	uint32_t before=ms_time();
	ToBson(where,b);
	//ns = dbname_+ns;
	Mongo_Result ret = MONGO_RES_SUCCESS;

	auto cursor = mongo_->conn->query(ns, b.obj());
	if (cursor->more())
	{
		mongo::BSONObj p = cursor->next();
		ToMap(p,result);
		ret = MONGO_RES_SUCCESS;
	}
	else
	{
		ret = MONGO_RES_NULL;
	}
	
	log_->WriteLog(_DEBUG_L,before,"Load();From="+ns,where,"where");

 	return ret;
}

Mongo_Result MongoWrap3::Query(string ns,const Map& where,Results& results)
{
	if(!CheckNS(ns)) return MONGO_RES_ERROR;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	mongo::BSONObjBuilder b;
	results.clear();

	uint32_t before=ms_time();
	ToBson(where,b);
	//ns = dbname_+ns;
	Mongo_Result ret = MONGO_RES_NULL;

	auto cursor = mongo_->conn->query(ns, b.obj());
	while (cursor->more())
	{
		mongo::BSONObj p = cursor->next();
		Map m;
		ToMap(p,m);
		results.push_back(m);
	}
	log_->WriteLog(_DEBUG_L,before,"Query();From="+ns,where,"where");
	return results.empty() ? MONGO_RES_NULL: MONGO_RES_SUCCESS;
}

Mongo_Result MongoWrap3::Query(string ns,const Map& where,Results& results,const Map& outfield)
{
	if(!CheckNS(ns)) return MONGO_RES_ERROR;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	mongo::BSONObjBuilder b;
	results.clear();

	uint32_t before=ms_time();
	ToBson(where,b);

	mongo::BSONObjBuilder bout;
	ToBson(outfield,bout);
	auto outfields = bout.obj();

	//ns = dbname_+ns;
	Mongo_Result ret = MONGO_RES_NULL;
	auto cursor = mongo_->conn->query(ns, mongo::Query(b.obj()),0,0,&outfields);
	while (cursor->more())
	{
		mongo::BSONObj p = cursor->next();
		Map m;
		ToMap(p,m);
		results.push_back(m);
	}
	log_->WriteLog(_DEBUG_L,before,"Query();From="+ns,where,"where");
	return results.empty() ? MONGO_RES_NULL: MONGO_RES_SUCCESS;
}

Mongo_Result MongoWrap3::Query( string ns,const Map& where,Results& results,string orderKey, int32_t bySort,int32_t limitResult )
{
	if(!CheckNS(ns)) return MONGO_RES_ERROR;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁
	
	mongo::BSONObjBuilder b;
	results.clear();

	uint32_t before=ms_time();
	ToBson(where,b);

	mongo::BSONObjBuilder whereBson;
	whereBson.append("$query",b.obj());


	if(!orderKey.empty())
	{
		mongo::BSONObjBuilder orderByBason;
		orderByBason.append(orderKey,bySort);
		whereBson.append("$orderby",orderByBason.obj());
	}

	//ns = dbname_+ns;
	Mongo_Result ret = MONGO_RES_NULL;

	auto cursor = mongo_->conn->query(ns, whereBson.obj(),limitResult);
	while (cursor->more())
	{
		mongo::BSONObj p = cursor->next();
		Map m;
		ToMap(p,m);
		results.push_back(m);
	}
	log_->WriteLog(_DEBUG_L,before,"Query();From="+ns,where,"where");
	return results.empty() ? MONGO_RES_NULL: MONGO_RES_SUCCESS;
}

Mongo_Result MongoWrap3::Query(string ns,const Map& where,Results& results,bool bGrate,string compareKey, uint32_t uValue)
{
	if(!CheckNS(ns)) return MONGO_RES_ERROR;				//验证库表命名是否合法
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	uint32_t before=ms_time();
	mongo::BSONObjBuilder bsonWhere;

	bsonWhere.append(compareKey,mongo::BSONObjBuilder().
		append(bGrate ? "$gte" : "$lt",uValue).obj());


	auto cursor = mongo_->conn->query(ns, bsonWhere.obj());
	while (cursor->more())
	{
		mongo::BSONObj p = cursor->next();
		Map m;
		ToMap(p,m);
		results.push_back(m);
	}

	log_->WriteLog(_DEBUG_L,before,"Query();From="+ns,where,"where",
		bGrate? "compareKey[>]" : "compareKey[<]","compareKey",uValue);

	return results.empty() ? MONGO_RES_NULL: MONGO_RES_SUCCESS;
}

bool MongoWrap3::Delete(string ns,const Map& where)
{
	std::unique_lock<std::mutex> lock(mutex_);				//上锁

	uint32_t before=ms_time();
	mongo::BSONObjBuilder bsonValues,bsonWhere;
	ToBson(where,bsonWhere);
	
	mongo_->conn->remove(ns,mongo::Query(bsonWhere.obj()));

	log_->WriteLog(_DEBUG_L,before,"Delete();From="+ns,where,"where");
	return true;
}

bool MongoWrap3::CheckNS(const string& ns)
{
	//里面带|线'\'前后去掉不可见字符	
	for (auto it = ns.begin(); it != ns.end(); ++it)
	{
		char c = *it;
		if( c >= 'a' && c <= 'z')		//97-122
			continue;
		if (c == '_' || c == '.')		//95, 46
			continue;		
		if( c >= '0' && c <='9')		//48-57
			continue;		
		if(c >= 'A' && c <= 'Z')		//65-90
			continue;

		//只允许_线及.以前英文及数字
		return false;
	}	
	
	return true;
}

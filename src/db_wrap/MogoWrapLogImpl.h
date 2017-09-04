#include <string>
#include <sstream>
#include <stdint.h>
#include "db_wrap/mongo_wrap_def.h"

using std::string;

template<typename T>
void fromString(T& t,string v)
{
	std::stringstream ss;
	ss << v;
	ss >> t;
}

template<typename T>
string toString(T t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}
//////////////////////////////////////////////////////////////////////////
//for MogoWrapLogImpl
struct MogoWrapLogImpl
{
	MogoWrapLogImpl()
	{
		log_level = _NO_LOG;
	}
	~MogoWrapLogImpl()
	{
		outLog_.close();
	}

	void Open(int level, string logpath)
	{
		if (logpath.empty())
			return;

		outLog_.open(logpath, std::ios::app);//独占打开
		if (outLog_.is_open())
		{
			log_level=(Log_Level)level;
		}
		else
		{
			//异步进程，修改文件名
			string asyPath = "";
			auto pos = logpath.find(".log");
			if (pos == string::npos)
				asyPath = logpath+"_asy";
			else
				asyPath = logpath.insert(pos, "_asy");

			outLog_.open(asyPath, std::ios::app);
			if (outLog_.is_open())
			{
				log_level=(Log_Level)level;
			}
		}
	}

	void DoLog(string msg, uint32_t before);
	void WriteLog(int lg_lv, uint32_t before,string msg )
	{
		if(lg_lv < log_level)
			return;
		DoLog(msg,before);
	}
	void WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data, string sDataKey );
	void WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data, string sDataKey, string sKey,string strKey );
	void WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data, string sDataKey, const std::map<string,string>& data2,string sDataKey2 );
	void WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data,string  sDataKey, string sKey,string strKey,int32_t value);

	Log_Level log_level;
	std::ofstream outLog_;//日志文件流
};


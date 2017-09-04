#include <comm/common_stl.h>
#include "MogoWrapLogImpl.h"

void MogoWrapLogImpl::DoLog(string msg, uint32_t before )
{
	time_t   t; 
	tm   *tp; 
	t=time(NULL); 
	tp=localtime(&t); 
	char str[50];
	memset(str, 0, sizeof(str));
	sprintf(str, "%d/%d/%d/%d/%d/%d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
	outLog_<<str<<";"<<get_time_diff(before,ms_time())<<";"<<msg<<std::endl;
}

void MogoWrapLogImpl::WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data, string sDataKey )
{
	if(lg_lv != log_level)
		return;
	string msg;
	msg+=title+";"+sDataKey+":";
	for (auto it: data)
	{
		msg+=it.first+"="+it.second+"?";
	}
	msg=msg.substr(0,msg.length()-1);
	DoLog(msg,before);
}

void MogoWrapLogImpl::WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data,string sDataKey, string sKey,string strKey )
{
	if(lg_lv != log_level)
		return;
	string msg;
	msg+=title+";"+sDataKey+":";
	for (auto it: data)
	{
		msg+=it.first+"="+it.second+"?";
	}
	msg = msg.substr(0,msg.length()-1);
	msg +=";";
	msg += strKey+":"+sKey;
	DoLog(msg,before);
}

void MogoWrapLogImpl::WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data,string sDataKey, string sKey,string strKey,int32_t value )
{
	if(lg_lv != log_level)
		return;
	string msg;
	msg+=title+";"+sDataKey+":";
	for (auto it: data)
	{
		msg+=it.first+"="+it.second+"?";
	}
	msg=msg.substr(0,msg.length()-1);
	msg+=";"+strKey+"="+sKey+";value="+toString(value);
	DoLog(msg,before);
}

void MogoWrapLogImpl::WriteLog(int lg_lv, uint32_t before,string title,const std::map<string,string>& data, string sDataKey, const std::map<string,string>& data2,string sDataKey2 )
{
	if(lg_lv != log_level)
		return;
	string msg;
	msg+=title+";"+sDataKey+":";
	for (auto it: data)
	{
		msg+=it.first+"="+it.second+"?";
	}
	msg=msg.substr(0,msg.length()-1);

	msg+=";"+sDataKey2+":";
	for (auto it: data2)
	{
		msg+=it.first+"="+it.second+"?";
	}	
	msg=msg.substr(0,msg.length()-1);

	DoLog(msg,before);
}


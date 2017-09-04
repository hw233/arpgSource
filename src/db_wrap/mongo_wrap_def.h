#ifndef _MONGO_WRAP_DEF_H_
#define _MONGO_WRAP_DEF_H_

#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <fstream>
#include <functional>

using std::string;

enum Log_Level
{
	_NO_LOG=-1,
	_DEBUG_L=0, //调试级别
	_ERROR_L    //错误级别
};

enum Mongo_Result
{
	MONGO_RES_ERROR=-1,  //错误
	MONGO_RES_SUCCESS=0,//成功
	MONGO_RES_NULL    //返回空
};
//
//typedef std::map<string,string> Map;
//typedef std::vector<uint32_t> Values;
//typedef std::vector<string> StrValues;
//typedef std::vector<Map> Results;


#endif

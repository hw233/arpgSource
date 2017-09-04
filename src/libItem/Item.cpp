#include "Item.h"


#include "comm/str_util.h"
#include <iostream>
#include <sstream>
#include <comm/b64.h>

#include <minijson/minijson_writer.hpp>
#include <minijson/minijson_reader.hpp>

#define GET_BIT(v,i) (((v)>>(i))&0x1)
#define SET_BIT(v,i) ((1<<(i))|v)
#define UNSET_BIT(v,i) (~(1<<(i))&v)


static string toJson (const std::map<string,double>& m) 
{
	if(m.empty())
		return "{}";

	std::stringstream ss;
	minijson::object_writer writer(ss);
	for (auto kv: m)	
		writer.write(kv.first.c_str(),kv.second);
	writer.close();
	return ss.str();
}

static void addAttStr(std::stringstream &ss,const std::vector<ItemBaseAtt> &ary){
	ss<<";";
	for (unsigned int i=0;i < ary.size();i++)
	{
		ss<<ary[i].key<<","<<ary[i].val<<","<<ary[i].qua;
		if (i < (ary.size()-1))
		{
			ss<<",";
		}
	}
}
static void getAttStr(string &str,std::vector<ItemBaseAtt> &ary){
	if(str.size() == 0){
		return;
	}
	Tokens tokens;
	StrSplit(tokens, str, ',');
	int len = tokens.size();
	for (int i=0;i<len;i+=3)
	{
		int key =atoi(tokens[i].c_str());
		int val =atoi(tokens[i+1].c_str());
		int qua =atoi(tokens[i+2].c_str());
		ItemBaseAtt temp;
		temp.key = key;
		temp.val = val;
		temp.qua = qua;
		ary.push_back(temp);
	}

}

static bool fromJson(const string& str_json,std::map<string,double>& m)
{
	m.clear();
	try
	{
		minijson::const_buffer_context ctx(str_json.c_str(), str_json.size()); // may throw
		minijson::parse_object(ctx,[&](const char* k,minijson::value v){		
			m[k] = v.as_double();
		});
	}
	catch (std::exception&)
	{
		//tea_perror("fromJson :%s",e.what());
		return false;
	}	
	return true;
}


Item::Item():id_(0),entry_(0),count_(0),flags_(0)
{
}

Item::~Item()
{
}

//设置标志位
bool Item::GetBit(int pos)
{
	return GET_BIT(flags_,pos);
}
void Item::SetBit(int pos)
{
	flags_ = SET_BIT(flags_,pos);	
}
void Item::UnSetBit(int pos)
{
	flags_ = UNSET_BIT(flags_,pos);
}

//设置属性
void Item::SetAttr(const string& key,double values)
{
	attrs_[key] = values;
}

double Item::GetAttr(string key)
{
	if(attrs_.find(key)==attrs_.end())
		return 0;
	return attrs_[key];
}

void Item::SetBaseAttr(unsigned int pos,int key,double val,int qua)
{
	if (pos < itemBaseAtt_.size())
	{
		itemBaseAtt_[pos].key = key;
		itemBaseAtt_[pos].val = val;
		itemBaseAtt_[pos].qua = qua;
	}
}

void Item::AddBaseAttr(int key,double val,int qua)
{
	ItemBaseAtt temp;
	temp.key = key;
	temp.val = val;
	temp.qua = qua;
	itemBaseAtt_.push_back(temp);
}

void Item::ForEachBaseAttr(std::function<void (int, double)> func)
{
	for (unsigned int i=0;i<itemBaseAtt_.size();i++)
	{
		func(itemBaseAtt_[i].key,itemBaseAtt_[i].val);
	}
}

bool Item::FromString(const string& s)
{
	Tokens tokens;
	StrSplit(tokens, s, ';');
	if(tokens.size()!=kStringMember)
		return false;
	//物品ID
	id_=atoi(tokens[0].c_str());
	//模板ID
	entry_=atoi(tokens[1].c_str());
	//叠加数
	count_=atoi(tokens[2].c_str());
	//标志位
	flags_=atoi(tokens[3].c_str());
	//所有的特殊属性使用键值对的方式往里扔	
	attrs_.clear();
	if(!fromJson(tokens[4],attrs_))
		return false;
	itemBaseAtt_.clear();
	if (tokens.size() >= 6)
	{
		getAttStr(tokens[5],itemBaseAtt_);
	}
	return true;
}

string Item::ToString() const
{
	std::stringstream ss;
	string s = toJson(attrs_);
	//id_;entry_;count;flag;{k1:v1,k2:v2}
	ss<<id_<<";"<<entry_<<";"<<count_<<";"<<flags_<<";"<<s;

	addAttStr(ss,itemBaseAtt_);
	
	return ss.str();
}




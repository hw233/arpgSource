#ifndef _ITEM_ITEM_H_
#define _ITEM_ITEM_H_

#include <string>
#include <map>
#include <stdint.h>

using std::string;
#include <functional>
//以 id;entry;count;flag;{k1:v1,k2:v2}
const int kStringMember = 6;

#include <vector>
//物品类

struct ItemBaseAtt
{
	int key;
	double val;
	int qua;
};

class Item
{
public:
	Item();
	virtual ~Item();

	bool FromString(const string& s);
	string ToString() const;

	int GetId() const{return id_;}
	void SetId(int value){id_ = value;}

	int GetEntry(){return entry_;}
	void SetEntry(int value){entry_ = value;}

	int GetCount(){return count_;}
	void SetCount(int value){count_=value;}

	bool GetBit(int pos);
	void SetBit(int pos);
	void UnSetBit(int pos);

	void SetAttr(const string& key,double values );
	double GetAttr(string key);
	void ClearAttr()
	{//清空属性对
		attrs_.clear();
	}
	//设置基础属性
	void SetBaseAttr(unsigned int pos,int key,double val,int qua);
	//添加基础属性
	void AddBaseAttr(int key,double val,int qua);
	//清空基础属性
	void ClearBaseAttr()
	{
		itemBaseAtt_.clear();
	}

	//遍历属性
	void ForEachBaseAttr(std::function<void (int, double)> func);

	void SetGuid(string guid){guid_ = guid;}
	string GetGuid(){return guid_;}
private:

	int id_;
	//模板ID
	int entry_;
	//叠加数
	int count_;
	//标志位
	int flags_;
	//所有的特殊属性使用键值对的方式往里扔
	std::map<string,double> attrs_;
	//基础属性列表
	std::vector<ItemBaseAtt> itemBaseAtt_;
	string guid_;	//物品guid "item_guid;id_"
};

#endif

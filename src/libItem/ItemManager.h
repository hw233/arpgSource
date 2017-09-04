#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

//1 所有的物品都放在一个大的binlog里面。这个binlog叫做物品管理者
//2 每一个物品在binlog里面占用一个int下标和一个string下标
//3 物品的int下标存一个位置信息比如102 
//	这个102最终会通过接口转换成类似 包裹0（普通背包）位置13这样的一个二维位置信息。主要用来给客户端显示用以及
//4 string下标存物品的所有信息
//5.会有另外一个item类来最终转换和操作第四条里面的string。

#include <core_obj/GuidObject.h>

using core_obj::GuidObject;

class Item;

//binlog的owner下标索引,ps:这里不能应用SharedDef.h头文件,因为这是独立的项目
const int owner_index = 3;
//物品guid前缀
const char item_guid_prefix = 'I';

//每个包裹最大格子数量
const int kMaxBagSize = 1000;
const int kItemPosStart = 128;

//最大ID跳号的下标位置
const int kItemIDJumpIndex = 8;
//下标9~23 每个16位用来存储当前包裹的业务大小
const int kBagSizeIndex = 9;
//最大包裹数量
const int kBagCount = 30;

class ItemManager
{
public:
	ItemManager();
	virtual ~ItemManager();
	void Init(GuidObject *obj);

	//根据位置找物品,找不到返回空
	Item *GetByPos(int bag_type,int pos);
	//根据物品ID找物品
	Item *Get(int id);
	//根据物品guid找物品
	Item *GetItemByGuid(const string guid);

	//插入一个物品对象,返回物品ID
	int Add(const Item& item,int bag,int pos=-1);
	//删除物品,返回是否成功
	bool Del(int id);
	//扣除指定模板的物品 TODO绑定这块在哪处理好呢 另外起接口比较合理。。
	int Sub(int entry, int count,int bag);

	//保存物品,注意
	void Save(const Item& item, int bag, int pos);
	//保存物品,这是不拷贝保存
	void SavePtr(const Item* item);
	//移动物品
	void Move(int id,  int to_bag, int to_pos);	
	

	//统计指定物品数量
	int CountEntey(int entry,int bag);
	//背包剩余空位
	int GetEmptyCount(int bag);
	//包裹实际大小
	int GetBagSize(int bag);
	void SetBagSize(int bag,int size);

	//从binlog内部获得一个新的ID
	int GetNewID();

	//遍历指定包裹的所有物品
	void ForEachBagItem(int bag, std::function<bool (Item*, int)> func);
	//获得物品管理器guid
	const string& GetGuid();
	//从包裹内部找空位
	int GetEmptyPos(int bag);
protected:
	int FindBinlogEmptyPos();	
	//二维转一维一个换算
	int TransformPos(int bag,int pos){return bag*kMaxBagSize +pos;}

	//通过位置信息 找binlog的下标位置
	int GetBinlogIndexBypos(int true_pos);
	//负责验证包裹和位置的合法性 先直接断言
    int CheckBagAndPos(int bag,int pos);

private:
	GuidObject *obj_;
	//用来遍历
	std::map<int,Item> itemsMap_;
	//当前自增id,防止一次性加多个物品时下标被中心服务同步回来给盖掉了
	uint32_t m_cur_max_id;
};


#endif

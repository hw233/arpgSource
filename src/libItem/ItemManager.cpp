#include "ItemManager.h"
#include "Item.h"

ItemManager::ItemManager():m_cur_max_id(0),obj_(nullptr)
{	
}

ItemManager::~ItemManager()
{		
}


void ItemManager::Init(GuidObject *obj)
{
	string owner_guid = obj->GetStr(owner_index);
	obj_ = obj;
	//初始化物品数组 从下标弄出来到map
	for (uint32_t i=kItemPosStart;i<obj_->length_uint32();++i)
	{
		string s = obj_->GetStr(i);
		if(s!="")
		{
			Item item;
			//TODO 考虑from不出来的处理 日志之类
			if(item.FromString(s))
			{
				//构造物品唯一guid
				char temp[50] = {0};
				sprintf(temp, "%c%s;%d", item_guid_prefix, owner_guid.substr(1).c_str(), item.GetId());
				item.SetGuid(temp);
				//map维护点0
				itemsMap_[i] = item;
			}
				
		}
	}
}

//位置越界检查 
int ItemManager::CheckBagAndPos(int bag,int pos)
{
	//TODO 此处可有日志
	if(pos>=kMaxBagSize)
		return -1;
	if(bag>=kBagCount)
		return -2;
	if(pos>=GetBagSize(bag))
		return -3;
	return 0;
}

//根据位置找物品,找不到返回空
Item* ItemManager::GetByPos(int bag_type,int pos)
{	
	if(CheckBagAndPos(bag_type,pos))
		return NULL;
	//在下标里面存的真实位置信息
	int true_pos =  TransformPos(bag_type,pos);
	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++iter)
	{
		if (true_pos == obj_->GetUInt32(iter->first))
		{
			return &iter->second;
		}
	}
	
	return NULL;
}

//根据物品ID找物品
Item* ItemManager::Get(int id)
{
	//纯遍历找ID
	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++iter)
	{
		if (iter->second.GetId() == id)
			return &iter->second;
	}

	return NULL;
}

//根据物品guid找物品
Item *ItemManager::GetItemByGuid(const string guid)
{
	//纯遍历找ID
	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++iter)
	{
		if (iter->second.GetGuid() == guid)
			return &iter->second;
	}

	return NULL;
}


//插入一个物品对象,返回物品ID
int ItemManager::Add(const Item& item,int bag,int pos)
{
	//这里其实是检查包裹
	if(CheckBagAndPos(bag,pos))
		return -3;
	//TODO  错误枚举还是要搞一个
	//非指定位置 找空位
	if(pos == -1)
	{
		pos = GetEmptyPos(bag);
		//找不到空位 拜拜
		if(pos==-1)
			return -1;
	}
	else
	{
		//TODO业务上的位置大小 是否需要在这边判断?
		//设计最大包裹大小
		//这个位置并不是空的
		Item * _item = GetByPos(bag,pos);
		if(_item)
			return -2;
	}
	int binlog_pos = FindBinlogEmptyPos();
	//map维护点
	auto iter = itemsMap_.insert(std::make_pair(binlog_pos, item));
	int id = GetNewID();
	iter.first->second.SetId(id);

	string owner_guid = obj_->GetStr(owner_index);
	//构造物品唯一guid
	char temp[50] = {0};
	sprintf(temp, "%c%s;%d", item_guid_prefix, owner_guid.substr(1).c_str(), id);	
	iter.first->second.SetGuid(temp);

	string s = iter.first->second.ToString();
	int true_pos =  TransformPos(bag,pos);
	
	//存进去
	obj_->SetUInt32(binlog_pos,true_pos);
	obj_->SetStr(binlog_pos,s);
	
	// 原来的是id
	return pos;
}

//删除物品,返回是否成功
bool ItemManager::Del(int id)
{
	//纯遍历找ID
	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++iter)
	{
		if (iter->second.GetId() == id)
		{
			//下标肯定要删除的嘛
			obj_->SetUInt32(iter->first,0);
			obj_->SetStr(iter->first,"");

			//map要跟着删除//map维护点2
			itemsMap_.erase(iter);
			return true;
		}
	}

	return false;
}

void ItemManager::Save(const Item& item, int bag, int pos )
{
	//TODO  检查下这个位置有没其它物品 考虑做个断言
	//业务上的位置大小 在此只做断言处理?
	//设计最大包裹大小
	//直接断言吧
	if(CheckBagAndPos(bag,pos))
	{
		ASSERT(false);
		return;
	}
	string s = item.ToString();
	int true_pos =  TransformPos(bag,pos);

	auto iter =  itemsMap_.begin();
	for(;iter!=itemsMap_.end();++iter)
	{
		//除了ID其它可能都变了 所以这边必须用ID来判定
		if(iter->second.GetId()==item.GetId())
		{
			obj_->SetUInt32(iter->first,true_pos);
			obj_->SetStr(iter->first,item.ToString());
			//最后一步 更新MAP //map维护点3
			iter->second = item;
			break;
		}
	}
}

//保存物品,这是不拷贝保存
void ItemManager::SavePtr(const Item* item)
{
	auto iter =  itemsMap_.begin();
	for( ;iter != itemsMap_.end(); ++iter)
	{
		//除了ID其它可能都变了 所以这边必须用ID来判定
		if(iter->second.GetId() == item->GetId())
		{
			ASSERT(item == &iter->second);		//这两个的地址一定也是同一个
			obj_->SetStr(iter->first,item->ToString());		
			break;
		}
	}
}

//统计指定物品数量
int ItemManager::CountEntey(int entry,int bag)
{
	if(CheckBagAndPos(bag,0))
		return 0;
	int count = 0;

	//遍历
	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++iter)
	{
		uint32_t true_pos = obj_->GetUInt32(iter->first);
		if(iter->second.GetEntry() != entry || true_pos/kMaxBagSize != bag)
			continue;
		count += iter->second.GetCount();
	}

	return count;
}

//扣除指定模板的物品
int ItemManager::Sub(int entry, int count,int bag)
{
	if(CheckBagAndPos(bag,0))
		ASSERT(false);
	//TODO 这边负不负责验证数量
	if(CountEntey(entry,bag) < count)
	{
		return -1;
	}

	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++iter)
	{
		Item &item = iter->second;
		uint32_t true_pos = obj_->GetUInt32(iter->first);
		//验证背包和物品entry
		if(item.GetEntry() != entry || true_pos/kMaxBagSize != bag)
			continue;
		//只要减掉数量
		int cur_count = item.GetCount();
		//三种情况 有多 刚好 不够扣
		if(cur_count>count)
		{
			item.SetCount(cur_count-count);
			Save(item, bag,true_pos%kMaxBagSize);
			return 0;
		}
		else if(cur_count==count)
		{
			obj_->SetUInt32(iter->first,0);
			obj_->SetStr(iter->first,"");
			itemsMap_.erase(iter->first);
			return 0;
		}
		else //连物品一起删除
		{
			obj_->SetUInt32(iter->first,0);
			obj_->SetStr(iter->first,"");
			itemsMap_.erase(iter->first);
			count -= item.GetCount();
		}
	}

	return 0;
}

//通过
int ItemManager::GetBinlogIndexBypos(int true_pos)
{
	for (uint32_t i=kItemPosStart;i<obj_->length_uint32();++i)
	{
		int pos = obj_->GetUInt32(i);
		string s = obj_->GetStr(i);
		if(pos == true_pos && s!="")
			return i;
	}
	return -1;
}

//移动物品
void ItemManager::Move(int id, int to_bag, int to_pos)
{
	if(CheckBagAndPos(to_bag,to_pos))
		ASSERT(false);
	
	//此处不管业务验证 
	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++iter)
	{
		//找到物品
		if (iter->second.GetId() == id)
		{
			int old_pos = obj_->GetUInt32(iter->first);	//先保存下旧的位置
			//看下目的位置有没有东西
			int index = GetBinlogIndexBypos(TransformPos(to_bag, to_pos));
			if (index != -1 && index != iter->first)
			{
				obj_->SetInt32(index, old_pos);
			}
			obj_->SetInt32(iter->first,TransformPos(to_bag, to_pos));
		}
	}
}	

//BINLOG找个位置放物品
int ItemManager::FindBinlogEmptyPos()
{
	for (uint32_t i=kItemPosStart;i<obj_->length_uint32();++i)
	{
		string s = obj_->GetStr(i);
		//回收利用
		if(s=="")
			return i;
	}
	//给个最新的位置 起码要从32开始
	return obj_->length_uint32()>=kItemPosStart?obj_->length_uint32():kItemPosStart;
}

//从包裹内部找空位
int ItemManager::GetEmptyPos(int bag)
{
	//怎么找捏。。先把这个包裹的里的东西 全弄出来。。然后捏，，一个个找位置。。知道
	std::set<int> not_empty;
	int min_pos = bag*kMaxBagSize;
	int max_pos = bag*kMaxBagSize+GetBagSize(bag)-1;
	//把这个包裹有东西的位置记起来
	for(auto iter= itemsMap_.begin();iter!=itemsMap_.end();++iter)
	{
		int pos =obj_->GetUInt32(iter->first); 
		if(pos>=min_pos && pos<=max_pos)
			not_empty.insert(pos-bag*kMaxBagSize);
	}
	//找到一个空位
	for(int i = 0;i<GetBagSize(bag);i++)
	{
		if(not_empty.find(i)==not_empty.end())
			return i;
	}

	return -1;
}

//找背包空余位置
int ItemManager::GetEmptyCount(int bag)
{
	int count = 0;
	int min_pos = bag*kMaxBagSize;
	int max_pos = bag*kMaxBagSize+GetBagSize(bag)-1;
	//把这个包裹有东西的位置记起来
	for(auto iter=itemsMap_.begin();iter!=itemsMap_.end();++iter)
	{
		int pos =obj_->GetUInt32(iter->first); 
		if(pos>=min_pos && pos<=max_pos)
			count++;
	}
	return GetBagSize(bag)-count;
}

//包裹的业务大小 包裹类型从0开始
int ItemManager::GetBagSize(int bag)
{
	ASSERT(bag<kBagCount);
	return obj_->GetInt16(kBagSizeIndex+bag/2,bag%2);
}
void ItemManager::SetBagSize(int bag,int size)
{
	if (obj_->GetUInt16(kBagSizeIndex+bag/2, bag%2) != size)
		obj_->SetInt16(kBagSizeIndex+bag/2,bag%2,size);
	ASSERT(bag<kBagCount);
}

//从binlog内部获得一个新的ID TODO 40多亿以后的复用机制 或者考虑用double
int ItemManager::GetNewID()
{
	if (m_cur_max_id == 0)
		m_cur_max_id = obj_->GetUInt32(kItemIDJumpIndex);

	++m_cur_max_id;	
	obj_->SetUInt32(kItemIDJumpIndex, m_cur_max_id);
	return m_cur_max_id;
}

//遍历指定包裹的所有物品
void ItemManager::ForEachBagItem(int bag, std::function<bool (Item*,int)> func)
{
	for (auto iter = itemsMap_.begin(); iter != itemsMap_.end(); ++ iter)
	{
		uint32_t true_pos = obj_->GetUInt32(iter->first);
		if (true_pos/kMaxBagSize != bag)
			continue;
		if (func(&iter->second, true_pos%kMaxBagSize))
			break;
	}
}

//获得物品管理器guid
const string& ItemManager::GetGuid()
{
	ASSERT(obj_);
	return obj_->GetStr(0);
}


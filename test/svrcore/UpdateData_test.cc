#include <gtest/gtest.h>
#include <comm/common_stl.h>
#include <net/packet.h>
#include "ObjectInstance.h"

#if 0
void __func(std::set<uint64>* _t,uint64 v)
{
	_t->insert(v);
}

TEST(UpdateData,Copy)
{
	vector<uint64> arr(3);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;

	std::set<uint64> _s;
	std::bind1st(std::ptr_fun(__func),&_s);
	std::mem_fun(&std::set<uint64>::insert)(&_s,3);

//	std::bind1st(std::mem_fun(&std::vector<uint64>::push_back),&arr);
	//std::mem_fun1(&vector<uint64>::push_back)(&arr,3);
//	std::bind1st(std::_Void_mem_fun1_ptr(std::mem_fun1(&vector<uint64>::push_back)),&arr);

		
	//for_each(arr.begin(),arr.end(),std::bind1st(std::ptr_fun(__func),&_s));

	std::vector<uint64> dest(3,99);
	dest.resize(dest.size()+arr.size());

	std::copy(arr.begin(),arr.end(),dest.end()-arr.size());
	/*std::vector<uint64> m_result(100);
	std::merge(arr.begin(),arr.end(),dest.begin(),dest.begin(),m_result.begin());*/


	//std::for_each(arr.begin(),arr.end(),)
	/*vector<uint64> v2;
	mem_fun_ref(vector<uint64>::push_back)(v2,1);*/
}
#endif

//class ObjectFactoryTest2:public IObjectFactory
//{
//public:
//	ObjectFactoryTest2()
//	{
//		UpdateBlock::obj_factory = this;
//	}
//
//	virtual Object *FindObject(uint64 guid)
//	{
//		return NULL;
//	}
//
//	virtual Object *CreateObject(uint64 guid)
//	{
//		PlayerData *data = new PlayerData();
//		data->Create(1,"test",0,0,0,0);
//		return data;
//	}
//};
//
//static ObjectFactoryTest2 oft;

//TEST(UpdateData,UpdateData_Packet/*打包解包测试*/)
//{
//	UpdateData data_test;
//	std::vector<ObjectInstance *> objectVec;
//	for (uint16 i = 0;i<10;i++)
//	{
//		ObjectInstance *obj  = new ObjectInstance; 
//		string name = "菜刀";
//		static_cast<ObjectInstance*>(obj)->Create(i,name,0,0,0,0);
//		//初始气血能量
//		static_cast<ObjectInstance*>(obj)->SetMaxHealth(i);					
//		static_cast<ObjectInstance*>(obj)->SetHealth(i);
//		byte_buffer *buffer = data_test.MallocBuffer();
//		static_cast<ObjectInstance*>(obj)->CreateUpdateBlock(UPDATEMASK_UPDATE,NULL,*buffer);
//
//		data_test.AddUpdateBlock(buffer);
//		objectVec.push_back(obj);
//	}
//	packet *pkt;
//	packet_init(&pkt);
//	data_test>>(*pkt);
//	
//	uint16 size;
//	*pkt>>size;
//	EXPECT_EQ(size, 10);
//
//	for(uint16 i = 0;i<size;++i)
//	{
//		uint16 flag;
//		*pkt>>flag;
//		EXPECT_EQ(flag, UPDATEMASK_UPDATE);
//		uint64 guid;
//		*pkt >> guid;
//		EXPECT_EQ(guid, objectVec[i]->GetGUID());
//		uint8  type_id;
//		*pkt >> type_id;
//		EXPECT_EQ(type_id, objectVec[i]->GetTypeId());
//		ObjectInstance *obj  = new ObjectInstance; 
//		obj->Create(uint32(guid), objectVec[i]->GetName(), 0,0,0,0);
//		obj->ApplyUpdateBlock(flag, *pkt);
//		EXPECT_EQ(obj->GetHealth(), objectVec[i]->GetHealth());
//		delete objectVec[i];
//		delete obj;
//	}
//	packet_free(pkt);
//}

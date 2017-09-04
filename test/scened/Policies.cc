#include <gtest/gtest.h>
#include <comm/common_stl.h>
#include "Policies.h"

namespace Tea
{

struct PoliciesCreateStruct
{
	void Hello()
	{
		std::cout << "Hello:this is ," << (void*)this << std::endl;
	}
};

template<typename T>
struct PoliciesCreateStructCreateCallBack
{
	static T *CreateCallBack()
	{
		return new T();
	}
	
	static void DestroyCallBack(T* t)
	{
		delete t;
	}

};

TEST(Policies,Object_Instance_Create)
{
	//用new 分配
	PoliciesCreateStruct *instance1 = OperatorNew<PoliciesCreateStruct>::Create();
	instance1->Hello();
	OperatorNew<PoliciesCreateStruct>::Destroy(instance1);

	//栈区分配
	PoliciesCreateStruct *instance2 = LocalStaticCreation<PoliciesCreateStruct>::Create();
	instance2->Hello();
	LocalStaticCreation<PoliciesCreateStruct>::Destroy(instance2);

	//malloc
	PoliciesCreateStruct *instance3 = CreateUsingMalloc<PoliciesCreateStruct>::Create();
	instance3->Hello();
	CreateUsingMalloc<PoliciesCreateStruct>::Destroy(instance3);

	//callback
	PoliciesCreateStruct *instance4 = CreateOnCallBack<PoliciesCreateStruct,PoliciesCreateStructCreateCallBack<PoliciesCreateStruct> >::Create();
	instance4->Hello();
	CreateOnCallBack<PoliciesCreateStruct,PoliciesCreateStructCreateCallBack<PoliciesCreateStruct> >::Destroy(instance4);
}

INSTANTIATE_SINGLETON_1(PoliciesCreateStruct);

TEST(Policies, Singleton)
{
	Singleton<PoliciesCreateStruct>::Instance().Hello();
}

}


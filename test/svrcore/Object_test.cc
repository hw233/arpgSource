//#include <gtest/gtest.h>
//#include <svrcore.h>
//#include <object/Object.h>
//#include "ObjectInstance.h"
//
//TEST(Object,GUID)
//{
//	ObjectInstance oi;
//
//	uint32 guidlow = 18;
//	const std::string name = "volcano";
//
//#if 0
//	oi.Create(guidlow,name,5,1,2,5);	
//#else
//	oi.Create(0,name,0,0,0,0);	
//	stringstream ss;
//	ss << MAKE_NEW_GUID(guidlow,0,HIGHGUID_PLAYER);
//	oi.SetStr(OBJECT_STRING_FIELD_GUID, ss.str().c_str());
//#endif
//	ASSERT_EQ(oi.GetGUID(),guidlow);
//	for(int i = 0; i < 32; i++){		
//		//EXPECT_FALSE(oi.GetBit(PLAYER_QUEST_LOG_0,i));
//	}
//	for(int i = 0; i < 32; i++){
//		//if(i & 0x01)
//			//oi.SetBit(PLAYER_QUEST_LOG_0,i);
//	}
//	for(int i = 0; i < 32; i++){
//		//if(i & 0x01)
//		//	EXPECT_TRUE(oi.GetBit(PLAYER_QUEST_LOG_0,i));
//	}
//
//}
//
//
////Object属性设置及读取测试
//TEST(Object, Object_PROE)
//{
//	ObjectInstance *p = new ObjectInstance();
//	uint32 guidlow = 18;
//	const std::string name = "volcano";
//	p->Create(guidlow,name,5,1,2,5);
//	const std::string name1 = p->GetName();
//
//	p->SetEntry(168);
//	ASSERT_EQ(p->GetEntry(),168);
//	ASSERT_EQ(name1,name);
//
//	
//
//	EXPECT_EQ(p->GetGUID(),guidlow); //----取低位GUID有问题	GetGUID()
////	EXPECT_EQ(p->GetGUIDMid(),2); //----取低位GUID有问题	GetGUID()
//	delete p;
//}

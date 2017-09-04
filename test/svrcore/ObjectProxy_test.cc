//#ifndef TEST_PROXY
//#define TEST_PROXY
//#endif
//
//#include <gtest/gtest.h>
//#include <object/GameObject.h>
//
//#define private public
//#define protected public
//#include <object/ObjectProxy.h>
//
//class TestObject : public Object
//{
//public:
//	TestObject()
//	{
//		m_valuesCount = MAX_GAMEOBJCET_FIELD;
//		this->_Create(1,1,HIGHGUID_PLAYER);
//	}
//	~TestObject(){}
//public:
//	uint16 GetValuesCount()
//	{
//		return m_valuesCount;
//	}
//};
//
//TEST(ObjectProxy, TEST)
//{
//	TestObject *obj_loca = new TestObject;
//	TestObject *obj_remo = new TestObject;
//	ObjectProxy op_loca, op_remo;
//	packet *pkt = NULL;
//	packet_init(&pkt);
//	op_loca.m_obj = obj_loca;
//	op_remo.m_obj = obj_remo;
//	for(uint32 i = 0; i < obj_loca->GetValuesCount(); i++)
//	{
//		op_loca.RemoteSetUInt32(i, 1);
//		op_loca.RemoteSetUInt16(i, 0, 1);
//		op_loca.RemoteSetUInt16(i, 1, 1);
//		op_loca.RemoteSetByte(i, 0, 1);
//		op_loca.RemoteSetByte(i, 1, 1);
//		op_loca.RemoteSetByte(i, 2, 1);
//		op_loca.RemoteSetByte(i, 3, 1);
//		op_loca.RemoteAddUInt32(i, 1);
//		op_loca.RemoteAddUInt16(i, 0, 1);
//		op_loca.RemoteAddUInt16(i, 1, 1);
//		op_loca.RemoteAddByte(i, 0, 1);
//		op_loca.RemoteAddByte(i, 1, 1);
//		op_loca.RemoteAddByte(i, 2, 1);
//		op_loca.RemoteAddByte(i, 3, 1);
//		op_loca.RemoteSubUInt32(i, 1);
//		op_loca.RemoteSubUInt16(i, 0, 1);
//		op_loca.RemoteSubUInt16(i, 1, 1);
//		op_loca.RemoteSubByte(i, 0, 1);
//		op_loca.RemoteSubByte(i, 1, 1);
//		op_loca.RemoteSubByte(i, 2, 1);
//		op_loca.RemoteSubByte(i, 3, 1);
//	}
//	op_loca >> *pkt;
//	op_remo << *pkt;
//	op_remo.Clear();
//	for(uint32 i = 0; i < obj_loca->GetValuesCount(); i++)
//	{
//		EXPECT_EQ(obj_loca->GetUInt32(i), obj_remo->GetUInt32(i));			
//	}
//	for(uint32 i = 0; i < obj_loca->GetValuesCount(); i++)
//	{
//		op_loca.RemoteSetBit(i, 0);
//		op_loca.RemoteSetBit(i, 1);
//		op_loca.RemoteSetBit(i, 2);
//		op_loca.RemoteSetBit(i, 3);
//		op_loca.RemoteSetBit(i, 4);
//		op_loca.RemoteSetBit(i, 5);
//		op_loca.RemoteSetBit(i, 6);
//		op_loca.RemoteSetBit(i, 7);
//	}
//	op_loca >> *pkt;
//	op_remo << *pkt;
//	for(uint32 i = 0; i < obj_loca->GetValuesCount(); i++)
//	{
//		EXPECT_EQ(obj_loca->GetUInt32(i), obj_remo->GetUInt32(i));			
//	}
//	for(uint32 i = 0; i < obj_loca->GetValuesCount(); i++)
//	{
//		op_loca.RemoteUnSetBit(i, 0);
//		op_loca.RemoteUnSetBit(i, 1);
//		op_loca.RemoteUnSetBit(i, 2);
//		op_loca.RemoteUnSetBit(i, 3);
//		op_loca.RemoteUnSetBit(i, 4);
//		op_loca.RemoteUnSetBit(i, 5);
//		op_loca.RemoteUnSetBit(i, 6);
//		op_loca.RemoteUnSetBit(i, 7);
//	}
//	op_loca >> *pkt;
//	op_remo << *pkt;
//	for(uint32 i = 0; i < obj_loca->GetValuesCount(); i++)
//	{
//		EXPECT_EQ(obj_loca->GetUInt32(i), obj_remo->GetUInt32(i));			
//	}
//	ASSERT_EQ(1, 1);
//
//	packet_free(pkt);
//	delete obj_loca;
//	delete obj_remo;
//}
//#undef private
//#undef protected

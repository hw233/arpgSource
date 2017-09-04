#include <gtest/gtest.h>
#include <protocol/WorldPacket.h>
#include <object/BinLogObject.h>

class BinLogTestObj:public BinLogObject
{
	
};
#if 0
TEST(BinLogObject,WriteCreateBinLog)
{
	BinLogTestObj o1,o2;
	WorldPacket packet;
	
	o1.Init(4,2);
	o2.Init(4,2);
	o1.SetInt32(0,100);
	o1.SetBit(1,1);
	o1.SetByte(2,2,1);
	o1.SetUInt16(3,1,3);
	ASSERT_EQ(o1.GetUInt16(3,1),3);
	o1.SetStr(1,"hello");
	o1.WriteCreateBinLog(packet.GetPkt());
	o2.ReadCreateBinLog(packet.GetPkt());

	for (int i = 0; i < o1.GetUInt32Count(); i++)
	{
		ASSERT_EQ(o1.GetUInt32(i) ,o2.GetUInt32(i)); 
	}

	for (int i = 0; i < o1.GetStringCount(); i++)
	{
		ASSERT_TRUE(o1.GetStr(i) == o2.GetStr(i));
	}
	ASSERT_EQ(o2.GetInt32(0),100);
	ASSERT_TRUE(o2.GetBit(1,1));
	ASSERT_EQ(o2.GetByte(2,2),1);
	ASSERT_EQ(o2.GetUInt16(3,1),3);

	//////////////////////////////////////////////////////////////////////
	//²âÊÔ¸üÐÂ
	o1.Clear();
	o2.Clear();

	WorldPacket packet_update;
	o1.SetInt32(0,200);
	o1.SetBit(1,2);
	o1.SetByte(2,2,5);
	o1.SetUInt16(3,1,4);
	o1.SetStr(1,"hello1");

	o1.WriteBinLog(packet_update.GetPkt());
	o2.ReadBinLog(packet_update.GetPkt());

	for (int i = 0; i < o1.GetUInt32Count(); i++)
	{
		ASSERT_EQ(o1.GetUInt32(i) ,o2.GetUInt32(i)); 
	}

	for (int i = 0; i < o1.GetStringCount(); i++)
	{
		ASSERT_TRUE(o1.GetStr(i) == o2.GetStr(i));
	}
	ASSERT_EQ(o2.GetInt32(0),200);
	ASSERT_TRUE(o2.GetBit(1,2));
	ASSERT_EQ(o2.GetByte(2,2),5);
	ASSERT_EQ(o2.GetUInt16(3,1),4);
}


TEST(BinLogObject,BinLogStru)
{
	BinLogTestObj::BinLogStru bls;
	memset(&bls,0,sizeof(bls));
	bls.opt = BinLogTestObj::BINLOG_OPT_SET;
	bls.typ = BinLogTestObj::BINLOG_TYP_UINT16;
	bls.index = 2;

	bls.offset = 0;
	bls.uint16_val = 4;

	auto get_byte = [](uint32 value,int offset)->uint8{
		return (value&(0x000000FF << (offset<<3)))>> (offset<<3) & 0xFF;
	};
	ASSERT_EQ(get_byte(bls.value,0),bls.offset);

	auto get_uint16 = [](uint32 value,int offset)->uint16{
		return (value & (0x0000FFFF << (offset << 4))) >> (offset << 4) & 0xFFFF;
	};
	ASSERT_EQ(get_uint16(bls.value,1),bls.uint16_val);	

	///////////////
	//²âÊÔbyte
	bls.value = 0;

	bls.offset = 5;
	bls.byte_val = 4;	

	ASSERT_EQ(get_byte(bls.value,0),bls.offset);
	ASSERT_EQ(get_uint16(bls.value,1),bls.byte_val);	
}
#endif

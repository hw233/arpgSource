#include <comm/common_stl.h>
#include <gtest/gtest.h>
#include <set>
#include "protocol/WorldPacket.h"

TEST(WorldPacket,uint8)
{
	WorldPacket a;
	uint8 b;
	a <<(uint8)100;
	a >>b;
	ASSERT_EQ(b,(uint8)100);
}
TEST(WorldPacket,int8)
{
	WorldPacket a;
	int8 b;
	a <<(int8)100;
	a >>b;
	ASSERT_EQ(b,(int8)100);
}
TEST(WorldPacket,uint16)
{
	WorldPacket a;
	uint16 b;
	a <<(uint16)100;
	a >>b;
	ASSERT_EQ(b,(uint16)100);
}
TEST(WorldPacket,int16)
{
	WorldPacket a;
	int16 b;
	a <<(int16)100;
	a >>b;
	ASSERT_EQ(b,(int16)100);
}
TEST(WorldPacket,uint32)
{
	WorldPacket a;
	uint32 b;
	a <<(uint32)100;
	a >>b;
	ASSERT_EQ(b,(uint32)100);
}
TEST(WorldPacket,int32)
{
	WorldPacket a;
	int32 b;
	a <<(int32)100;
	a >>b;
	ASSERT_EQ(b,(int32)100);
}
TEST(WorldPacket,uint64)
{
	WorldPacket a;
	uint64 b;
	a <<(uint64)100;
	a >>b;
	ASSERT_EQ(b,(uint64)100);
}
TEST(WorldPacket,int64)
{
	WorldPacket a;
	int64 b;
	a <<(int64)100;
	a >>b;
	ASSERT_EQ(b,(int64)100);
}
TEST(WorldPacket,float)
{
	WorldPacket a;
	float b;
	a <<(float)100;
	a >>b;
	ASSERT_EQ(b,(float)100);
}
TEST(WorldPacket,double)
{
	WorldPacket a;
	double b;
	a <<(double)100;
	a >>b;
	ASSERT_EQ(b,(double)100);
}
TEST(WorldPacket,string)
{
	WorldPacket a;
	string b;
	a <<(string)"100";
	a >>b;
	ASSERT_EQ(b,(string)"100");
}

TEST(WorldPacket,_bool)
{
	WorldPacket a;
	bool b;
	a << false;
	a >> b;
	ASSERT_EQ(b,false);
}
TEST(WorldPacket,vector2000)
{
	WorldPacket a;
	std::vector<int> b;
	std::vector<int> c;
	for(int i = 0;i<2000;i++)
	{
		b.push_back(i);
	}
	a << b;
	a  >>c;
	ASSERT_EQ(b.size(),c.size());
	for(int j = 0;j<2000;j++)
	{
		ASSERT_EQ(b[j],c[j]);
	}
}

const int LOOP_COUNT = 2000;

TEST(WorldPacket,map2000)
{
	WorldPacket a;
	std::map<int,int> b;
	std::map<int,int> c;
	for(int i = 0;i<LOOP_COUNT;i++)
	{
		b[i]=i;
	}
	a << b;
	a  >>c;
	ASSERT_EQ(b.size(),c.size());
	for(int j = 0;j<LOOP_COUNT;j++)
	{
		ASSERT_EQ(b[j],c[j]);
	}
}

TEST(WorldPacket,list2000)
{
	WorldPacket a;
	std::list<int> b;
	std::list<int> c;
	for(int i = 0;i<LOOP_COUNT;i++)
	{
		b.push_back(i);
	}
	a << b;
	a  >>c;
	ASSERT_EQ(b.size(),c.size());
	std::list<int>::iterator iter1=c.begin();
	for(std::list<int>::iterator iter = b.begin();iter!=b.end();iter1++,iter++)
	{
		ASSERT_EQ(*iter,*iter1);
	}
}

TEST(WorldPacket,set2000)
{
	WorldPacket a;
	std::set<int> b;
	std::set<int> c;
	for(int i = 0;i<LOOP_COUNT;i++)
	{
		b.insert(i);
	}
	a << b;
	a  >>c;
	ASSERT_EQ(b.size(),c.size());
	std::set<int>::iterator iter1=c.begin();
	for(std::set<int>::iterator iter = b.begin();iter!=b.end();iter1++,iter++)
	{
		ASSERT_EQ(*iter,*iter1);
	}
}

TEST(WorldPacket,finenal_test/*×îºó²âÊÔ*/)
{
	WorldPacket a;
	a <<(int8)100;
	a <<(uint8)200;
	a <<(int16)300;
	a <<(uint16)400;
	a <<(int32)500;
	a <<(uint32)600;
	a <<(int64)700;
	a <<(uint64)800;
	a <<(float)900;
	a <<(double)1000;
	a <<(string)"100";

	std::vector<int> vb;
	std::vector<int> vvb;
	for(int i = 0;i<LOOP_COUNT;i++)
	{
		vb.push_back(i);
	}
	a << vb;

	std::map<int,int> mc;
	std::map<int,int> mmc;
	for(int i = 0;i<LOOP_COUNT;i++)
	{
		mc[i] = i;
	}
	a << mc;

	std::list<int> ld;
	std::list<int> lld;
	for(int i = 0;i<LOOP_COUNT;i++)
	{
		ld.push_back(i);
	}
	a << ld;

	std::set<int> sc;
	std::set<int> ssc;
	for(int i = 0;i<LOOP_COUNT;i++)
	{
		sc.insert(i);
	}
	a << sc;

	int8		b;
	uint8		c;
	int16		d;
	uint16		e;
	int32		f;
	uint32		g;
	int64		h;
	uint64		o;
	float		p;
	double		q;
	string		r;
	a >> b;
	a >> c;
	a >> d;
	a >> e;
	a >> f;
	a >> g;
	a >> h;
	a >> o;
	a >> p;
	a >> q;
	a >> r;
	a >> vvb;
	a >> mmc;
	a >> lld;
	a >> ssc;
	ASSERT_EQ(b,(int8)100);
	ASSERT_EQ(c,(uint8)200);
	ASSERT_EQ(d,(int16)300);
	ASSERT_EQ(e,(uint16)400);
	ASSERT_EQ(f,(int32)500);
	ASSERT_EQ(g,(uint32)600);
	ASSERT_EQ(h,(int64)700);
	ASSERT_EQ(o,(uint64)800);
	ASSERT_EQ(p,(float)900);
	ASSERT_EQ(q,(double)1000);
	ASSERT_EQ(r,(string)"100");
	ASSERT_EQ(vb.size(),vvb.size());
	for(int j = 0;j<LOOP_COUNT;j++)
	{
		ASSERT_EQ(vb[j],vvb[j]);
	}
	ASSERT_EQ(mc.size(),mmc.size());
	for(int j = 0;j<LOOP_COUNT;j++)
	{
		ASSERT_EQ(mc[j],mmc[j]);
	}
	ASSERT_EQ(ld.size(),lld.size());
	std::list<int>::iterator iter1=ld.begin();
	for(std::list<int>::iterator iter = lld.begin();iter!=lld.end();iter1++,iter++)
	{
		ASSERT_EQ(*iter,*iter1);
	}
	ASSERT_EQ(sc.size(),ssc.size());
	std::set<int>::iterator iter2=sc.begin();
	for(std::set<int>::iterator iter = ssc.begin();iter!=ssc.end();iter2++,iter++)
	{
		ASSERT_EQ(*iter,*iter2);
	}
}

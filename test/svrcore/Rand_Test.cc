#include <gtest/gtest.h>
#include <comm/common_stl.h>
#include <shared/util.h>
#include <shared/probability_rand.h>

TEST(util,test_urand1)
{
	uint32 x1=0;
	uint32 x2=0;
	for (int i=0;i<10000;i++)
	{
		uint32 temp=urand(1,10000);
		if(temp<=5000)
			x1++;
		else
			x2++;
	}
	//随机下机率不大于%1即可
	int cmp_result = abs(int(x1-x2));
	//ASSERT_LT(cmp_result,int(10000/100));
}

TEST(util,test_urand2)
{
	uint32 x1=0;
	uint32 x2=0;
	for (int i=0;i<1000;i++)
	{
		uint32 temp=urand(1,1000);
		if(temp<=500)
			x1++;
		else
			x2++;
	}
	int dist = x1>x2 ? x1-x2 : x2-x1;	
	//ASSERT_LT(dist,10);
}

TEST(util,test_urand3)
{
	uint32 x1=0;
	uint32 x2=0;
	for (int i=0;i<100;i++)
	{
		uint32 temp=urand(1,100);
		if(temp<=50)
			x1++;
		else
			x2++;
	}
	int dist = x1>x2 ? x1-x2 : x2-x1;
	//ASSERT_LT(dist,1);	
}

uint32 Rand_test1(uint32 imax)
{
	ProbabilityRand<int>* rand=new ProbabilityRand<int>;
	rand->AddRandPossibility(25,1);
	rand->AddRandPossibility(25,2);
	rand->AddRandPossibility(25,3);
	rand->AddRandPossibility(25,4);
	uint32 x1=0;
	for (uint32 i=0;i<imax;i++)
	{
		if(rand->Rand(1)==1&&rand->Rand(1)==2&&rand->Rand(1)==3&&rand->Rand(1)==4)
		{
			x1++;
		}
	}
	delete rand;
	return x1;
}



TEST(probability_rand,rand1)
{
	int x1=Rand_test1(1000);
	//ASSERT_LE(x1,4);
}

TEST(probability_rand,rand2)
{
	int x1=Rand_test1(10000);
	//ASSERT_LE(x1,40);
}

TEST(probability_rand,rand3)
{
	int x1=Rand_test1(100000);
	//ASSERT_LE(x1,400);
}

TEST(probability_rand,base_rand)
{
	int count = 0;
	const int COUNT_NUM = 10000;
	for (int i = 0; i < COUNT_NUM; i++)
	{
		//机率等于10%
		if(urand(0,9999) >= 9000)
			count++;
	}	

	count = 0;
	for (int i = 0; i < COUNT_NUM*2; i++)
	{
		//机率等于10%
		if(urand(0,9999) >= 9000)
			count++;
	}	

	count = 0;
	for (int i = 0; i < COUNT_NUM*10; i++)
	{
		//机率等于10%
		if(urand(0,9999) >= 9000)
			count++;
	}	

	count = 0;
	for (int i = 0; i < COUNT_NUM*100; i++)
	{
		//机率等于10%
		if(urand(0,9999) >= 9000)
			count++;
	}	
}

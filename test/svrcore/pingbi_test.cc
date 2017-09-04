#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <server/svrcore_app.h>
extern "C" {
#include <ahocorasick/aho_corasick.h>
}

TEST(PINGBI,pingbi)
{
#if 0
	AC_AUTOMATA m_aca;
	uint32 i = 0,j, test_count = 10;
	const char *join_str = "趣游厦门";
	char *test_str = (char*)malloc(1000);
	time_t t1,t2;

	std::cout << "Load  _fuck_pingbi.txt ING" << std::endl;
	char *path = "E://Server//trunk//tea//contrib//data//_fuck_pingbi.txt";
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if(!file)
	{
		std::cout << "LoadFuckPingbi fail" << std::endl;
		ASSERT_EQ(1,0);
	}


	unsigned int id = 1, start = 0, count  =0, len;	
	std::string str;
	STRING tmp_str;	
	std::stringstream buffer;
	char *m_fuck_pingbi;
	vector<uint32> v_index;
	time(&t1);

	//初始化AC自动机
	ac_automata_init (&m_aca, &match_handler);

	buffer << file.rdbuf();   
	str = buffer.str();  

	file.close(); 
	len = str.length() + 1;
	m_fuck_pingbi = (char *)malloc(len * sizeof(char *));
	for(i = 0; i < len; ++i)
	{
		if(strchr("\r\n", str.c_str()[i]))
		{
			m_fuck_pingbi[i] = 0;
			count++;
		}
		else
			m_fuck_pingbi[i] = str.c_str()[i];
	}
	for (i = 0; i < len; ++i)
	{
		if(m_fuck_pingbi[i] == 0)
		{
			if(start != i)
			{
				tmp_str.id = id++;
				tmp_str.str = (ALPHA *)(m_fuck_pingbi + start);
				tmp_str.length = i - start;
				ac_automata_add_string(&m_aca, &tmp_str);
				v_index.push_back(start);
			}
			start = i + 1;
		}
	}
	ac_automata_locate_failure (&m_aca);
	std::cout << "Load  _fuck_pingbi.txt OK!" << std::endl;

	if(v_index.size() != --count)
	{
		ASSERT_EQ(1,2);
	}

	time(&t2);
	std::cout << t2 - t1 << std::endl;
	std::cout << "Load  _fuck_pingbi.txt OK!" << std::endl;
	STRING pb_str;
	std::cout << "PingBi start" << std::endl;
	time(&t1);

	for(;i < 1000000; ++i)
	{
		uint32 index = i % count;
		char *pingbi_str = m_fuck_pingbi + v_index[index];
		memset(test_str, 0, 1000);
		strcat(test_str, join_str);
		for(j = 0; j < test_count; ++j)
		{
			strcat(test_str, pingbi_str);
			strcat(test_str, join_str);
		}

		pb_str.str = test_str;
		pb_str.length = strlen(pb_str.str);

		ac_automata_search (&m_aca, &pb_str, (void *)test_str);

		ac_automata_reset(&m_aca);
		//std::cout << test_str << std::endl;
	}
	time(&t2);
	std::cout << t2 - t1 << std::endl;
	std::cout << "PingBi end" << std::endl;
	ASSERT_LT(t2 - t1,50);
#endif
}

TEST(PINGBI,error)
{
	ASSERT_EQ(1,1);
}
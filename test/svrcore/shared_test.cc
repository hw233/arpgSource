#include <gtest/gtest.h>
#include <shared/progress_bar.h>
#include <shared/storage_loader.h>
//#include <shared/storage_loader.h>
TEST(svrcore_shared,progress_bar)
{
	const int count = 10000;
	barGoLink bg(count);
	for (int i=0;i<count;i++)
	{
		bg.step();
	}
}

//TEST(svrcore_shared,ËØ²Ä¼ÓÔØ)
//{
//	std::string __path = "D:/";
//	int result = Load_map_base_info(__path);
//	ASSERT_EQ(result,0);
//}
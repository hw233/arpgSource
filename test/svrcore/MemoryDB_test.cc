#include <gtest/gtest.h>
#include <comm/common.h>
#include <shared/MemoryDB.h>

typedef struct test_data_t
{
	int32 i;
	int32 j;
	int32 k;
	int32 l;
	int32 m;
}test_data;

#define DATA_NUM 100
#define PRE_INDEX 10
#define _RAND_ 10
#define _RAND2_ 2

typedef struct test_chars_t
{
	int32 i;
	char j[20];
}test_chars;

TEST(MemoryDB, create_index)
{
	MemoryDB<test_data> memDB(NULL);
	memdb_index * index = memDB.create_index();
	memDB.add_index_cell(index, MEMDB_INDX(test_data_t, i,default_compare_handle));
	bool b = memDB.add_index(index);//单索引
	ASSERT_EQ(b, true);

	index = memDB.create_index();
	memDB.add_index_cell(index, MEMDB_INDX(test_data_t, i,default_compare_handle));
	memDB.add_index_cell(index, MEMDB_INDX(test_data_t, j,default_compare_handle));
	b = memDB.add_index(index);//组合索引
	ASSERT_EQ(b, true);
}

TEST(MemoryDB,select)
{
	MemoryDB<test_data> memDB(NULL);
	memdb_index * index = memDB.create_index();
	memDB.add_index_cell(index, MEMDB_INDX(test_data_t, i,default_compare_handle));
	memDB.add_index_cell(index, MEMDB_INDX(test_data_t, j,default_compare_handle));
	memDB.add_index(index);

	test_data *v = (test_data *)malloc(sizeof(test_data) * DATA_NUM);
	for(int i = 0 ; i < DATA_NUM/PRE_INDEX; ++i)
	{
		for(int j= 0;j < PRE_INDEX; ++j )
		{
			int k = i*PRE_INDEX + j;
			v[k].i = i;
			v[k].j = i + 1;
			v[k].k = i;
			v[k].l = i;
			v[k].m = i;
			memDB.insertDB(v + k);
		}
	}
	// 单条件搜索
	MemDB_Result<test_data> result = memDB.select(MEMDB_LT(test_data_t, i, (uint32)_RAND_));
	printf("%d\n\n", result.size());
	for(uint32 i = 0; i < result.size(); ++i)
	{
		ASSERT_LT(result[i]->i, _RAND_);
	}
	
	//多条件搜索
	memDB.add_select_condition(MEMDB_LT(test_data_t, i, (uint32)_RAND_));
	memDB.add_select_condition(MEMDB_GT(test_data_t, j, (uint32)_RAND2_));
	result = memDB.select();
	printf("%d\n\n", result.size());
	for(uint32 i = 0; i < result.size(); ++i)
	{
		ASSERT_LT(result[i]->i, _RAND_);
		ASSERT_GT(result[i]->j, _RAND2_);
	}
	free(v);
}

TEST(MemoryDB,insertVector)
{
	MemoryDB<test_data> memDB(NULL);
	memdb_index * index = memDB.create_index();
	memDB.add_index_cell(index, MEMDB_INDX(test_data_t, i,default_compare_handle));
	memDB.add_index_cell(index, MEMDB_INDX(test_data_t, j,default_compare_handle));
	memDB.add_index(index);

	test_data *v = (test_data *)malloc(sizeof(test_data) * 100);
	for(int i = 0 ; i < 100; ++i)
	{
		v[i].i = i;
		v[i].j = 100 - i;
		v[i].k = i;
		v[i].l = i;
		v[i].m = i;
		//插入下标连续数据（下标为偏移量为0， 整型为int的成员）
		memDB.insertVector(v + i);
	}

	for(int i = 0; i < 100; ++i)
	{
		ASSERT_EQ(memDB[i]->i, i);
		ASSERT_EQ(memDB[i]->j, 100 - i);
	}
	free(v);
}


TEST(MemoryDB,selectChars)
{
	MemoryDB<test_chars> memDB(NULL);
	memDB.register_compare_handle(MEMDB_CMP_PTR(test_chars, j, MemDB_Compare_Handle<char *>));
	test_chars *info = new test_chars ;		
	info->i = 1;
	strcpy(info->j ,"abc");
	memDB. insertVector(info);

	delete info;
	info = new test_chars ;		
	info->i = 1;
	strcpy(info->j ,"ab");
	memDB. insertVector(info);
	
	MemDB_Result<test_chars> result = memDB.select(MEMDB_EQ(test_chars, j, "ab"));
	printf("%d\n", result.size());
	printf("%s\n", result[0]->j);
	memdb_index * index = memDB.create_index();
	memDB.add_index_cell(index, MEMDB_INDX(test_chars, j,MemDB_Compare_Handle<char *>));
	memDB.add_index(index);
	result = memDB.select(MEMDB_EQ(test_chars, j, "ab"));
	printf("%d\n", result.size());
	printf("%s\n", result[0]->j);

	delete info;
}
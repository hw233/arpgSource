#include <gtest/gtest.h>
#include <shared/lua_helper.h>
//
//TEST(lua_helper,LuaGetTimeOfDay)
//{
//	lua_State *L = lua_open();
//	lua_register(L,"getTimeOfDay",LuaGetTimeOfDay);
//	uint32 a,b;
//
//	{
//		luaL_dostring(L,"a=getTimeOfDay('15:30')");
//
//		LuaStackAutoPopup ap(L);
//		lua_getglobal(L,"a");
//		a = (uint32)LUA_TOINTEGER(L,-1);	
//
//		///////////////////////
//		uint32 t_now = (uint32)time(NULL);		//现在的标准时间，要加上当前时区的小时数
//		uint32 t_day = t_now - t_now % 86400;	//今天0点的时间	
//		uint32 h,m;	
//		sscanf("15:30","%u:%u",&h,&m);
//		h = (h + 24 - 8) % 24;
//		uint32 need_time = 60*(h*60+m);
//		need_time += t_day;
//		//////////////////////
//
//		EXPECT_EQ(a,need_time);	
//	}
//	
//	{
//		LuaStackAutoPopup ap(L);
//		luaL_dostring(L,"b=getTimeOfDay('18:00','2013/09/21')");
//		lua_getglobal(L,"b");
//		b = (uint32)LUA_TOINTEGER(L,-1);	
//		EXPECT_EQ(b,1379757600);
//	}
//	
//	lua_close(L);
//}
//////////////////////////////////////////////////////////////////////////
//
//#include <deps/lua_tinker.h>
//
//namespace sample1
//{
//	//函数及C交互
//	int sample1_cpp_func(int arg1, int arg2)
//	{
//		return arg1 + arg2;
//	}
//
//	TEST(lua_helper,sample1)
//	{
//		lua_State* L = lua_open();
//		luaopen_base(L);
//
//		/*std::function<int (int,int)> cpp_func = [](int arg1, int arg2){
//			return arg1 + arg2;
//		};*/
//		lua_tinker::def(L, "cpp_func", sample1_cpp_func);
//	
//		lua_tinker::dostring(L, "result = cpp_func(1, 2)");
//		lua_tinker::dostring(L, "print('cpp_func(1,2) = '..result)");
//		lua_tinker::dostring(L, "\
//			function lua_func(arg1, arg2)\
//				return arg1 + arg2\
//			end\
//			");	
//
//		int result = lua_tinker::call<int>(L, "lua_func", 3, 4);
//		printf("lua_func(3,4) = %d\n", result);
//
//		lua_close(L);
//	}
//
//}
//
//TEST(lua_helper,sample2)
//{
//	//lua全局变量交互
//	static int cpp_int = 100;
//
//	lua_State* L = lua_open();
//	luaopen_base(L);
//	lua_tinker::set(L, "cpp_int", cpp_int);
//
//	//lua_tinker::dofile(L, "sample2.lua");
//	lua_tinker::dostring(L,"print('cpp_int = '..cpp_int)");
//	lua_tinker::dostring(L,"lua_int = 200");
//	//lua_tinker::dofile(L, "sample2.lua"); end
//
//	int lua_int = lua_tinker::get<int>(L, "lua_int");
//	printf("lua_int = %d\n", lua_int);
//	lua_close(L);
//}
//
////////////////////////////////////////////////////////////////////////////
//
//namespace sample3
//{
//	//c++的面向对象及table的封装
//	const char *sample3_lua = "\
//print(g_test._test)\
//print(g_test:is_test())\
//print(g_test:ret_int())\
//temp = test(4)\
//print(temp._test)\
//\
//a = g_test:get()\
//temp:set(a)\
//print(temp._test)\
//print(temp:is_base())\
//print(temp:is_test())\
//\
//function objinfo(obj)\
//	local meta = getmetatable(obj)\
//	if meta ~= nil then\
//		metainfo(meta)\
//	else\
//		print('no object infomation !!')\
//	end\
//end\
//\
//function metainfo(meta)\
//	if meta ~= nil then\
//		local name = meta['__name']\
//		if name ~= nil then\
//			metainfo(meta['__parent'])\
//			print('<'..name..'>')\
//			for key,value in pairs(meta) do \
//				if not string.find(key, '__..') then \
//					if type(value) == 'function' then\
//						print('\t[f] '..name..':'..key..'()') \
//					elseif type(value) == 'userdata' then\
//						print('\t[v] '..name..':'..key)\
//					end\
//				end\
//			end\
//		end\
//	end\
//end\
//-------------------------------------------------------------------------------\
//\
//print('g_test	-> ', g_test)\
//print('temp	-> ', temp)\
//print('a	-> ', a)\
//\
//print('objinfo(g_test)')\
//objinfo(g_test)\
//\
//print('objinfo(temp)')\
//objinfo(temp)\
//\
//print('objinfo(a)')\
//objinfo(a)\
//\
//	";
//
//	struct A
//	{
//		A(int v) : value(v) {}
//		int value;
//	};
//
//	struct base
//	{
//		base() {}
//
//		const char* is_base(){ return "this is base"; }
//	};
//
//	class test : public base
//	{
//	public:
//		test(int val) : _test(val) {}
//		~test() {}
//
//		const char* is_test(){ return "this is test"; }
//
//		void ret_void() {}
//		int ret_int()				{ return _test;			}
//		int ret_mul(int m) const	{ return _test * m;		}
//		A get()						{ return A(_test);		}
//		void set(A a)				{ _test = a.value;		}
//
//		int _test;
//	};
//
//	test g_test(11);
//
//	TEST(lua_helper,sample3)
//	{		
//		lua_State* L = lua_open();
//		luaopen_base(L);		
//		luaopen_string(L);
//		
//		lua_tinker::class_add<base>(L, "base");
//		lua_tinker::class_def<base>(L, "is_base", &base::is_base);
//		
//
//		lua_tinker::class_add<test>(L, "test");
//		lua_tinker::class_inh<test, base>(L);				//继承
//		lua_tinker::class_con<test>(L, lua_tinker::constructor<test,int>);		//构造函数
//
//		lua_tinker::class_def<test>(L, "is_test", &test::is_test);
//		lua_tinker::class_def<test>(L, "ret_void", &test::ret_void);
//		lua_tinker::class_def<test>(L, "ret_int", &test::ret_int);
//		lua_tinker::class_def<test>(L, "ret_mul", &test::ret_mul);
//		lua_tinker::class_def<test>(L, "get", &test::get);
//		lua_tinker::class_def<test>(L, "set", &test::set);
//		lua_tinker::class_mem<test>(L, "_test", &test::_test);
//		
//		lua_tinker::set(L, "g_test", &g_test);
//
//		//lua_tinker::dofile(L, "sample3.lua");
//		lua_tinker::dostring(L, sample3_lua);
//		
//		lua_close(L);
//	}
//}
//
//namespace sample4
//{
//	//C++跟lua的交互
//	const char* sample4_lua = "\
//							  print(haha)\
//							  print(haha.value)\
//							  print(haha.inside)\
//							  print(haha.inside.value)\
//							  haha.test = 'input from lua'\
//							  \
//							  function print_table(arg)\
//								  print('arg = ', arg)\
//								  print('arg.name = ', arg.name)\
//							  end\
//							  \
//							  function return_table(arg)\
//								  local ret = {}\
//								  ret.name = arg\
//								  return ret\
//							  end\
//	";
//
//	TEST(lua_helper,sample4)
//	{
//		
//		lua_State* L = lua_open();
//		luaopen_base(L);
//
//		lua_tinker::table haha(L, "haha");
//		haha.m_obj->inc_ref();				//如果没有应用计数+1，析构后小于0，无法delete
//
//		haha.set("value", 1);
//		haha.set("inside", lua_tinker::table(L));
//		lua_tinker::table inside = haha.get<lua_tinker::table>("inside");
//		inside.set("value", 2);
//
//		//lua_tinker::dofile(L, "sample4.lua");
//		lua_tinker::dostring(L,sample4_lua);
//
//		const char* test = haha.get<const char*>("test");
//		printf("haha.test = %s\n", test);
//
//		lua_tinker::table temp(L);
//		temp.set("name", "local table !!");
//		lua_tinker::call<void>(L, "print_table", temp);
//
//		lua_tinker::table ret = lua_tinker::call<lua_tinker::table>(L, "return_table", "give me a table !!");
//		printf("ret.name =\t%s\n", ret.get<const char*>("name"));
//
//		lua_close(L);
//	}
//}
//
//namespace sample5
//{
//	void show_error(const char* error)
//	{
//		printf("_ALERT -> %s\n", error);
//	}
//
//	const char* sample5_lua = "\
//function test_error()\
//	print('test_error() called !!')	\
//	test_error_1()\
//end\
//\
//function test_error_1()\
//	print('test_error_1() called !!')	\
//	test_error_2()\
//end\
//\
//function test_error_2()\
//	print('test_error_2() called !!')	\
//	test_error_3()\
//end\
//		";
//
//	TEST(lua_helper,sample5)
//	{
//		lua_State* L = lua_open();
//		luaopen_base(L);
//
//		printf("%s\n","-------------------------- current stack");
//		lua_tinker::enum_stack(L);
//
//		lua_pushnumber(L, 1);
//		printf("%s\n","-------------------------- stack after push '1'");
//		lua_tinker::enum_stack(L);
//
//		//lua_tinker::dofile(L, "sample5.lua");
//		lua_tinker::dostring(L,sample5_lua);
//
//		printf("%s\n","-------------------------- calling test_error()");
//		lua_tinker::call<void>(L, "test_error");
//
//		printf("%s\n","-------------------------- calling test_error_3()");
//		lua_tinker::call<void>(L, "test_error_3");
//
//		lua_tinker::def(L, "_ALERT", show_error);
//
//		lua_tinker::call<void>(L, "_ALERT", "test !!!");
//
//		printf("%s\n","-------------------------- calling test_error()");
//		lua_tinker::call<void>(L, "test_error");
//
//		lua_close(L);
//	}
//}

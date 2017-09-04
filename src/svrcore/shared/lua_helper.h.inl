

/************************************************************************/
/* 提供给脚本使用的系统及函数                                           */
/************************************************************************/

template<typename APP>
int __outString(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	/* get number of arguments */
	int n = lua_gettop(formulaL);
	ASSERT(n == 1);	//暂不支持日志多参数输出
	const char *str = LUA_TOSTRING(formulaL, 1);
	__out_file(APP::g_app->get_logger(),LOGLEVEL_INFO,str);
	return 0;
}

template<typename APP>
int __outDebug(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	/* get number of arguments */
	int n = lua_gettop(formulaL);
	ASSERT(n == 1);	//暂不支持日志多参数输出
	const char *str = LUA_TOSTRING(formulaL, 1);
	__out_file(APP::g_app->get_logger(),LOGLEVEL_DEBUG,str);
	return 0;
}

template<typename APP>
int __outError(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	/* get number of arguments */
	int n = lua_gettop(formulaL);
	ASSERT(n == 1);	//暂不支持日志多参数输出
	const char *str = LUA_TOSTRING(formulaL, 1);
	__out_file(APP::g_app->get_logger(),LOGLEVEL_ERROR,str);
	return 0;
}



//////////////////////////////////////////////////////////////////////////
//for lua script BINLOG

template<typename BINLOG_T>
int LuaGetBit(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetBit obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint16 offset = (uint16)LUA_TOINTEGER(scriptL, 3);
	lua_pushboolean(scriptL, obj->GetBit(index, offset));
	return 1;
}


template<typename BINLOG_T>
int LuaSetBit(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetBit obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	obj->SetBit(index, offset);
	return 0;
}

template<typename BINLOG_T>
int LuaUnSetBit(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetBit obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	obj->UnSetBit(index, offset);
	return 0;
}

template<typename BINLOG_T>
int LuaGetByte(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetByte obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	lua_pushinteger(scriptL, obj->GetByte(index, offset));
	return 1;
}

template<typename BINLOG_T>
int LuaAddByte(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetByte obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	uint8 val = (uint8)LUA_TOINTEGER(scriptL, 4);
	obj->AddByte(index, offset, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSubByte(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetByte obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	uint8 val = (uint8)LUA_TOINTEGER(scriptL, 4);
	obj->SubByte(index, offset, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSetByte(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetByte obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	uint8 val = (uint8)LUA_TOINTEGER(scriptL, 4);
	obj->SetByte(index, offset, val);
	return 0;
}

template<typename BINLOG_T>
int LuaGetUInt16(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt16 obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	lua_pushinteger(scriptL, obj->GetUInt16(index, offset));
	return 1;
}

template<typename BINLOG_T>
int LuaAddUInt16(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt16 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	uint16 val = (uint16)LUA_TOINTEGER(scriptL, 4);
	obj->AddUInt16(index, offset, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSubUInt16(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt16 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	uint16 val = (uint16)LUA_TOINTEGER(scriptL, 4);
	obj->SubUInt16(index, offset, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSetUInt16(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt16 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	uint16 val = (uint16)LUA_TOINTEGER(scriptL, 4);
	obj->SetUInt16(index, offset, val);
	return 0;
}

//TODO: FIXME this
template<typename BINLOG_T>
int LuaSetInt16(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt16 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint8 offset = (uint8)LUA_TOINTEGER(scriptL, 3);
	uint16 val = (uint16)LUA_TOINTEGER(scriptL, 4);
	obj->SetInt16(index, offset, val);
	return 0;
}

template<typename BINLOG_T>
int LuaGetUInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	lua_pushinteger(scriptL, obj->GetUInt32(index));
	return 1;
}

template<typename BINLOG_T>
int LuaAddUInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint32 val = (uint32)LUA_TOINTEGER(scriptL, 3);
	obj->AddUInt32(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSubUInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint32 val = (uint32)LUA_TOINTEGER(scriptL, 3);
	obj->SubUInt32(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSetUInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	uint32 val = (uint32)LUA_TOINTEGER(scriptL, 3);
	obj->SetUInt32(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaGetInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	lua_pushinteger(scriptL, obj->GetInt32(index));
	return 1;
}

template<typename BINLOG_T>
int LuaAddInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	int32 val = (int32)LUA_TOINTEGER(scriptL, 3);
	obj->AddInt32(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSubInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	int32 val = (int32)LUA_TOINTEGER(scriptL, 3);
	obj->SubInt32(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSetInt32(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	int32 val = (int32)LUA_TOINTEGER(scriptL, 3);
	obj->SetInt32(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaGetDouble(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		lua_pushnumber(scriptL, 0);
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	lua_pushnumber(scriptL, obj->GetDouble(index));
	return 1;
}

template<typename BINLOG_T>
int LuaAddDouble(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	double val = LUA_TONUMBER(scriptL, 3);
	obj->AddDouble(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSubDouble(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	double val = LUA_TONUMBER(scriptL, 3);
	obj->SubDouble(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaSetDouble(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	double val = LUA_TONUMBER(scriptL, 3);
	obj->SetDouble(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaGetFloat(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	lua_pushnumber(scriptL, obj->GetFloat(index));
	return 1;
}

template<typename BINLOG_T>
int LuaSetFloat(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	float val = (float)LUA_TONUMBER(scriptL, 3);
	obj->SetFloat(index, val);
	return 0;
}

template<typename BINLOG_T>
int LuaGetStr(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		lua_pushstring(scriptL, "");
		return 1;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	lua_pushstring(scriptL, obj->GetStr(index).c_str());
	return 1;
}

template<typename BINLOG_T>
int LuaSetStr(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		return 0;
	}
	uint32 index = (uint32)LUA_TOINTEGER(scriptL, 2);
	string str = LUA_TOSTRING(scriptL, 3);
	ASSERT(index != 3 && index != 0);
	obj->SetStr(index, str.c_str());
	return 0;
}

template<typename BINLOG_T>
int LuaSync(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		return 0;
	}
	obj->sync();
	return 0;
}

template<typename BINLOG_T>
int LuaGetUInt32Length(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	lua_pushinteger(scriptL, obj->length_uint32());
	return 1;
}

template<typename BINLOG_T>
int LuaGetStrLength(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaGetUInt32 obj is null");
		lua_pushinteger(scriptL, 0);
		return 1;
	}
	lua_pushinteger(scriptL, obj->length_str());
	return 1;
}

template<typename BINLOG_T>
int LuaSetBinlogMaxSize(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	BINLOG_T *obj = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	if(!obj)
	{
		//tea_perror("BinLogObject::LuaSetBinlogMaxSize obj is null");
		return 0;
	}
	uint32 size = (uint32)LUA_TOINTEGER(scriptL, 2);
	obj->SetBinlogMaxSize(size);
	return 0;
}

//binlog数据转换成字符串
template<typename BINLOG_T>
int LuaToString(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	BINLOG_T* binlog = (BINLOG_T*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeNone);
	string data, data_str;
	if (binlog)
	{
		binlog->ToString(data, data_str);
	}
	lua_pushstring(scriptL, data.c_str());
	lua_pushstring(scriptL, data_str.c_str());

	return 2;
}

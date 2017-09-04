#include "robotd_unit.h"
#include "robotd_context.h"
#include <object/SharedDef.h>

void RobotdUnit::OnAfterUpdate(SyncEventRecorder *data,int flags,UpdateMask& mask,UpdateMask& str_mask)
{
	RobotdApp::g_app->StatUpdateIndex(data, mask, mask_string_);

	//ASSERT((flags & OBJ_OPT_NEW) || !mask.GetBit(UNIT_FIELD_UINT16_1));

	//监听到移动速度下标变化时重算一下移动时间
	if (mask.GetBit(UNIT_FIELD_BYTE_0))
	{
		if (!this->hasCreated) {
			this->hasCreated = true;
			this->StartMoving(m_client_path, "OnAfterUpdate");
		}
		// 玩家如果复活
		if (this->GetByte(UNIT_FIELD_BYTE_0, 1) == DEATH_STATE_ALIVE) {
			this->m_main_player->OnRobotRespawn();
		}
	}
}

void RobotdUnit::ReadOffsetPath(ByteArray& bytes)
{
	uint16 pos_x = bytes.readShort();
	uint16 pos_y = bytes.readShort();
	vector<char> ofpath;
	bytes >> ofpath;
	
	ASSERT(ofpath.size() %2 == 0);
	
	this->SetPosition(pos_x,pos_y, "ReadOffsetPath");
	m_moving_path.clear();
	for (uint16 i = 0; i < ofpath.size(); i+=2)
	{
		m_moving_path.push_back(pos_x +=ofpath[i]);
		m_moving_path.push_back(pos_y +=ofpath[i+1]);
	}

	this->StartMoving(false, "ReadOffsetPath");
}

void RobotdUnit::StopMoving(uint16 x, uint16 y)
{
	m_moving_path.clear();
	SetPosition((float)x, (float)y, "StopMoving");
	m_moving_ms = 0;
	m_moving_last_diff = 0;
}

#define CHECK_PATH_DEF 0
void RobotdUnit::StartMoving(bool client_path, string from)
{
	m_client_path = client_path;

	if (m_moving_path.size() < 2)
	{
		m_moving_ms = 0;
		m_moving_last_diff = 0;
		return;
	}

	float to_x = 0.f;
	float to_y = 0.f;

	/**
	bool fa = false;
	string s_path;
	for (uint32 i = 0; i < m_moving_path.size(); ++ (++ i)) {
		char dd[49];
		sprintf(dd, "(%.2f, %.2f) ", m_moving_path[ i ], m_moving_path[i+1]);
		s_path += dd;
		if (m_moving_path[ i ] < 1 || m_moving_path[i+1] < 1) {
			fa = true;
		}
	}
	
	if (fa) {
		tea_pinfo("%s StartMoving from (%d, %d) to %s", this->GetGuid().c_str(), (uint16)this->GetPosX(), (uint16)this->GetPosY(), s_path.c_str());
		return;
	}
	*/

	//计算前两点
/*
	if (m_moving_path.size() <= 2)
	{
/ *
		to_x = m_moving_path[0];
		to_y = m_moving_path[1];* /
		m_moving_ms = 0;
		m_moving_last_diff = 0;
		return;
	}
	else*/
	{
		to_x = m_moving_path[0];
		to_y = m_moving_path[1];
	}

	float angle = GetAngle(to_x, to_y);
	float range = GetDistance(to_x, to_y);
	m_orientation = angle;
	m_moving_ms = uint32(range * 10 * 1000 / GetMoveSpeed());
	m_moving_last_diff = 0;

	//tea_pdebug("from %s--- %s startmoving to (%.2f, %.2f)\n", from.c_str(), this->GetGuid().c_str(), to_x, to_y);
}

void RobotdUnit::SetPosition(float x, float y, string from)
{
	//tea_pdebug("%s from %s set x, y (%.2f, %.2f)\n", this->GetGuid().c_str(), from.c_str(), x, y);
	ASSERT(x > 0 && y > 0);
#ifdef CHECK_PATH_DEF
	if(m_main_player)
	{
		const MapTemplate *templ = MapTemplate::GetMapTempalte(GetMapId());
		//ASSERT(x <= templ->GetMapBaseInfo().width);
		//ASSERT(y <= templ->GetMapBaseInfo().height);
		if(x > templ->GetMapBaseInfo().width || y > templ->GetMapBaseInfo().height)
		{
			tea_perror("RobotdUnit::SetPosition check pos err, %s %s %u %u %u", m_main_player->guid().c_str(), guid().c_str(), GetMapId(), x, y);
			m_main_player->Close();
		}
	}
#endif
	m_positionX = x;
	m_positionY = y;
}

// Return angle in range 0..2*pi
float RobotdUnit::GetAngle( const float x, const float y ) const
{
	float dx = x - m_positionX;
	float dy = y - m_positionY;

	float ang = atan2(dy, dx);
	ang = (ang >= 0) ? ang : 2 * M_PI + ang;
	return ang;
}

float RobotdUnit::GetDistance(const float x,const float y) const
{
	float dx = x - m_positionX;
	float dy = y - m_positionY;
	return (float)sqrt(dx*dx + dy*dy);
}

bool RobotdUnit::ReadFrom(int flags,ByteArray& bytes)
{
	ASSERT(SyncEventRecorder::ReadFrom(flags, bytes));
	if(flags & SyncEventRecorder::OBJ_OPT_NEW)
	{
		m_moving_path.clear();
		int16_t x = bytes.readShort();
		int16_t y = bytes.readShort();
		int16_t last_x = x;
		int16_t last_y = y;
		m_moving_path.push_back(last_x);
		m_moving_path.push_back(last_y);

		int16_t len = bytes.readShort();
		for(int16_t i=0; i<len; i+=2)
		{
			last_x += bytes.readByte();
			last_y += bytes.readByte();
			m_moving_path.push_back(last_x);
			m_moving_path.push_back(last_y);
		}

		float ori = bytes.readFloat();
		this->SetPosition(x,y, "ReadFrom");
		if (!this->hasCreated) {
			this->hasCreated = true;
			this->StartMoving(false, "ReadFrom");
		}
	}
	return true;
}

//#define PRINT_LOCATE 1

bool RobotdUnit::UpdateLocate(uint32 diff)
{
	//非移动状态
	if(!IsMoving())
		return false;
	float x = 0.0f, y = 0.0f;

	//累计时间
	m_moving_last_diff += diff;
	//tea_pdebug("RobotdUnit::UpdateLocate end %s %.2f %.2f %u %u speed:%u"
	//	, guid_.c_str(), m_positionX, m_positionY, m_moving_ms, m_moving_last_diff,GetMoveSpeed());
	//如果已经超过点了
	if(m_moving_ms <= m_moving_last_diff)
	{
		//设置为新坐标并弹出点
		SetPosition(m_moving_path[0], m_moving_path[1], "UpdateLocate");
		m_moving_path.pop_front();
		m_moving_path.pop_front();

#ifdef PRINT_LOCATE
		if(m_moving_path.size() == 0)
		{
			tea_pdebug("RobotdUnit::UpdateLocate end %s %.2f %.2f %u %u speed:%u"
				, guid_.c_str(), m_positionX, m_positionY, m_moving_ms, m_moving_last_diff,GetMoveSpeed());
		}
		else
		{
			tea_pdebug("RobotdUnit::UpdateLocate pop point %s %.2f %.2f %.0f %.0f %u %u speed:%u"
				, guid_.c_str(), m_positionX, m_positionY, m_moving_path[0], m_moving_path[1], m_moving_ms, m_moving_last_diff,GetMoveSpeed());
		}
#endif // PRINT_LOCATE

		if (m_moving_path.size() == 0)		//到达最后的点
		{
			//已经到达目标了
			m_moving_ms = 0;
			m_moving_last_diff = 0;
			////发送移动停止，与服务端同步坐标
			//if(IsMainPlayer())
			//	m_main_player->send_move_stop(GetUIntGuid(), uint16(m_positionX), uint16(m_positionY));
		}
		else
		{
			//计算一下上一点消耗了多少时间
			m_moving_last_diff -= m_moving_ms;

			//计算下一个点需要消耗的时间
			float angle = GetAngle(m_moving_path[0], m_moving_path[1]);
			float dist = GetDistance(m_moving_path[0], m_moving_path[1]);
			m_orientation = angle;
			m_moving_ms = uint32(dist * 10 * 1000 / GetMoveSpeed());			
			if(!m_moving_ms)
				m_moving_ms=1;
		}        
	}
	else
	{
		m_moving_ms -= m_moving_last_diff;

		//刷新坐标
		float range = float(m_moving_last_diff) * float(GetMoveSpeed()) / 1000 / 10;
		float distX = range * cos(m_orientation);
		float distY = range * sin(m_orientation);

		x = m_positionX + distX;
		y = m_positionY + distY;

		SetPosition(x, y, "UpdateLocate");

#ifdef PRINT_LOCATE
		//tea_pdebug("RobotdUnit::UpdateLocate %s %.2f %.2f %.0f %.0f %u %u speed:%u"
		//	, guid_.c_str(), x, y, m_moving_path[0], m_moving_path[1], m_moving_ms, m_moving_last_diff,GetMoveSpeed());
		tea_pdebug("%u,%u 原坐标(%f,%f) 新坐标(%f,%f),ori:%f speed:%u",m_moving_last_diff,m_moving_ms, m_positionX, m_positionY, x, y,m_orientation,GetMoveSpeed());
#endif		
		m_moving_last_diff = 0;
	}
	return true;
}


//获取坐标
int RobotdUnit::LuaGetPos(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	RobotdUnit *unit = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeUnit);
	ASSERT(unit);
	lua_pushinteger(scriptL, (int)unit->GetPosX());
	lua_pushinteger(scriptL, (int)unit->GetPosY());
	return 2;
}

//通过GUID获取玩家实例
int RobotdUnit::LuaSetPos(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	RobotdUnit *unit = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeUnit);
	ASSERT(unit);
	float x = (float)LUA_TONUMBER(scriptL, 2);
	float y = (float)LUA_TONUMBER(scriptL, 3);
	unit->SetPosition(x,y,"LuaSetPos");
	return 0;
}

//获取两个unit之间的距离
int RobotdUnit::LuaGetDistance(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 2);
	RobotdUnit *unit1 = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeUnit);
	RobotdUnit *unit2 = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 2, ObjectTypeUnit);
	ASSERT(unit1 && unit2);
	lua_pushnumber(scriptL, unit1->GetDistance(unit2));
	return 1;
}

//是否移动状态
int RobotdUnit::LuaIsMoving(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	RobotdUnit *unit = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeUnit);
	ASSERT(unit);
	lua_pushboolean(scriptL, unit->IsMoving() ? TRUE : FALSE);
	return 1;
}

//停止移动
int RobotdUnit::LuaStopMoving(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	RobotdUnit *unit = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeUnit);
	ASSERT(unit);
	uint16 x = (uint16)LUA_TONUMBER(scriptL, 2);
	uint16 y = (uint16)LUA_TONUMBER(scriptL, 3);
	unit->StopMoving(x,y);
	return 0;
}

//获取unit与坐标点之间的距离
int RobotdUnit::LuaGetDistanceByPos(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 3);
	RobotdUnit *unit = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 1, ObjectTypeUnit);
	ASSERT(unit);
	float x = (float)LUA_TONUMBER(scriptL, 2);
	float y = (float)LUA_TONUMBER(scriptL, 3);
	lua_pushnumber(scriptL, unit->GetDistance(x,y));
	return 1;
}

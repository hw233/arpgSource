#include "PlayerBase.h"
#include "shared/lua_helper.h"

//////////////////////////////////////////////////////////////////////////
//初始化
PlayerBase::PlayerBase(core_obj::SyncMode _m):BinLogObject(_m)
{	
}

PlayerBase::~PlayerBase()
{
}

//启用时装
void PlayerBase::SetUseFashion(uint16 pos)
{
	if(GetBit(PLAYER_EXPAND_INT_USE_FASHION, pos))
	{
		UnSetBit(PLAYER_EXPAND_INT_USE_FASHION, pos);
	}
	else
	{
		SetBit(PLAYER_EXPAND_INT_USE_FASHION, pos);
	}
}

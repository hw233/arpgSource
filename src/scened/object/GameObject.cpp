#include "svrcore-internal.h"
#include "GameObject.h"
#include <shared/storage_loader.h>

//////////////////////////////////////////////////////////////////////////
//for GameObject
//uint32 GameObject::m_new_gameobj_id = 0;
//GAMEOBJ_GUID_MAP GameObject::GameObjGuidMap;

GameObject::GameObject():m_is_garbage(false)
{
}

GameObject::~GameObject()
{

}

bool GameObject::Create(const string &lguid, uint32 entry)
{
	//传送点
	const gameobject_template* tpl = gameobject_template_db[entry];
	if(!tpl) return false;

	Unit::_Create(lguid, entry);
	SetTypeId(TYPEID_GAMEOBJECT);
	//设置名称
	SetName(tpl->name);
	SetUInt32(GO_FIELD_FLAGS,tpl->go_flag);
	m_respawn_time = 5;

	return true;
}

void GameObject::UpdateLiveStatus(uint32 diff)
{
	//重生定时器
	if(isAlive())
		return;
	m_live_timer.Update(diff);

	//定时器未到
	if (!m_live_timer.Passed())
		return;

	//尸体状态下变形为死亡状态
	if(GetDeathState() == DEATH_STATE_CORPSE) {
		SetDeathState(DEATH_STATE_DEAD);
		m_live_timer.Reset(m_respawn_time*1000);
	}
	else if(GetDeathState() == DEATH_STATE_DEAD)	//复活
	{
		Respawn();
		m_live_timer.Reset(0);
	}
}

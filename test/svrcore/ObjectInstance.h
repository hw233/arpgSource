//#include <object/Object.h>
//#include <object/UpdateFields.h>
//class ObjectInstance:public Object
//{
//public:
//	ObjectInstance()
//	{
//		m_objectType |= TYPEMASK_PLAYER;
//		m_objectTypeId = TYPEID_PLAYER;
//
//		m_valuesCount = MAX_PLAYER_FIELD;
//	}
//
//	void Create (uint32 guidlow, const std::string& name,uint8 faction,uint8 gender,uint8 head_id,uint8 hair_id)
//	{
//		//创建基本属性
//		Object::_Create(guidlow,2,HIGHGUID_PLAYER);
//		SetName(name);      
//	}
//
//	void SetHealth(uint32 val)
//	{
//		uint32 maxHealth = GetMaxHealth();
//
//		if(maxHealth < val)
//			val = maxHealth;
//
//		SetUInt32(UNIT_FIELD_HEALTH, val);
//	}
//
//	void SetMaxHealth(uint32 val)
//	{
//		uint32 health = GetHealth();
//		SetUInt32(UNIT_FIELD_MAXHEALTH, val);
//
//		if(val < health)
//			SetHealth(val);
//	}
//
//	uint32 GetHealth()    const { return GetUInt32(UNIT_FIELD_HEALTH); }
//	uint32 GetMaxHealth() const { return GetUInt32(UNIT_FIELD_MAXHEALTH); }
//};
//

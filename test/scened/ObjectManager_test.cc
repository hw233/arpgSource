#include <gtest/gtest.h>
#include <svrcore.h>
#include "ObjectManager.h"
#include <object/UpdateBlock.h>

TEST(ObjectManager,CreateObject)
{
	ObjectManager obj;
	uint64 guid = MAKE_NEW_GUID(123, 1, HIGHGUID_PLAYER);
	Player *player = static_cast<Player*>(obj.CreateObject(guid));
	ASSERT_EQ(player->GetGUIDLow(),123);
}

TEST(ObjectManager,AppendUpdateBlock)
{
	ObjectManager obj;
	uint64 guid = MAKE_NEW_GUID(123, 1, HIGHGUID_PLAYER);
	Player *player = static_cast<Player*>(obj.CreateObject(guid));

	CreateBlock block;
	block.SetObject(player,player->GetGUIDLow());
	
	packet *pkt = NULL;
	packet_init(&pkt);
	
	block >> *pkt;

	//从包中再读取该对象
	ObjectManager obj2;
	Player *player2 = obj2.AppendUpdateBlock(*pkt);

	ASSERT_EQ(player->GetGUID(),player2->GetGUID());

	packet_free(pkt);
}



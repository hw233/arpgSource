#include <gtest/gtest.h>
#include <comm/common.h>
#include <net/netstd.h>
#include <svrcore.h>
#include <protocol/external.h>

TEST(external_protocol_test,new_free)
{
	svrcore_init();
	for (int i = 0; i < uint16(-1); i++)
	{
		packet *pkt = external_protocol_new_packet(i);
		ASSERT_EQ(pkt->head->optcode,i);
		external_protocol_free_packet(pkt);
	}	
	svrcore_free();
}

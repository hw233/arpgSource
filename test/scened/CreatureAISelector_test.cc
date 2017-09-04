#include <gtest/gtest.h>
#include <comm/common_stl.h>
#include "FactoryHolder.h"
#include "CreatureAI.h"
#include "Creature.h"

TEST(CreatureAISelector,CreatureAISelector)
{	

	creature_template_vec.resize(10);
	creature_template_vec[2].id = 2;
	strcpy(creature_template_vec[2].ainame , "NullCreatureAI");

	Tea::InitializeAI();

	Creature c;
	c.Create(1,(Map*)0,2);	

	CreatureAI *ai = Tea::SelectAI(&c);
	delete ai;
}

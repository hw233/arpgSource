#include <gtest/gtest.h>
#include "scened.h"
#include "Grid.h"
#include "Player.h"
#include "Creature.h"

TEST(GridManager,GridManager)
{
	//GridManager gm(800,800);
	////验证生成的grid坐标是否有效
	//Grid& g = gm.GetGrid(0,0);
	//ASSERT_EQ(g.notice_grid[3*1+1] , &g);
	//ASSERT_EQ(g.notice_grid[3*2+2] , &gm.GetGrid(1,1)); //最后一个应该等于当前格的右下角

	//Grid& g2 = gm.GridCoord(71.3f,39.4f);
	//ASSERT_FALSE(g2.IsNotInThisGrid(71.3f,39.4f));
}

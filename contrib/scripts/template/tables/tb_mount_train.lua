---------------------------------------------------------------------------------
--------------------------以下代码为自动生成，请勿手工改动-----------------------
---------------------------------------------------------------------------------


tb_mount_train = {
	--  id:int 序号
	--  level:int 坐骑阶数
	--  star:int 坐骑星级
	--  exp:int 培养到下一级需要经验
	--  prosKeys:array 坐骑属性key
	--  prosValues:array 坐骑属性值
	--  traincost:array 培养消耗
	--  addTrainExp:int 培养增加经验
	[1] = {id = 1, level = 1, star = 0, exp = 120, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {4800,360,102,30,51,30,68,30}, traincost = {{3,3696},{227,4}}, addTrainExp = 40},
	[2] = {id = 2, level = 1, star = 1, exp = 120, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {5600,395,114,35,57,35,76,35}, traincost = {{3,3696},{227,4}}, addTrainExp = 40},
	[3] = {id = 3, level = 1, star = 2, exp = 210, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {6400,430,126,40,63,40,84,40}, traincost = {{3,6982},{227,7}}, addTrainExp = 70},
	[4] = {id = 4, level = 1, star = 3, exp = 300, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {7200,465,138,45,69,45,92,45}, traincost = {{3,10268},{227,10}}, addTrainExp = 100},
	[5] = {id = 5, level = 1, star = 4, exp = 390, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {8000,500,150,50,75,50,100,50}, traincost = {{3,13554},{227,13}}, addTrainExp = 130},
	[6] = {id = 6, level = 1, star = 5, exp = 480, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {8800,535,162,55,81,55,108,55}, traincost = {{3,16839},{227,16}}, addTrainExp = 160},
	[7] = {id = 7, level = 1, star = 6, exp = 570, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {9600,570,174,60,87,60,116,60}, traincost = {{3,20125},{227,19}}, addTrainExp = 190},
	[8] = {id = 8, level = 1, star = 7, exp = 660, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {10400,605,186,65,93,65,124,65}, traincost = {{3,23411},{227,22}}, addTrainExp = 220},
	[9] = {id = 9, level = 1, star = 8, exp = 750, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {11200,640,198,70,99,70,132,70}, traincost = {{3,26696},{227,25}}, addTrainExp = 250},
	[10] = {id = 10, level = 1, star = 9, exp = 840, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {12000,675,210,75,105,75,140,75}, traincost = {{3,29982},{227,28}}, addTrainExp = 280},
	[11] = {id = 11, level = 1, star = 10, exp = 900, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {12800,710,222,80,111,80,148,80}, traincost = {{3,33268},{227,30}}, addTrainExp = 300},
	[12] = {id = 12, level = 2, star = 0, exp = 160, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {13600,745,234,85,117,85,156,85}, traincost = {{3,5858},{227,4}}, addTrainExp = 40},
	[13] = {id = 13, level = 2, star = 1, exp = 160, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {14400,780,246,90,123,90,164,90}, traincost = {{3,5858},{227,4}}, addTrainExp = 40},
	[14] = {id = 14, level = 2, star = 2, exp = 320, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {15200,815,258,95,129,95,172,95}, traincost = {{3,11066},{227,8}}, addTrainExp = 80},
	[15] = {id = 15, level = 2, star = 3, exp = 440, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {16000,850,270,100,135,100,180,100}, traincost = {{3,16273},{227,11}}, addTrainExp = 110},
	[16] = {id = 16, level = 2, star = 4, exp = 560, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {16800,885,282,105,141,105,188,105}, traincost = {{3,21481},{227,14}}, addTrainExp = 140},
	[17] = {id = 17, level = 2, star = 5, exp = 720, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {17600,920,294,110,147,110,196,110}, traincost = {{3,26688},{227,18}}, addTrainExp = 180},
	[18] = {id = 18, level = 2, star = 6, exp = 840, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {18400,955,306,115,153,115,204,115}, traincost = {{3,31896},{227,21}}, addTrainExp = 210},
	[19] = {id = 19, level = 2, star = 7, exp = 960, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {19200,990,318,120,159,120,212,120}, traincost = {{3,37103},{227,24}}, addTrainExp = 240},
	[20] = {id = 20, level = 2, star = 8, exp = 1120, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {20000,1025,330,125,165,125,220,125}, traincost = {{3,42310},{227,28}}, addTrainExp = 280},
	[21] = {id = 21, level = 2, star = 9, exp = 1240, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {20800,1060,342,130,171,130,228,130}, traincost = {{3,47518},{227,31}}, addTrainExp = 310},
	[22] = {id = 22, level = 2, star = 10, exp = 1360, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {21600,1095,354,135,177,135,236,135}, traincost = {{3,52725},{227,34}}, addTrainExp = 340},
	[23] = {id = 23, level = 3, star = 0, exp = 200, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {22400,1130,366,140,183,140,244,140}, traincost = {{3,22673},{227,4}}, addTrainExp = 40},
	[24] = {id = 24, level = 3, star = 1, exp = 200, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {23200,1165,378,145,189,145,252,145}, traincost = {{3,22673},{227,4}}, addTrainExp = 40},
	[25] = {id = 25, level = 3, star = 2, exp = 400, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {24000,1200,390,150,195,150,260,150}, traincost = {{3,42827},{227,8}}, addTrainExp = 80},
	[26] = {id = 26, level = 3, star = 3, exp = 600, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {24800,1235,402,155,201,155,268,155}, traincost = {{3,62980},{227,12}}, addTrainExp = 120},
	[27] = {id = 27, level = 3, star = 4, exp = 750, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {25600,1270,414,160,207,160,276,160}, traincost = {{3,83134},{227,15}}, addTrainExp = 150},
	[28] = {id = 28, level = 3, star = 5, exp = 950, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {26400,1305,426,165,213,165,284,165}, traincost = {{3,103288},{227,19}}, addTrainExp = 190},
	[29] = {id = 29, level = 3, star = 6, exp = 1100, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {27200,1340,438,170,219,170,292,170}, traincost = {{3,123441},{227,22}}, addTrainExp = 220},
	[30] = {id = 30, level = 3, star = 7, exp = 1300, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {28000,1375,450,175,225,175,300,175}, traincost = {{3,143595},{227,26}}, addTrainExp = 260},
	[31] = {id = 31, level = 3, star = 8, exp = 1450, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {28800,1410,462,180,231,180,308,180}, traincost = {{3,163749},{227,29}}, addTrainExp = 290},
	[32] = {id = 32, level = 3, star = 9, exp = 1650, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {29600,1445,474,185,237,185,316,185}, traincost = {{3,183903},{227,33}}, addTrainExp = 330},
	[33] = {id = 33, level = 3, star = 10, exp = 1800, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {30400,1480,486,190,243,190,324,190}, traincost = {{3,204056},{227,36}}, addTrainExp = 360},
	[34] = {id = 34, level = 4, star = 0, exp = 300, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {31200,1515,498,195,249,195,332,195}, traincost = {{3,339849},{227,5}}, addTrainExp = 50},
	[35] = {id = 35, level = 4, star = 1, exp = 300, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {32000,1550,510,200,255,200,340,200}, traincost = {{3,339849},{227,5}}, addTrainExp = 50},
	[36] = {id = 36, level = 4, star = 2, exp = 480, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {32800,1585,522,205,261,205,348,205}, traincost = {{3,641937},{227,8}}, addTrainExp = 80},
	[37] = {id = 37, level = 4, star = 3, exp = 720, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {33600,1620,534,210,267,210,356,210}, traincost = {{3,944025},{227,12}}, addTrainExp = 120},
	[38] = {id = 38, level = 4, star = 4, exp = 960, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {34400,1655,546,215,273,215,364,215}, traincost = {{3,1246112},{227,16}}, addTrainExp = 160},
	[39] = {id = 39, level = 4, star = 5, exp = 1140, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {35200,1690,558,220,279,220,372,220}, traincost = {{3,1548200},{227,19}}, addTrainExp = 190},
	[40] = {id = 40, level = 4, star = 6, exp = 1380, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {36000,1725,570,225,285,225,380,225}, traincost = {{3,1850288},{227,23}}, addTrainExp = 230},
	[41] = {id = 41, level = 4, star = 7, exp = 1620, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {36800,1760,582,230,291,230,388,230}, traincost = {{3,2152376},{227,27}}, addTrainExp = 270},
	[42] = {id = 42, level = 4, star = 8, exp = 1860, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {37600,1795,594,235,297,235,396,235}, traincost = {{3,2454464},{227,31}}, addTrainExp = 310},
	[43] = {id = 43, level = 4, star = 9, exp = 2040, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {38400,1830,606,240,303,240,404,240}, traincost = {{3,2756552},{227,34}}, addTrainExp = 340},
	[44] = {id = 44, level = 4, star = 10, exp = 2280, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {39200,1865,618,245,309,245,412,245}, traincost = {{3,3058640},{227,38}}, addTrainExp = 380},
	[45] = {id = 45, level = 5, star = 0, exp = 350, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {40000,1900,630,250,315,250,420,250}, traincost = {{3,2034511},{227,5}}, addTrainExp = 50},
	[46] = {id = 46, level = 5, star = 1, exp = 350, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {40800,1935,642,255,321,255,428,255}, traincost = {{3,2034511},{227,5}}, addTrainExp = 50},
	[47] = {id = 47, level = 5, star = 2, exp = 630, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {41600,1970,654,260,327,260,436,260}, traincost = {{3,3842965},{227,9}}, addTrainExp = 90},
	[48] = {id = 48, level = 5, star = 3, exp = 840, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {42400,2005,666,265,333,265,444,265}, traincost = {{3,5651419},{227,12}}, addTrainExp = 120},
	[49] = {id = 49, level = 5, star = 4, exp = 1120, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {43200,2040,678,270,339,270,452,270}, traincost = {{3,7459873},{227,16}}, addTrainExp = 160},
	[50] = {id = 50, level = 5, star = 5, exp = 1400, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {44000,2075,690,275,345,275,460,275}, traincost = {{3,9268327},{227,20}}, addTrainExp = 200},
	[51] = {id = 51, level = 5, star = 6, exp = 1680, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {44800,2110,702,280,351,280,468,280}, traincost = {{3,11076782},{227,24}}, addTrainExp = 240},
	[52] = {id = 52, level = 5, star = 7, exp = 1960, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {45600,2145,714,285,357,285,476,285}, traincost = {{3,12885236},{227,28}}, addTrainExp = 280},
	[53] = {id = 53, level = 5, star = 8, exp = 2170, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {46400,2180,726,290,363,290,484,290}, traincost = {{3,14693690},{227,31}}, addTrainExp = 310},
	[54] = {id = 54, level = 5, star = 9, exp = 2450, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {47200,2215,738,295,369,295,492,295}, traincost = {{3,16502144},{227,35}}, addTrainExp = 350},
	[55] = {id = 55, level = 5, star = 10, exp = 2730, prosKeys = {1,2,3,4,5,6,7,10}, prosValues = {48000,2250,750,300,375,300,500,300}, traincost = {{3,18310598},{227,39}}, addTrainExp = 390},
}

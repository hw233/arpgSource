---------------------------------------------------------------------------------
--------------------------以下代码为自动生成，请勿手工改动-----------------------
---------------------------------------------------------------------------------


tb_hongyan_map = {
	--  id:int	地点ID
	--  name:string	名称
	--  vitality:int	出游消耗活力
	--  open_condition:array	开启条件
	--  close_season:array	闭馆季节
	--  close_time:array	闭馆时间
	--  close_weather:array	闭馆天气
	--  alone_event_id:int	独自出游触发基础事件ID
	--  peri_event_id:int	通用带美人触发基础事件ID
	--  special_peri_event_id:array	特殊情况带美人触发基础事件ID（美女ID，事件……）
	[1] = {id = 1,name = "平王府",vitality = 5,open_condition = {},close_season = {},close_time = {},close_weather = {},alone_event_id = 1,peri_event_id = 28,special_peri_event_id = {{12,55}},},
	[2] = {id = 2,name = "妙乐坊",vitality = 5,open_condition = {},close_season = {},close_time = {},close_weather = {},alone_event_id = 2,peri_event_id = 29,special_peri_event_id = {},},
	[3] = {id = 3,name = "市集",vitality = 5,open_condition = {},close_season = {},close_time = {},close_weather = {},alone_event_id = 3,peri_event_id = 30,special_peri_event_id = {},},
	[4] = {id = 4,name = "校场",vitality = 5,open_condition = {},close_season = {},close_time = {},close_weather = {},alone_event_id = 4,peri_event_id = 31,special_peri_event_id = {{12,56}},},
	[5] = {id = 5,name = "有间客栈",vitality = 5,open_condition = {3,100,1000},close_season = {},close_time = {},close_weather = {},alone_event_id = 5,peri_event_id = 32,special_peri_event_id = {{22,57},{21,58},{3,59}},},
	[6] = {id = 6,name = "白鹿书院",vitality = 5,open_condition = {3,100,1000},close_season = {},close_time = {3},close_weather = {},alone_event_id = 6,peri_event_id = 33,special_peri_event_id = {},},
	[7] = {id = 7,name = "大佛寺",vitality = 5,open_condition = {3,100,1000},close_season = {},close_time = {},close_weather = {},alone_event_id = 7,peri_event_id = 34,special_peri_event_id = {},},
	[8] = {id = 8,name = "演武场",vitality = 5,open_condition = {3,100,1000},close_season = {},close_time = {},close_weather = {},alone_event_id = 8,peri_event_id = 35,special_peri_event_id = {},},
	[9] = {id = 9,name = "城外官道",vitality = 5,open_condition = {3,100,1000},close_season = {},close_time = {2},close_weather = {},alone_event_id = 9,peri_event_id = 36,special_peri_event_id = {},},
	[10] = {id = 10,name = "皇宫",vitality = 5,open_condition = {3,250,1000},close_season = {},close_time = {},close_weather = {},alone_event_id = 10,peri_event_id = 37,special_peri_event_id = {},},
	[11] = {id = 11,name = "芙蓉园",vitality = 5,open_condition = {3,250,1000},close_season = {2,3},close_time = {},close_weather = {},alone_event_id = 11,peri_event_id = 38,special_peri_event_id = {},},
	[12] = {id = 12,name = "不思归",vitality = 5,open_condition = {3,250,1000},close_season = {},close_time = {1},close_weather = {},alone_event_id = 12,peri_event_id = 39,special_peri_event_id = {},},
	[13] = {id = 13,name = "胭脂湖畔",vitality = 5,open_condition = {3,250,1000},close_season = {},close_time = {},close_weather = {},alone_event_id = 13,peri_event_id = 40,special_peri_event_id = {},},
	[14] = {id = 14,name = "皇陵",vitality = 5,open_condition = {3,250,1000},close_season = {2,4},close_time = {},close_weather = {},alone_event_id = 14,peri_event_id = 41,special_peri_event_id = {},},
	[15] = {id = 15,name = "秋火渡",vitality = 5,open_condition = {3,250,1000},close_season = {},close_time = {3},close_weather = {},alone_event_id = 15,peri_event_id = 42,special_peri_event_id = {},},
	[16] = {id = 16,name = "西山",vitality = 5,open_condition = {3,250,1000},close_season = {},close_time = {},close_weather = {3},alone_event_id = 16,peri_event_id = 43,special_peri_event_id = {},},
	[17] = {id = 17,name = "城郊",vitality = 5,open_condition = {3,250,1000},close_season = {},close_time = {},close_weather = {2},alone_event_id = 17,peri_event_id = 44,special_peri_event_id = {},},
	[18] = {id = 18,name = "万花楼",vitality = 5,open_condition = {3,500,1000},close_season = {},close_time = {1},close_weather = {},alone_event_id = 18,peri_event_id = 45,special_peri_event_id = {},},
	[19] = {id = 19,name = "得意坊",vitality = 5,open_condition = {3,500,1000},close_season = {},close_time = {1},close_weather = {},alone_event_id = 19,peri_event_id = 46,special_peri_event_id = {},},
	[20] = {id = 20,name = "决斗场",vitality = 5,open_condition = {3,500,1000},close_season = {1,3,4},close_time = {},close_weather = {},alone_event_id = 20,peri_event_id = 47,special_peri_event_id = {},},
	[21] = {id = 21,name = "清溪小筑",vitality = 5,open_condition = {3,500,1000},close_season = {},close_time = {},close_weather = {3},alone_event_id = 21,peri_event_id = 48,special_peri_event_id = {},},
	[22] = {id = 22,name = "羽林军营",vitality = 5,open_condition = {3,500,1000},close_season = {},close_time = {3},close_weather = {},alone_event_id = 22,peri_event_id = 49,special_peri_event_id = {},},
	[23] = {id = 23,name = "驿馆",vitality = 5,open_condition = {3,800,1000},close_season = {},close_time = {},close_weather = {},alone_event_id = 23,peri_event_id = 50,special_peri_event_id = {},},
	[24] = {id = 24,name = "寂寞舫",vitality = 5,open_condition = {3,800,1000},close_season = {},close_time = {1},close_weather = {},alone_event_id = 24,peri_event_id = 51,special_peri_event_id = {},},
	[25] = {id = 25,name = "观雾庵",vitality = 5,open_condition = {3,800,1000},close_season = {},close_time = {},close_weather = {1,3},alone_event_id = 25,peri_event_id = 52,special_peri_event_id = {},},
	[26] = {id = 26,name = "玄冥教",vitality = 5,open_condition = {3,800,1000},close_season = {1,2},close_time = {1,2},close_weather = {},alone_event_id = 26,peri_event_id = 53,special_peri_event_id = {},},
	[27] = {id = 27,name = "飘渺阁",vitality = 5,open_condition = {3,800,1000},close_season = {2,3,4},close_time = {},close_weather = {1,2},alone_event_id = 27,peri_event_id = 54,special_peri_event_id = {},},
}

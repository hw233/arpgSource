---------------------------------------------------------------------------------
--------------------------以下代码为自动生成，请勿手工改动-----------------------
---------------------------------------------------------------------------------


tb_spell_template = {
	--  id:int	技能ID
	--  name:string	技能名称
	--  name_nv:string	技能名称(女)
	--  type:int	技能类型
	--  singleCD:int	公共CD
	--  groupCD:int	冷却时间
	--  group:int	技能族
	--  target_type:int	施放类型
	--  distance:int	施放范围
	--  num:int	施放数量
	--  hurt_percent:int	伤害系数百分比
	--  cannot_defence_hure:array	无视防御伤害参数
	--  fight_value:array	技能战斗力系数
	--  magic_type:int	施放消耗类型
	--  magic_value:int	消耗参数
	--  cost_value:array	升级消耗参数
	--  isUnitBallistic:int	是否unit弹道
	[1] = {id = 1,name = "玉虹贯日",name_nv = "玉虹贯日",type = 2,singleCD = 500,groupCD = 600,group = 1,target_type = 0,distance = 15,num = 1,hurt_percent = 120,cannot_defence_hure = {1200,0},fight_value = {1200,40},magic_type = 0,magic_value = 0,cost_value = {1000,0},isUnitBallistic = 0,},
	[2] = {id = 2,name = "天道剑术",name_nv = "天道剑术",type = 2,singleCD = 500,groupCD = 600,group = 1,target_type = 1,distance = 7,num = 4,hurt_percent = 80,cannot_defence_hure = {800,0},fight_value = {800,25},magic_type = 0,magic_value = 0,cost_value = {1000,0},isUnitBallistic = 0,},
	[3] = {id = 3,name = "天道剑术",name_nv = "天道剑术",type = 2,singleCD = 500,groupCD = 600,group = 1,target_type = 1,distance = 7,num = 4,hurt_percent = 80,cannot_defence_hure = {800,0},fight_value = {800,25},magic_type = 0,magic_value = 0,cost_value = {1000,0},isUnitBallistic = 0,},
	[4] = {id = 4,name = "天道剑术",name_nv = "天道剑术",type = 2,singleCD = 500,groupCD = 600,group = 1,target_type = 1,distance = 7,num = 4,hurt_percent = 80,cannot_defence_hure = {800,0},fight_value = {800,25},magic_type = 0,magic_value = 0,cost_value = {1000,0},isUnitBallistic = 0,},
	[5] = {id = 5,name = "战神附体",name_nv = "战神附体",type = 0,singleCD = 500,groupCD = 30000,group = 1,target_type = 0,distance = 7,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 1,magic_value = 30000,cost_value = {},isUnitBallistic = 0,},
	[6] = {id = 6,name = "金刚不坏体",name_nv = "金刚不坏体",type = 0,singleCD = 500,groupCD = 30000,group = 1,target_type = 0,distance = 7,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 1,magic_value = 30000,cost_value = {},isUnitBallistic = 0,},
	[7] = {id = 7,name = "万剑归宗",name_nv = "万剑归宗",type = 2,singleCD = 500,groupCD = 600,group = 1,target_type = 1,distance = 15,num = 8,hurt_percent = 80,cannot_defence_hure = {800,0},fight_value = {133,750},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[10] = {id = 10,name = "全屏秒杀",name_nv = "全屏秒杀",type = 2,singleCD = 500,groupCD = 600,group = 2,target_type = 1,distance = 15,num = 100,hurt_percent = 100,cannot_defence_hure = {},fight_value = {},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[20] = {id = 20,name = "大圣归来",name_nv = "大圣归来",type = 2,singleCD = 600,groupCD = 1000,group = 2,target_type = 1,distance = 7,num = 4,hurt_percent = 100,cannot_defence_hure = {0,100},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[21] = {id = 21,name = "普通技能",name_nv = "普通技能",type = 2,singleCD = 500,groupCD = 1000,group = 1,target_type = 0,distance = 7,num = 1,hurt_percent = 100,cannot_defence_hure = {0,10},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[30] = {id = 30,name = "觅食",name_nv = "觅食",type = 0,singleCD = 0,groupCD = 0,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[31] = {id = 31,name = "狂奔",name_nv = "狂奔",type = 0,singleCD = 500,groupCD = 30000,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[32] = {id = 32,name = "采集",name_nv = "采集",type = 0,singleCD = 0,groupCD = 0,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[33] = {id = 33,name = "火把",name_nv = "火把",type = 0,singleCD = 500,groupCD = 45000,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[34] = {id = 34,name = "鬼爪",name_nv = "鬼爪",type = 2,singleCD = 500,groupCD = 1000,group = 4,target_type = 0,distance = 7,num = 1,hurt_percent = 0,cannot_defence_hure = {0,1},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[35] = {id = 35,name = "灵体",name_nv = "灵体",type = 0,singleCD = 0,groupCD = 0,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[36] = {id = 36,name = "妖术",name_nv = "妖术",type = 2,singleCD = 500,groupCD = 1000,group = 4,target_type = 0,distance = 7,num = 1,hurt_percent = 0,cannot_defence_hure = {0,2},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[37] = {id = 37,name = "幻化",name_nv = "幻化",type = 0,singleCD = 500,groupCD = 45000,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[38] = {id = 38,name = "仙法",name_nv = "仙法",type = 2,singleCD = 500,groupCD = 600,group = 4,target_type = 0,distance = 15,num = 1,hurt_percent = 0,cannot_defence_hure = {0,2},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[39] = {id = 39,name = "天眼",name_nv = "天眼",type = 0,singleCD = 0,groupCD = 0,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[40] = {id = 40,name = "嗜杀",name_nv = "嗜杀",type = 2,singleCD = 500,groupCD = 600,group = 4,target_type = 0,distance = 7,num = 1,hurt_percent = 0,cannot_defence_hure = {0,3},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[41] = {id = 41,name = "修罗力",name_nv = "修罗力",type = 0,singleCD = 0,groupCD = 0,group = 4,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[42] = {id = 42,name = "步枪",name_nv = "步枪",type = 2,singleCD = 0,groupCD = 0,group = 5,target_type = 2,distance = 25,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[43] = {id = 43,name = "机枪",name_nv = "机枪",type = 2,singleCD = 0,groupCD = 0,group = 5,target_type = 2,distance = 15,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[44] = {id = 44,name = "狙击枪",name_nv = "狙击枪",type = 2,singleCD = 0,groupCD = 3000,group = 5,target_type = 2,distance = 25,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[45] = {id = 45,name = "更换弹匣",name_nv = "更换弹匣",type = 0,singleCD = 0,groupCD = 3000,group = 5,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[46] = {id = 46,name = "跳跃",name_nv = "跳跃",type = 0,singleCD = 0,groupCD = 3000,group = 5,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[47] = {id = 47,name = "疯狂乱抓",name_nv = "疯狂乱抓",type = 2,singleCD = 0,groupCD = 1000,group = 5,target_type = 1,distance = 4,num = 8,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[48] = {id = 48,name = "爆发突进",name_nv = "爆发突进",type = 0,singleCD = 0,groupCD = 30000,group = 5,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[49] = {id = 49,name = "变异再生",name_nv = "变异再生",type = 0,singleCD = 0,groupCD = 30000,group = 5,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[50] = {id = 50,name = "跳跃",name_nv = "跳跃",type = 0,singleCD = 0,groupCD = 3000,group = 5,target_type = 0,distance = 0,num = 1,hurt_percent = 0,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[100] = {id = 100,name = "怪物普通攻击",name_nv = "怪物普通攻击",type = 2,singleCD = 1000,groupCD = 1000,group = 3,target_type = 0,distance = 7,num = 1,hurt_percent = 100,cannot_defence_hure = {0,0},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[101] = {id = 101,name = "100级怪物普通攻击",name_nv = "100级怪物普通攻击",type = 2,singleCD = 1000,groupCD = 1000,group = 3,target_type = 0,distance = 7,num = 1,hurt_percent = 100,cannot_defence_hure = {0,45},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[102] = {id = 102,name = "200级怪物普通攻击",name_nv = "200级怪物普通攻击",type = 2,singleCD = 1000,groupCD = 1000,group = 3,target_type = 0,distance = 7,num = 1,hurt_percent = 100,cannot_defence_hure = {0,85},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
	[110] = {id = 110,name = "怪物远程攻击",name_nv = "怪物远程攻击",type = 2,singleCD = 1000,groupCD = 1000,group = 3,target_type = 0,distance = 16,num = 1,hurt_percent = 100,cannot_defence_hure = {0,5},fight_value = {0,0},magic_type = 0,magic_value = 0,cost_value = {},isUnitBallistic = 0,},
}

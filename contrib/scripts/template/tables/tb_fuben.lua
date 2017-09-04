---------------------------------------------------------------------------------
--------------------------以下代码为自动生成，请勿手工改动-----------------------
---------------------------------------------------------------------------------


tb_fuben = {
	--  id:int	地图ID
	--  name:string	名字
	--  limit:int	副本上限次数
	--  type_1:array	进入消耗
	--  level:int	进入等级
	--  type_2:int	完成目标类型
	--  num:array	通关条件
	--  time:int	通关时间（秒）
	--  refresh:array	刷怪规则
	--  cost_double:int	元宝翻倍消耗
	--  enemy:array	怪物
	--  enemy_special:int	特殊怪物
	--  drop_special:array	特殊怪物掉落
	--  point:array	刷新坐标
	--  reward_show:array	奖励道具
	[111] = {id = 111,name = "铜钱副本",limit = 2,type_1 = {642,1},level = 102,type_2 = 1,num = {2901,60,2902,3},time = 1800,refresh = {1,0,20,2,30,20,3,20,20},cost_double = 100,enemy = {2901},enemy_special = 2902,drop_special = {2,10,20,5000},point = {{10,7,1},{8,58,2},{85,58,3},{72,8,4}},reward_show = {652,10,1,20000},},
	[112] = {id = 112,name = "侠侣副本",limit = 2,type_1 = {642,1},level = 101,type_2 = 1,num = {2911,90,2912,90,2913,1},time = 1800,refresh = {1,0,30,2,30,30,3,30,30,4,30,30,5,30,30,6,30,30},cost_double = 100,enemy = {2911,2912},enemy_special = 2913,drop_special = {},point = {{70,40},{17,53},{71,10}},reward_show = {652,10,4,40},},
	[113] = {id = 113,name = "经验副本",limit = 2,type_1 = {642,1},level = 100,type_2 = 1,num = {2921,30,2922,1},time = 1800,refresh = {15,15,4,1},cost_double = 100,enemy = {2921,2921,2923},enemy_special = 2922,drop_special = {},point = {{35,45},{58,43},{86,19}},reward_show = {4,100},},
}

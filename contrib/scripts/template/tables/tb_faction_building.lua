---------------------------------------------------------------------------------
--------------------------以下代码为自动生成，请勿手工改动-----------------------
---------------------------------------------------------------------------------


tb_faction_building = {
	--  id:int 建筑ID
	--  type:int 建筑类别
	--  level:int 等级
	--  name:string 建筑名称
	--  unlock:int 解锁等级
	--  need_buildinglv:array 升级需求建筑前置等级{类别,等级}
	--  cost:int 消耗资金
	--  time:int 消耗时间(分钟)
	--  can_lvup:int 能否升级
	--  desc:array 描述
	--  params:array 参数
	[101] = {id = 101, type = 1, level = 1, name = "家族主殿", unlock = 1, need_buildinglv = {}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[102] = {id = 102, type = 1, level = 2, name = "家族主殿", unlock = 0, need_buildinglv = {}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[103] = {id = 103, type = 1, level = 3, name = "家族主殿", unlock = 0, need_buildinglv = {}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[104] = {id = 104, type = 1, level = 4, name = "家族主殿", unlock = 0, need_buildinglv = {}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[105] = {id = 105, type = 1, level = 5, name = "家族主殿", unlock = 0, need_buildinglv = {}, cost = 0, time = 0, can_lvup = 0, desc = {"说明1","说明2"}, params = {}},
	[201] = {id = 201, type = 2, level = 1, name = "家族宝库", unlock = 1, need_buildinglv = {{1,2}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {20,20,1503,10}},
	[202] = {id = 202, type = 2, level = 2, name = "家族宝库", unlock = 0, need_buildinglv = {{1,3}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {25,21,1503,10}},
	[203] = {id = 203, type = 2, level = 3, name = "家族宝库", unlock = 0, need_buildinglv = {{1,4}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {30,22,1503,10}},
	[204] = {id = 204, type = 2, level = 4, name = "家族宝库", unlock = 0, need_buildinglv = {{1,5}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {35,23,1503,10}},
	[205] = {id = 205, type = 2, level = 5, name = "家族宝库", unlock = 0, need_buildinglv = {}, cost = 0, time = 0, can_lvup = 0, desc = {"说明1","说明2"}, params = {40,24,1503,10}},
	[401] = {id = 401, type = 4, level = 1, name = "活动大厅", unlock = 1, need_buildinglv = {{1,2}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[402] = {id = 402, type = 4, level = 2, name = "活动大厅", unlock = 0, need_buildinglv = {{1,3}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[403] = {id = 403, type = 4, level = 3, name = "活动大厅", unlock = 0, need_buildinglv = {{1,4}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[404] = {id = 404, type = 4, level = 4, name = "活动大厅", unlock = 0, need_buildinglv = {{1,5}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[405] = {id = 405, type = 4, level = 5, name = "活动大厅", unlock = 0, need_buildinglv = {}, cost = 0, time = 0, can_lvup = 0, desc = {"说明1","说明2"}, params = {}},
	[501] = {id = 501, type = 5, level = 1, name = "技能大厅", unlock = 1, need_buildinglv = {{1,2}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[502] = {id = 502, type = 5, level = 2, name = "技能大厅", unlock = 0, need_buildinglv = {{1,3}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[503] = {id = 503, type = 5, level = 3, name = "技能大厅", unlock = 0, need_buildinglv = {{1,4}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[504] = {id = 504, type = 5, level = 4, name = "技能大厅", unlock = 0, need_buildinglv = {{1,5}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[505] = {id = 505, type = 5, level = 5, name = "技能大厅", unlock = 0, need_buildinglv = {}, cost = 0, time = 0, can_lvup = 0, desc = {"说明1","说明2"}, params = {}},
	[601] = {id = 601, type = 6, level = 1, name = "家族商店", unlock = 1, need_buildinglv = {{1,2}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[602] = {id = 602, type = 6, level = 2, name = "家族商店", unlock = 0, need_buildinglv = {{1,3}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[603] = {id = 603, type = 6, level = 3, name = "家族商店", unlock = 0, need_buildinglv = {{1,4}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[604] = {id = 604, type = 6, level = 4, name = "家族商店", unlock = 0, need_buildinglv = {{1,5}}, cost = 10, time = 5, can_lvup = 1, desc = {"说明1","说明2"}, params = {}},
	[605] = {id = 605, type = 6, level = 5, name = "家族商店", unlock = 0, need_buildinglv = {}, cost = 0, time = 0, can_lvup = 0, desc = {"说明1","说明2"}, params = {}},
}

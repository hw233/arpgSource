
--获取GM命令对应的枚举
function GetGmCommandKey(gm_str)
	local gm_key = -1
	if(gm_str == '@ClientInfo')then	
		return GM_COMMAND_JIULONGCHAO
	
	elseif gm_str == '@CUSTOM' then
		return GM_COMMAND_CUSTOM
	elseif gm_str == '@自杀' then
		return GM_COMMAND_SELF_KILL
    
	elseif(gm_str == '@素材'  or gm_str == '@Material')then
		return GM_COMMAND_SUCAI    					
    
	elseif(gm_str == '@脚本'  or gm_str == '@Script' )then
		return GM_COMMAND_JIAOBEN 						
	
	elseif(gm_str == '@踢'  or gm_str == '@Kick')then
		return GM_COMMAND_TI							
    
	elseif(gm_str == '@在线'  or gm_str == '@Online')then
		return GM_COMMAND_ZAIXIAN  					
    
	elseif(gm_str == '@地图人数'  or gm_str == '@MapMembers' )then
		return GM_COMMAND_DITURENSHU 					
    
	elseif(gm_str == '@招财'  or gm_str == '@ShowMeTheMoney' )then
		return GM_COMMAND_ZHAOCAI				
    
	elseif(gm_str == '@制造' or gm_str == '@Make')then
		return GM_COMMAND_ZHIZAO						
    
	elseif(gm_str == '@清理'  or gm_str=='@ClearMainBag')then
		return GM_COMMAND_QINGLI						
    
	elseif(gm_str == '@装备'  or gm_str=='@GiveMeEquip')then
		return GM_COMMAND_ZHUANGBEI					
    
	elseif(gm_str == '@排行榜' or gm_str=='@RefreshRankingList')then
		return GM_COMMAND_PAIHANGBANG					
    
	elseif(gm_str == '@邮件'  or gm_str=='@Mail')then
		return GM_COMMAND_YOUJIAN
					
    
	elseif(gm_str == '@崩掉' or gm_str=='@Crash')then
		return GM_COMMAND_BENGDIAO						
    
	elseif(gm_str == '@帮助' or gm_str=='@Help')then
		return GM_COMMAND_BANGZHU						
    
	elseif(gm_str == '@公告' or gm_str=='@Notice')then
		return GM_COMMAND_GONGGAO						
    
	elseif(gm_str == '@世界提示' or gm_str=='@WorldTip')then
		return GM_COMMAND_SHIJIETISHI					
    
	elseif(gm_str == '@排队' or gm_str=='@QueueUp')then
		return GM_COMMAND_PAIDUI						
    
	elseif(gm_str == '@屏蔽词' or gm_str=='@ReloadPingBiWords')then
		return GM_COMMAND_PINGBICI						
    
	elseif(gm_str == '@开启所有副本' or gm_str=='@ActivateAllInstance')then
		return GM_COMMAND_KAIQISUOYOUFUBEN				
        
	elseif(gm_str == '@关闭服务器'  or gm_str=='@CloseServer')then
		return GM_COMMAND_GUANBIFUWUQI					
    
	elseif(gm_str == '@LUA' or gm_str == '@lua')then
		return GM_COMMAND_LUA							
    
	elseif(gm_str == '@充值'  or gm_str=='@PayMoney')then
		return GM_COMMAND_CHONGZHI						
    
	elseif(gm_str == '@关闭地图'        or gm_str=='@CloseMap')then
		return GM_COMMAND_GUANBIDITU					
    
	elseif(gm_str == '@等级'            or gm_str=='@SetLevel' or gm_str=='@Rank')then
		return GM_COMMAND_DENGJI						
    
	elseif(gm_str == '@清理任务物品'    or gm_str=='@ClearQuestItem')then
		return GM_COMMAND_QINGLIRENWUWUPIN				
    
	elseif(gm_str == '@清理任务'        or gm_str=='@ClearQuest')then
		return GM_COMMAND_QINGLIRENWU					
    
	elseif(gm_str == '@完成任务'        or gm_str=='@CompleteQuest')then
		return GM_COMMAND_WANCHENGRENWU				
    
	elseif(gm_str == '@接受任务'        or gm_str=='@ReceiveQuest')then
		return GM_COMMAND_JIESHOURENWU					
    
	elseif(gm_str == '@干掉'            or gm_str=='@Kill')then
		return GM_COMMAND_GANDIAO						
    
	elseif(gm_str == '@刷怪'            or gm_str=='@MakeMonster')then
		return GM_COMMAND_SHUAGUAI						
    
	elseif(gm_str == '@传送'            or gm_str=='@Teleport')then
		return GM_COMMAND_CHUANSONG					
    
	elseif(gm_str == '@消除冷却'        or gm_str=='@ClearSpellCD')then
		return GM_COMMAND_XIAOCHULENGQUE		
    
	elseif(gm_str == '@瞎走' or gm_str=='@RandomMove')then
		return GM_COMMAND_XIAZOU	
		
	elseif(gm_str == '@自爆' or gm_str == '@suicide')then
		return GM_COMMAND_SUICIDE
	elseif(gm_str == '@服务器' or gm_str == '@server')then
		return GM_COMMAND_SERVER_INFO
	elseif(gm_str == '@数据库' or gm_str == '@db')then
		return GM_COMMAND_DB
	elseif(gm_str == '@开服时间' or gm_str == '@kfsj')then
		return GM_COMMAND_KAIFUSHIJIAN
	elseif(gm_str == '@测试数据' or gm_str == '@cssj')then
		return GM_COMMAND_TEST_SHUJU
	elseif(gm_str == '@狩猎开始' or gm_str == '@slks')then
		return GM_COMMAND_SHOULIE_START
	elseif(gm_str == '@更新服务器' or gm_str == '@fwqgx')then
		return GM_COMMAND_UPDATE_SERVER
	elseif(gm_str == '@协议注册' or gm_str == '@xyzc')then
		return GM_COMMAND_RESESSION_OPTS
	elseif(gm_str == '@任务跳转' or gm_str == '@rwtz')then
		return GM_COMMAND_RENWUTIAOZHUAN
	elseif(gm_str == '@备份数据' or gm_str == '@bfsj')then
		return GM_COMMAND_BACKUP_DATA
	elseif(gm_str == '@回档数据' or gm_str == '@hdsj')then
		return GM_COMMAND_RETURN_DATA
	elseif(gm_str == '@打印对象' or gm_str == '@dydx')then
		return GM_COMMAND_PRINT_OBJECT
	elseif(gm_str == '@测试合服' or gm_str == '@cshf')then
		return GM_COMMAND_TEST_HEFU
	elseif(gm_str == '@清空排行榜' or gm_str=='@ClearRankingList')then
		return GM_COMMAND_CLEAR_PAIHANGBANG
	elseif(gm_str == '@内存回收' or gm_str=='@MemoryRecovery')then
		return GM_COMMAND_MEMORY_RECOVERY
	elseif(gm_str == '@打印地图' or gm_str=='@PrintMap')then
		return GM_COMMAND_PRINT_MAP
	elseif(gm_str == '@清空地图信息' or gm_str=='@ClearMapInfo')then
		return GM_COMMAND_DEL_MAP_INFO
	elseif(gm_str == '@后台命令' or gm_str=='@RestoreSystem')then
		return GM_COMMAND_RESTORE_SYSTEM
	elseif(gm_str == '@清空物品监听' or gm_str=='@CallRemoveItem')then
		return GM_COMMAND_CALL_REMOVE_ITEM
	elseif(gm_str == '@破坏数据' or gm_str=='@DestroyData')then
		return GM_COMMAND_UNSET_OTHER_TAG
	elseif(gm_str == '@分线' or gm_str == '@subline')then
		return GM_COMMAND_SUBLINE
		
	elseif(gm_str == '@技能建筑' or gm_str == '@jnjz')then
		return 993
	elseif(gm_str == '@家族成员' or gm_str == '@jzcy')then
		return 994
	elseif(gm_str == '@装备养成' or gm_str == '@zbyc')then
		return 995
	elseif(gm_str == '@远征' or gm_str == '@tower')then
		return 996
	elseif(gm_str == '@家族商店' or gm_str == '@jzsd')then
		return 997
	elseif(gm_str == '@整理' or gm_str == '@zlbb')then
		return 998
	elseif(gm_str == '@测试' or gm_str == '@test')then
		return 999
	end
	return gm_key
end

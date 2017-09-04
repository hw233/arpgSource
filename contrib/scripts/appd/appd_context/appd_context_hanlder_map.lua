
--函数包路由表
local hanlders = {}

hanlders[CMSG_LIMIT_ACTIVITY_RECEIVE] = PlayerInfo.Hanlde_Limit_Activity_Receive
hanlders[CMSG_BAG_EXTENSION] = PlayerInfo.Hanlde_Bag_Extension
hanlders[CMSG_WAREHOUSE_SAVE_MONEY] = PlayerInfo.Hanlde_Warehouse_Save_Money
hanlders[CMSG_WAREHOUSE_TAKE_MONEY] = PlayerInfo.Hanlde_Warehouse_Take_Money
--hanlders[CMSG_RECEIVE_GIFT_PACKS] = PlayerInfo.Hanlde_Receive_Gift_Packs
hanlders[CMSG_QUERY_PLAYER_INFO] = PlayerInfo.Hanlde_Query_Player_Info
--hanlders[CMSG_USE_GOLD_OPT] = PlayerInfo.Hanlde_Use_Gold
hanlders[CMSG_BAG_ITEM_USER] = PlayerInfo.Hanlde_Bag_Item_User
hanlders[CMSG_BAG_ITEM_SELL] = PlayerInfo.Hanlde_Bag_Item_Sell
hanlders[CMSG_BAG_ITEM_SORT] = PlayerInfo.Hanlde_Bag_Item_Sort
hanlders[CMSG_FINISH_NOW_GUIDE] = PlayerInfo.Hanlde_Finish_Now_Guide

--修炼场
hanlders[CMSG_GET_CULTIVATION_INFO] = PlayerInfo.Hanlde_Get_Cultivation_Info
hanlders[CMSG_GET_CULTIVATION_RIVALS_INFO] = PlayerInfo.Hanlde_Get_Cultivation_Rivals_Info
hanlders[CMSG_GET_CULTIVATION_REWARD] = PlayerInfo.Hanlde_Get_Cultivation_Reward
hanlders[CMSG_REFRESH_CULTIVATION_RIVALS] = PlayerInfo.Hanlde_Refresh_Cultivation_Rivals
hanlders[CMSG_PLUNDER_CULTIVATION_RIVAL] = PlayerInfo.Hanlde_Plunder_Cultivation_Rival
hanlders[CMSG_REVENGE_CULTIVATION_RIVAL] = PlayerInfo.Hanlde_Revenge_Cultivation_Rival
hanlders[CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT] = PlayerInfo.Hanlde_Buy_Cultivation_Left_Plunder_Count

--登录大礼
hanlders[CMSG_GET_LOGIN_ACTIVITY_REWARD] = PlayerInfo.Hanlde_Get_Login_Activity_Reward



hanlders[CMSG_BAG_EXCHANGE_POS] = PlayerInfo.Hanlde_Bag_Exchange_Pos
--hanlders[CMSG_BAG_DESTROY] = PlayerInfo.Hanlde_Bag_Destroy
--hanlders[CMSG_BAG_ITEM_SPLIT] = PlayerInfo.Hanlde_Bag_Item_Split
--hanlders[CMSG_NPC_GET_GOODS_LIST] = PlayerInfo.Hanlde_Npc_Get_Goods_List
--hanlders[CMSG_STORE_BUY] = PlayerInfo.Handle_Store_Buy
hanlders[CMSG_NPC_SELL] = PlayerInfo.Hanlde_Npc_Sell
hanlders[CMSG_NPC_REPURCHASE] = PlayerInfo.Hanlde_Npc_Rrpurchase
hanlders[MSG_CHAT_NOTICE] = PlayerInfo.Handle_Chat_Notice
--hanlders[MSG_CHAT_HORN] = PlayerInfo.Handle_Chat_Horn
hanlders[CMSG_CHAT_WHISPER] = PlayerInfo.Handle_Chat_Whisper
--hanlders[MSG_CHAT_WORLD] = PlayerInfo.Handle_Chat_World

hanlders[CMSG_MALL_BUY] = PlayerInfo.Handle_Mall_Buy

hanlders[CMSG_STRENGTH] = PlayerInfo.Handle_Strength
hanlders[CMSG_CREATE_FACTION] = PlayerInfo.Handle_Faction_Create
--hanlders[CMSG_FACTION_UPGRADE] = PlayerInfo.Handle_Faction_Upgrade
hanlders[CMSG_FACTION_JOIN] = PlayerInfo.Hanlde_Faction_Apply
hanlders[CMSG_RAISE_BASE_SPELL] = PlayerInfo.Handle_Raise_BaseSpell
hanlders[CMSG_UPGRADE_ANGER_SPELL] = PlayerInfo.Handle_Upgrade_AngleSpell

hanlders[CMSG_RAISE_MOUNT] = PlayerInfo.Handle_Raise_Mount
hanlders[CMSG_UPGRADE_MOUNT] = PlayerInfo.Handle_Upgrade_Mount
hanlders[CMSG_UPGRADE_MOUNT_ONE_STEP] = PlayerInfo.Handle_Upgrade_Mount_One_Step
hanlders[CMSG_ILLUSION_MOUNT_ACTIVE] = PlayerInfo.Handle_Illusion_Active
hanlders[CMSG_ILLUSION_MOUNT] = PlayerInfo.Handle_Illusion

hanlders[CMSG_GEM] = PlayerInfo.gem

--[[hanlders[CMSG_DIVINE_ACTIVE] = PlayerInfo.Handle_Divine_Active
hanlders[CMSG_DIVINE_UPLEV] = PlayerInfo.Handle_Divine_UpLev
hanlders[CMSG_DIVINE_SWITCH] = PlayerInfo.Handle_Divine_Switch--]]


hanlders[CMSG_SWEEP_VIP_INSTANCE] = PlayerInfo.Handle_Sweep_Vip_Instance

hanlders[CMSG_HANG_UP] = PlayerInfo.Handle_Hang_Up

hanlders[CMSG_HANG_UP_SETTING] = PlayerInfo.Handle_Hang_Up_Setting

hanlders[CMSG_SWEEP_TRIAL_INSTANCE] = PlayerInfo.Handle_Sweep_Trial 
hanlders[CMSG_RES_INSTANCE_SWEEP] = PlayerInfo.Handle_Sweep_Res

hanlders[CMSG_RESET_TRIAL_INSTANCE] = PlayerInfo.Handle_Reset_Trial 

hanlders[CMSG_SOCIAL_ADD_FRIEND] = PlayerInfo.Handle_Add_Friend 
hanlders[CMSG_SOCIAL_SUREADD_FRIEND] = PlayerInfo.Handle_Sure_Add_Friend
hanlders[CMSG_SOCIAL_GIFT_FRIEND] = PlayerInfo.Handle_Gift_Friend
hanlders[CMSG_SOCIAL_RECOMMEND_FRIEND] = PlayerInfo.Handle_Recommend_Friend
hanlders[CMSG_SOCIAL_REVENGE_ENEMY] = PlayerInfo.Handle_Revenge_Enemy
hanlders[CMSG_SOCIAL_DEL_FRIEND] = PlayerInfo.Handle_Remove_Friend 
hanlders[CMSG_SOCIAL_CLEAR_APPLY] = PlayerInfo.Handler_Clear_Apply 
hanlders[CMSG_SOCIAL_ADD_FRIEND_BYNAME] = PlayerInfo.Handle_Add_Friend_ByName

hanlders[CMSG_CHAT_BY_CHANNEL] = PlayerInfo.Handle_Chat_By_Channel

hanlders[CMSG_MSG_DECLINE] = PlayerInfo.Handle_Msg_Decline
hanlders[CMSG_BLOCK_CHAT] = PlayerInfo.Handle_Block_Chat


-----------------------------------------------------------------------------
hanlders[CMSG_READ_MAIL] = PlayerInfo.Handle_Read_Mail
hanlders[CMSG_PICK_MAIL] = PlayerInfo.Handle_Pick_Mail
hanlders[CMSG_REMOVE_MAIL] = PlayerInfo.Handle_Remove_Mail
hanlders[CMSG_PICK_MAIL_ONE_STEP] = PlayerInfo.Handle_Pick_Mail_One_Step
hanlders[CMSG_REMOVE_MAIL_ONE_STEP] = PlayerInfo.Handle_Remove_Mail_One_Step

hanlders[CMSG_CANCEL_BLOCK_CHAT] = PlayerInfo.Handle_Cancel_Block_Chat
-------------------------------------------------------------------------------

hanlders[CMSG_FACTION_GETLIST] = PlayerInfo.Handle_Faction_Get_List 
hanlders[CMSG_FACTION_MANAGER] = PlayerInfo.Hanlde_Faction_Manager 
hanlders[CMSG_FACTION_MEMBER_OPERATE] = PlayerInfo.Handle_Faction_People 
hanlders[CMSG_FACTION_FAST_JOIN] = PlayerInfo.Handle_Faction_FastJoin


hanlders[CMSG_WORLD_BOSS_ENROLL] = PlayerInfo.Handle_World_Boss_Enroll

---------------------------
hanlders[CMSG_RANK_ADD_LIKE] = PlayerInfo.Handle_Rank_Like

---------------------------
hanlders[CMSG_GET_ACTIVITY_REWARD] = PlayerInfo.Handle_Avtive_Reward

hanlders[CMSG_GET_ACHIEVE_REWARD] = PlayerInfo.Handle_Achieve_Reward
hanlders[CMSG_GET_ACHIEVE_ALL_REWARD] = PlayerInfo.Handle_Achieve_All_Reward
hanlders[CMSG_SET_TITLE] = PlayerInfo.Handle_Set_Title
hanlders[CMSG_INIT_TITLE] = PlayerInfo.Handle_Init_Title
hanlders[CMSG_WELFARE_SHOUCHONG_REWARD] = PlayerInfo.Handle_Welfare_Shouchong
hanlders[CMSG_WELFARE_CHECKIN] = PlayerInfo.Handle_Welfare_CheckIn
hanlders[CMSG_WELFARE_CHECKIN_ALL] = PlayerInfo.Handle_Welfare_CheckInAll
hanlders[CMSG_WELFARE_CHECKIN_GETBACK] = PlayerInfo.Handle_Welfare_CheckIn_Getback
hanlders[CMSG_WELFARE_LEVEL] = PlayerInfo.Handle_Welfare_Level
hanlders[CMSG_WELFARE_ACTIVE_GETBACK] = PlayerInfo.Handle_Welfare_ActiveGetback
hanlders[CMSG_WELFARE_GETALLLIST_GETBACK] = PlayerInfo.Handle_Welfare_List_Getback
hanlders[CMSG_WELFARE_GETALL_GETBACK] = PlayerInfo.Handle_Welfare_All_Getback

hanlders[CMSG_WELFARE_GET_RECHARGE_REWARD] = PlayerInfo.Handle_Welfare_Get_Recharge_Reward
hanlders[CMSG_WELFARE_GET_CONSUME_REWARD] = PlayerInfo.Handle_Welfare_Get_Consume_Reward
hanlders[CMSG_WELFARE_GET_SEVENDAY_REWARD] = PlayerInfo.Handle_Welfare_Get_Sevenday_Reward
--------------------------------
hanlders[CMSG_PICK_QUEST_REWARD] = PlayerInfo.Handle_Pick_Quest

hanlders[CMSG_PICK_QUEST_CHAPTER_REWARD] = PlayerInfo.Handle_Pick_Quest_Chapter_Reward

hanlders[MSG_KUAFU_3V3_CANCEL_MATCH]	= PlayerInfo.Handle_Kuafu_3v3_Cancel_Match

--[[hanlders[CMSG_KUAFU_3V3_MATCH]	= PlayerInfo.Handle_Kuafu_3v3_Match

hanlders[CMSG_KUAFU_3V3_MATCH_OPER]	= PlayerInfo.Handle_Kuafu_3v3_Match_Oper


hanlders[CMSG_KUAFU_3V3_BUYTIMES]	= PlayerInfo.Handle_Kuafu_3v3_BuyTimes

hanlders[CMSG_KUAFU_3V3_DAYREWARD]	= PlayerInfo.Handle_Kuafu_3v3Day_Reward

hanlders[CMSG_KUAFU_3V3_GETRANLIST]	= PlayerInfo.Handle_Kuafu_3v3_RankList

hanlders[CMSG_KUAFU_3V3_GETMYRANK]	= PlayerInfo.Handle_Kuafu_3v3_My_Rank--]]

hanlders[CMSG_USE_VIRTUAL_ITEM] = PlayerInfo.Handle_Use_Virtual_Item

hanlders[CMSG_KUAFU_XIANFU_MATCH]	= PlayerInfo.Handle_Kuafu_Xianfu_Match

hanlders[CMSG_BUY_XIANFU_ITEM] = PlayerInfo.Handle_Buy_Xianfu_Item

hanlders[CMSG_DOUJIANTAI_FIGHT] = PlayerInfo.Handle_Doujiantai_Fight

hanlders[CMSG_DOUJIANTAI_BUYTIME] = PlayerInfo.Handle_Doujian_BuyTimes

hanlders[CMSG_DOUJIANTAI_CLEARCD] = PlayerInfo.Handle_Doujian_ClearCd

hanlders[CMSG_DOUJIANTAI_FIRST_REWARD] = PlayerInfo.Handle_Doujian_FirstReward


hanlders[MSG_DOUJIANTAI_GET_ENEMYS_INFO] = PlayerInfo.Handle_Doujiantai_Enemys_Info

hanlders[CMSG_DOUJIANTAI_GET_RANK] = PlayerInfo.Handle_Doujian_Get_Rank

hanlders[CMSG_DOUJIANTAI_REFRESH_ENEMYS] = PlayerInfo.Handle_Doujiantai_Refresh_Enemys

hanlders[MSG_DOUJIANTAI_TOP3] = PlayerInfo.Handle_Doujiantai_Top3

hanlders[CMSG_SUBMIT_QUEST_DAILY2] = PlayerInfo.Handle_Submit_Quest_Daily2

hanlders[CMSG_PICK_DAILY2_QUEST_REWARD] = PlayerInfo.Handle_Pick_Quest_Daily2

hanlders[CMSG_FINISH_OPTIONAL_GUIDE_STEP] = PlayerInfo.Handle_Optional_Guide_Step

hanlders[CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED] = PlayerInfo.Handle_Execute_Quest_Cmd_After_Accepted

hanlders[CMSG_SMELTING_EQUIP] = PlayerInfo.Handle_Smelting_Equip

hanlders[CMSG_STOREHOUSE_HAND_IN] = PlayerInfo.Handle_Storehouse_Hand_In

hanlders[CMSG_STOREHOUSE_EXCHANGE] = PlayerInfo.Handle_Storehouse_Exchange

hanlders[CMSG_STOREHOUSE_DESTROY] = PlayerInfo.Handle_Storehouse_Destroy

hanlders[CMSG_SEND_FACTION_INVITE] = PlayerInfo.Handle_Send_Faction_Invite

hanlders[CMSG_BUY_VIPGIFT] = PlayerInfo.Handle_Buy_VipGift

hanlders[CMSG_ACTIVITY_OPT_BUY_DAILYGIFT] = PlayerInfo.Handle_Activity_Opt_Buy_DailyGift
hanlders[CMSG_ACTIVITY_OPT_BUY_LIMITGIFT] = PlayerInfo.Handle_Activity_Opt_Buy_LimitGift
hanlders[CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD] = PlayerInfo.Handle_Activity_Opt_Get_Rank_Process_Reward
hanlders[CMSG_ACTIVITY_OPT_GET_RANK_LIST] = PlayerInfo.Handle_Activity_Opt_Show_Rank_List



hanlders[CMSG_SOCIAL_BUY_REVENGE_TIMES] = PlayerInfo.Handle_Buy_Revengr_Times
hanlders[CMSG_SOCIAL_REMOVE_ENEMY] = PlayerInfo.Handle_Remove_Enemy

--hanlders[CMSG_SEND_FACTION_GIFT] = PlayerInfo.Handle_Send_Faction_Gift
--hanlders[CMSG_GET_FACTION_GIFT_EXREWARD] = PlayerInfo.Handle_Get_Faction_Gift_Exreward
--hanlders[CMSG_GET_ALL_FACTION_GIFT_EXREWARD] = PlayerInfo.Handle_Get_All_Faction_Gift_Exreward

--hanlders[CMSG_GET_FACTION_GIFT_RANK_PAGE] = PlayerInfo.Handle_Get_Faction_Gift_Rank_Page

--hanlders[CMSG_DIVINE_FORGE] = PlayerInfo.Handle_Divine_Forge
--hanlders[CMSG_DIVINE_ADVANCE] = PlayerInfo.Handle_Divine_Advance
--hanlders[CMSG_DIVINE_SPIRIT] = PlayerInfo.Handle_Divine_Spirit


hanlders[CMSG_BUY_MASS_BOSS_TIMES] = PlayerInfo.Handle_Buy_Mass_Boss_Times

--法宝
hanlders[CMSG_TALISMAN_ACTIVE] = PlayerInfo.Handle_Talisman_Active
hanlders[CMSG_TALISMAN_LVUP] = PlayerInfo.Handle_Talisman_Lvup
--

--神羽
hanlders[CMSG_WINGS_ACTIVE] = PlayerInfo.Handle_Wings_Active
hanlders[CMSG_WINGS_BLESS] = PlayerInfo.Handle_Wings_Bless
hanlders[CMSG_WINGS_RANKUP] = PlayerInfo.Handle_Wings_RankUp
hanlders[CMSG_WINGS_STRENGTH] = PlayerInfo.Handle_Wings_Strength

--装备
hanlders[CMSG_EQUIPDEVELOP_OPERATE] = PlayerInfo.Handle_Equipdevelop_Operate

--
hanlders[CMSG_GROUP_INSTANCE_MATCH] = PlayerInfo.Handle_Group_Instance_Match
hanlders[CMSG_BUY_GROUP_INSTANCE_TIMES] = PlayerInfo.Handle_Buy_Group_Instance_Times

-- 经脉
hanlders[CMSG_MERIDIAN_PRACTISE] = PlayerInfo.Handle_Meridian_Practise
hanlders[CMSG_ADD_MERIDIAN_EXP] = PlayerInfo.Handle_Add_Meridian_Exp


hanlders[CMSG_RAISE_MOUNT_LEVEL_BASE] = PlayerInfo.Handle_Raise_Mount_Level_Base
hanlders[CMSG_ACTIVE_MOUNT]	= PlayerInfo.Handle_Active_Mount

hanlders[CMSG_MATCH_SINGLE_PVP] = PlayerInfo.Handle_Match_Single_PVP

hanlders[CMSG_BUY_MATCH_SINGLE_PVP_TIMES] = PlayerInfo.Handle_Buy_Match_Single_PVP_Times
hanlders[CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD] = PlayerInfo.Handle_Pick_Match_Single_PVP_Extra_Reward

hanlders[CMSG_ACTIVE_APPEARANCE] = PlayerInfo.Handle_Active_Appearance
hanlders[CMSG_EQUIP_APPEARANCE]  = PlayerInfo.Handle_Equip_Appearance
hanlders[CMSG_CANCEL_EQUIP_APPEARANCE] = PlayerInfo.Handle_Cancel_Equip_Appearance

hanlders[CMSG_UNLOCK_TITLE] = PlayerInfo.Handle_Unlock_title

hanlders[CMSG_DRAW_LOTTERY] = PlayerInfo.Handle_Draw_Lottery

hanlders[CMSG_RENAME] = PlayerInfo.Handle_Rename

hanlders[CMSG_GET_PLAYER_OVERVIEW] = PlayerInfo.Handle_Get_Player_Overview

hanlders[CMSG_RISK_GET_RANK] = PlayerInfo.Handle_Get_Risk_Reward

return hanlders
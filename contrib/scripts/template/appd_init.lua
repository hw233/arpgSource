--[[
	应用服要用到的模板表请在这require
]]

require("template.tables.tb_skill_base")
require("template.tables.tb_skill_uplevel")
require("template.tables.tb_suit")
require("template.tables.tb_char_level")
require("template.tables.tb_char_skill")
require("template.tables.tb_assistangerspell_upgrade")
require("template.tables.tb_learn_spell")
require("template.tables.tb_item_template")
require("template.tables.tb_bag_size")
require("template.tables.tb_item_sort_cd")

-- 坐骑
require("template.tables.tb_mount_base")
require("template.tables.tb_mount_illusion")
require("template.tables.tb_mount_train")
require("template.tables.tb_mount_train_type")
require("template.tables.tb_mount_upgrade")
require("template.tables.tb_mount_raise_level")

require("template.tables.tb_shop")
require("template.tables.tb_store")
require("template.tables.tb_hook_hp_item")

--强化表
require("template.tables.tb_strengthen_base")
require("template.tables.tb_strengthen_bless")
require("template.tables.tb_strengthen_mul")

--宝石表
require("template.tables.tb_gem_base")
require("template.tables.tb_gem_cost")
require("template.tables.tb_gem_mul")
require("template.tables.tb_gem_pos")
require("template.tables.tb_gem_rate")

--装备养成表
require("template.tables.tb_equipdevelop_strength")
require("template.tables.tb_equipdevelop_refine")
require("template.tables.tb_equipdevelop_gem_part")
require("template.tables.tb_equipdevelop_gem")
require("template.tables.tb_equipdevelop_washattrs")
require("template.tables.tb_equipdevelop_bonus")


--神兵表
require("template.tables.tb_divine_base")
require("template.tables.tb_divine_streng")
require("template.tables.tb_divine_bless")
require("template.tables.tb_divine_forge")
require("template.tables.tb_divine_advance")
require("template.tables.tb_divine_spirit")

--法宝表
require("template.tables.tb_talisman_base")
require("template.tables.tb_talisman_spirit")

--神羽表
require("template.tables.tb_wings_base")
require("template.tables.tb_wings_bless")
require("template.tables.tb_wings_strength")

--副本
require("template.tables.tb_map_vip")
require("template.tables.tb_instance_trial")

--社交
require("template.tables.tb_social_shop")
require("template.tables.tb_social_familiay")
require("template.tables.tb_social_num")
require("template.tables.tb_enemy_revenge_buy")
require("template.tables.tb_enemy_revenge_base")


--帮派
require("template.tables.tb_faction_base")
require("template.tables.tb_faction_boss")
require("template.tables.tb_faction_boss_reward")
require("template.tables.tb_faction_creat")
require("template.tables.tb_faction_donation")
require("template.tables.tb_faction_monster")
require("template.tables.tb_faction_shop")
require("template.tables.tb_faction_zhiwei")
require("template.tables.tb_faction_building_base")
require("template.tables.tb_faction_building")
require("template.tables.tb_faction_building_reward")
require("template.tables.tb_faction_gift")
require("template.tables.tb_faction_gift_robot_rank")
require("template.tables.tb_faction_gift_rank_reward")
require("template.tables.tb_faction_gift_rank_base")
require("template.tables.tb_faction_bossdefense_base")
require("template.tables.tb_faction_bossdefense_point")
require("template.tables.tb_faction_bossdefense_pool")
require("template.tables.tb_faction_tower_base")
require("template.tables.tb_faction_tower_floor")

require("template.tables.tb_faction_skill_building")
require("template.tables.tb_faction_skill_base")
require("template.tables.tb_faction_skill_lvup")


-----------帮派权限------------
require("template.tables.tb_faction_privilege")



--vip
require("template.tables.tb_vip_base")

--信息
require("template.tables.tb_msg_text_type")

-- 地图
require("template.tables.tb_map")
require("template.tables.tb_field_boss_time")
require("template.tables.tb_worldboss_time")
require("template.tables.tb_instance_res")

-- 邮件
require("template.tables.tb_mail")
require("template.tables.tb_rank_reward")
require("template.tables.tb_rank_like")


--- 副本
require("template.tables.tb_instance_reward")

--活动
require("template.tables.tb_activity_base")
require("template.tables.tb_activity_reward")

-- 掉落包
require("template.tables.tb_drop_reward")
require("template.tables.tb_drop_packet")

--成就称号
require("template.tables.tb_achieve_base")
require("template.tables.tb_achieve_progress")
require("template.tables.tb_title_base")
require("template.tables.tb_welfare_shouchong")
require("template.tables.tb_welfare_checkin")
require("template.tables.tb_welfare_checkin_all")
require("template.tables.tb_welfare_level")
require("template.tables.tb_welfare_back_data")
require("template.tables.tb_welfare_back")

require("template.tables.tb_welfare_consume")
require("template.tables.tb_welfare_recharge")
require("template.tables.tb_welfare_sevengift")

-- 任务
require("template.tables.tb_quest")
require("template.tables.tb_quest_chapter")

-- 每日环任务
require("template.tables.tb_quest_daily_base")
require("template.tables.tb_quest_daily")

-- 日常任务
require("template.tables.tb_quest_daily2")
require("template.tables.tb_quest_daily2_base")
require("template.tables.tb_quest_daily2_set")
require("template.tables.tb_quest_daily2_finish_reward")


--竞技
require("template.tables.tb_kuafu3v3_base")
require("template.tables.tb_kuafu3v3_week_reward")
require("template.tables.tb_kuafu3v3_month_reward")
require("template.tables.tb_kuafu3v3_day_reward")

-- 仙府夺宝
require("template.tables.tb_kuafu_xianfu_base")
require("template.tables.tb_kuafu_xianfu_condition")

require("template.tables.tb_kuafu_xianfu_tst_base")

require("template.tables.tb_box")

-- 斗剑台
require("template.tables.tb_doujiantai_base")
require("template.tables.tb_doujiantai_dummy")
require("template.tables.tb_doujiantai_combat_win")
require("template.tables.tb_doujiantai_rank")
require("template.tables.tb_doujiantai_first")
require("template.tables.tb_doujiantai_day")

-- 修炼场
require("template.tables.tb_xiulianchang_base")
require("template.tables.tb_xiulianchang_dummy")
require("template.tables.tb_xiulianchang_reward")
require("template.tables.tb_xiulianchang_vip")
require("template.tables.tb_xiulianchang_fight_range")

--登录大礼
require("template.tables.tb_login_activity_reward")



---------------------------------------------------
require("template.tables.tb_doujiantai_fight_range")

require("template.tables.tb_creature_template")
require("template.tables.tb_gameobject_template")
require("template.tables.tb_system_base")

require("template.tables.tb_item_quality_color")

-- 世界冒险
require("template.tables.tb_risk_base")
require("template.tables.tb_risk_data")

-- 全名boss
require("template.tables.tb_mass_boss_base")
require("template.tables.tb_mass_boss_info")
require("template.tables.tb_mass_boss_loot")
require("template.tables.tb_mass_boss_times")

require("template.tables.tb_group_instance_base")
require("template.tables.tb_group_instance_buy")

-- 经脉
require("template.tables.tb_meridian_info")
require("template.tables.tb_meridian_item")
require("template.tables.tb_meridian_source")

-- 排名赛
require("template.tables.tb_single_pvp_base")
require("template.tables.tb_single_pvp_grade")
require("template.tables.tb_single_pvp_robot")
require("template.tables.tb_single_pvp_extra")
require("template.tables.tb_single_pvp_times")

require("template.tables.tb_appearance_info")
require("template.tables.tb_appearance_pokedex")

require("template.tables.tb_char_info")

require("template.tables.tb_battle_force")
require("template.tables.tb_rename_info")

require("template.tables.tb_activity_lottery_base")
require("template.tables.tb_activity_lottery_high")
require("template.tables.tb_activity_lottery_low")
require("template.tables.tb_activity_lottery_middle")
require("template.tables.tb_activity_time")

require("template.tables.tb_activity_daily_gift_base")
require("template.tables.tb_activity_daily_gift")

require("template.tables.tb_activity_limit_gift_base")
require("template.tables.tb_activity_limit_gift")

require("template.tables.tb_activity_rank_base")
require("template.tables.tb_activity_rank_rank_reward")
require("template.tables.tb_activity_rank_process_reward")



--- 数据预处理
require("template.tabset")
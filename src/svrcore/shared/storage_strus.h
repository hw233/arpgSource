#ifndef _STORAGE_STRUS_H_
#define _STORAGE_STRUS_H_
#include <comm/common_stl.h>


struct grade_up_pro{
	uint32 id;
};

const uint32 GRADE_UP_PRO_LEN = 40 * sizeof(uint32)/sizeof( grade_up_pro );

struct creature_template_pro{
	uint32 id;
};

const uint32 CREATURE_TEMPLATE_PRO_LEN = 40 * sizeof(uint32)/sizeof( creature_template_pro );
struct creature_template_spell{
	uint32 id;
};

const uint32 CREATURE_TEMPLATE_SPELL_LEN = 40 * sizeof(uint32)/sizeof( creature_template_spell );
struct creature_template_reward_id{
	uint32 id;
};

const uint32 CREATURE_TEMPLATE_REWARD_ID_LEN = 40 * sizeof(uint32)/sizeof( creature_template_reward_id );

struct item_template_basic_properties{
	uint32 id;
};

const uint32 ITEM_TEMPLATE_BASIC_PROPERTIES_LEN = 40 * sizeof(uint32)/sizeof( item_template_basic_properties );
struct item_template_suit_id{
	uint32 id;
};

const uint32 ITEM_TEMPLATE_SUIT_ID_LEN = 40 * sizeof(uint32)/sizeof( item_template_suit_id );
struct item_template_using_effect{
	uint32 id;
};

const uint32 ITEM_TEMPLATE_USING_EFFECT_LEN = 40 * sizeof(uint32)/sizeof( item_template_using_effect );
struct item_template_forge_pro{
	uint32 id;
};

const uint32 ITEM_TEMPLATE_FORGE_PRO_LEN = 40 * sizeof(uint32)/sizeof( item_template_forge_pro );
struct item_template_forge_pro_max{
	uint32 id;
};

const uint32 ITEM_TEMPLATE_FORGE_PRO_MAX_LEN = 40 * sizeof(uint32)/sizeof( item_template_forge_pro_max );

struct skill_base_follow{
	uint32 id;
};

const uint32 SKILL_BASE_FOLLOW_LEN = 40 * sizeof(uint32)/sizeof( skill_base_follow );
struct skill_base_time_change{
	uint32 id;
};

const uint32 SKILL_BASE_TIME_CHANGE_LEN = 40 * sizeof(uint32)/sizeof( skill_base_time_change );
struct skill_base_spell_time{
	uint32 id;
};

const uint32 SKILL_BASE_SPELL_TIME_LEN = 40 * sizeof(uint32)/sizeof( skill_base_spell_time );
struct skill_base_uplevel_id{
	uint32 id;
};

const uint32 SKILL_BASE_UPLEVEL_ID_LEN = 40 * sizeof(uint32)/sizeof( skill_base_uplevel_id );
struct skill_base_attack_mast{
	uint32 id;
};

const uint32 SKILL_BASE_ATTACK_MAST_LEN = 40 * sizeof(uint32)/sizeof( skill_base_attack_mast );

struct goods_cost{
	uint32 id;
};

const uint32 GOODS_COST_LEN = 40 * sizeof(uint32)/sizeof( goods_cost );


struct skill_uplevel_uplevel_cost{
	uint32 id;
};

const uint32 SKILL_UPLEVEL_UPLEVEL_COST_LEN = 40 * sizeof(uint32)/sizeof( skill_uplevel_uplevel_cost );
struct skill_uplevel_type_effect{
	uint32 id;
};

const uint32 SKILL_UPLEVEL_TYPE_EFFECT_LEN = 40 * sizeof(uint32)/sizeof( skill_uplevel_type_effect );


struct char_level_prop{
	uint32 id;
};

const uint32 CHAR_LEVEL_PROP_LEN = 40 * sizeof(uint32)/sizeof( char_level_prop );




struct quest_template_req_item_id{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REQ_ITEM_ID_LEN = 40 * sizeof(uint32)/sizeof( quest_template_req_item_id );
struct quest_template_req_item_count{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REQ_ITEM_COUNT_LEN = 40 * sizeof(uint32)/sizeof( quest_template_req_item_count );
struct quest_template_req_world_object_id{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REQ_WORLD_OBJECT_ID_LEN = 40 * sizeof(uint32)/sizeof( quest_template_req_world_object_id );
struct quest_template_req_world_object_count{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REQ_WORLD_OBJECT_COUNT_LEN = 40 * sizeof(uint32)/sizeof( quest_template_req_world_object_count );
struct quest_template_rew_choice_item_id{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REW_CHOICE_ITEM_ID_LEN = 40 * sizeof(uint32)/sizeof( quest_template_rew_choice_item_id );
struct quest_template_rew_choice_item_count{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REW_CHOICE_ITEM_COUNT_LEN = 40 * sizeof(uint32)/sizeof( quest_template_rew_choice_item_count );
struct quest_template_rew_item_id{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REW_ITEM_ID_LEN = 40 * sizeof(uint32)/sizeof( quest_template_rew_item_id );
struct quest_template_rew_item_count{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REW_ITEM_COUNT_LEN = 40 * sizeof(uint32)/sizeof( quest_template_rew_item_count );
struct quest_template_src_item_id{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_SRC_ITEM_ID_LEN = 40 * sizeof(uint32)/sizeof( quest_template_src_item_id );
struct quest_template_src_item_count{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_SRC_ITEM_COUNT_LEN = 40 * sizeof(uint32)/sizeof( quest_template_src_item_count );
struct quest_template_subType{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_SUBTYPE_LEN = 40 * sizeof(uint32)/sizeof( quest_template_subType );
struct quest_template_param0{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_PARAM0_LEN = 40 * sizeof(uint32)/sizeof( quest_template_param0 );
struct quest_template_param1{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_PARAM1_LEN = 40 * sizeof(uint32)/sizeof( quest_template_param1 );
struct quest_template_req_kill_level_info{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_REQ_KILL_LEVEL_INFO_LEN = 40 * sizeof(uint32)/sizeof( quest_template_req_kill_level_info );
struct quest_template_manual_param{
	uint32 id;
};

const uint32 QUEST_TEMPLATE_MANUAL_PARAM_LEN = 40 * sizeof(uint32)/sizeof( quest_template_manual_param );

struct buff_template
{
	int id;
	char name [50];
	int recalculation;
	int die_clear;
};

struct gameobject_template
{
	int id;
	char name [50];
	int time;
	int actionState;
	int regX;
	int regY;
	int isAround;
	int go_flag;
	int quest_id0;
	int quest_id1;
	int quest_id2;
	int quest_id3;
	int trigger_width;
	int trigger_height;

};

struct item_template
{
	int id;
	char name [50];
	int level;
	int rank;
	int type;
	int pos;
	int quality;
	int sex;
	int price;
	int is_transaction;
	int max_overlap;
	int is_slather;
	int use_result;
	union
	{
		float basic_properties [40];
		item_template_basic_properties s_basic_properties[ ITEM_TEMPLATE_BASIC_PROPERTIES_LEN ];
	};
	union
	{
		float suit_id [40];
		item_template_suit_id s_suit_id[ ITEM_TEMPLATE_SUIT_ID_LEN ];
	};
	union
	{
		float using_effect [40];
		item_template_using_effect s_using_effect[ ITEM_TEMPLATE_USING_EFFECT_LEN ];
	};
	int use_limit;
	int category;
	int category_cooldown;
	int cooldown;
	int is_discard;
	int goods_id;
	int expend_data;
	int max_durable;
	int use_object;
	int quest;
	union
	{
		float forge_pro [40];
		item_template_forge_pro s_forge_pro[ ITEM_TEMPLATE_FORGE_PRO_LEN ];
	};
	union
	{
		float forge_pro_max [40];
		item_template_forge_pro_max s_forge_pro_max[ ITEM_TEMPLATE_FORGE_PRO_MAX_LEN ];
	};
	int is_auction;
	int auction_money;

};

struct skill_base
{
	int id;
	char name [50];
	char name_nv [50];
	int is_initiative;
	int type;
	union
	{
		float follow [40];
		skill_base_follow s_follow[ SKILL_BASE_FOLLOW_LEN ];
	};
	union
	{
		float time_change [40];
		skill_base_time_change s_time_change[ SKILL_BASE_TIME_CHANGE_LEN ];
	};
	int singleCD;
	int groupCD;
	int self_cd;
	union
	{
		float spell_time [40];
		skill_base_spell_time s_spell_time[ SKILL_BASE_SPELL_TIME_LEN ];
	};
	int group;
	int target_type;
	int is_open;
	int skill_percent;
	union
	{
		float uplevel_id [40];
		skill_base_uplevel_id s_uplevel_id[ SKILL_BASE_UPLEVEL_ID_LEN ];
	};
	int magic_type;
	int nuqi_change;
	int is_kill;
	union
	{
		float attack_mast [40];
		skill_base_attack_mast s_attack_mast[ SKILL_BASE_ATTACK_MAST_LEN ];
	};
	int is_fix;
	int isUnitBallistic;
	int condition_skill;
	int lock_type;
	int tips_type;

};

struct skill_uplevel
{
	int id;
	int distance;
	int range;
	int num;
	int hurt_percent;
	int hurt_add;
	int cannot_defence_hure;
	int fight_value;
	int magic_value;
	union
	{
		float uplevel_cost [40];
		skill_uplevel_uplevel_cost s_uplevel_cost[ SKILL_UPLEVEL_UPLEVEL_COST_LEN ];
	};
	int need_level;
	union
	{
		float type_effect [40];
		skill_uplevel_type_effect s_type_effect[ SKILL_UPLEVEL_TYPE_EFFECT_LEN ];
	};

};

struct char_level
{
	int id;
	int next_exp;
	int meditation;
	union
	{
		float prop [40];
		char_level_prop s_prop[ CHAR_LEVEL_PROP_LEN ];
	};

};

struct quest_template
{
	int id;
	int method;
	int quest_level;
	int quest_type;
	int limit_time;
	int suggested_players;
	int quest_flags;
	int special_flags;
	int prev_quest;
	char title [50];
	char details [50];
	char objectives [50];
	char offer_reward_text [50];
	char request_item_text [50];
	char end_text [50];
	char start_script [50];
	char complete_script [50];
	uint32 min_level;
	int faction;
	int require_min_rep_value;
	union
	{
		uint32 req_item_id [40];
		quest_template_req_item_id s_req_item_id[ QUEST_TEMPLATE_REQ_ITEM_ID_LEN ];
	};
	union
	{
		uint32 req_item_count [40];
		quest_template_req_item_count s_req_item_count[ QUEST_TEMPLATE_REQ_ITEM_COUNT_LEN ];
	};
	union
	{
		uint32 req_world_object_id [40];
		quest_template_req_world_object_id s_req_world_object_id[ QUEST_TEMPLATE_REQ_WORLD_OBJECT_ID_LEN ];
	};
	union
	{
		uint32 req_world_object_count [40];
		quest_template_req_world_object_count s_req_world_object_count[ QUEST_TEMPLATE_REQ_WORLD_OBJECT_COUNT_LEN ];
	};
	union
	{
		uint32 rew_choice_item_id [40];
		quest_template_rew_choice_item_id s_rew_choice_item_id[ QUEST_TEMPLATE_REW_CHOICE_ITEM_ID_LEN ];
	};
	union
	{
		uint32 rew_choice_item_count [40];
		quest_template_rew_choice_item_count s_rew_choice_item_count[ QUEST_TEMPLATE_REW_CHOICE_ITEM_COUNT_LEN ];
	};
	union
	{
		uint32 rew_item_id [40];
		quest_template_rew_item_id s_rew_item_id[ QUEST_TEMPLATE_REW_ITEM_ID_LEN ];
	};
	union
	{
		uint32 rew_item_count [40];
		quest_template_rew_item_count s_rew_item_count[ QUEST_TEMPLATE_REW_ITEM_COUNT_LEN ];
	};
	int rew_xp;
	int rew_gold;
	int rew_gift;
	int rew_silver;
	int rew_spell_cast;
	union
	{
		uint32 src_item_id [40];
		quest_template_src_item_id s_src_item_id[ QUEST_TEMPLATE_SRC_ITEM_ID_LEN ];
	};
	union
	{
		uint32 src_item_count [40];
		quest_template_src_item_count s_src_item_count[ QUEST_TEMPLATE_SRC_ITEM_COUNT_LEN ];
	};
	int point_mapid;
	int point_x;
	int point_y;
	int req_level;
	int enable;
	int dynasty;
	int parent_id;
	union
	{
		float subType [40];
		quest_template_subType s_subType[ QUEST_TEMPLATE_SUBTYPE_LEN ];
	};
	union
	{
		float param0 [40];
		quest_template_param0 s_param0[ QUEST_TEMPLATE_PARAM0_LEN ];
	};
	union
	{
		float param1 [40];
		quest_template_param1 s_param1[ QUEST_TEMPLATE_PARAM1_LEN ];
	};
	union
	{
		float req_kill_level_info [40];
		quest_template_req_kill_level_info s_req_kill_level_info[ QUEST_TEMPLATE_REQ_KILL_LEVEL_INFO_LEN ];
	};
	int manual_type;
	union
	{
		float manual_param [40];
		quest_template_manual_param s_manual_param[ QUEST_TEMPLATE_MANUAL_PARAM_LEN ];
	};

};

struct quest_wo_relation
{
	int id;
	int quest_id;
	int wo_entry;
	int relatetion_type;

};

struct quest_Point_Info
{
	int id;
	int template_id;
	int type;
	int map_id;
	int pos_x;
	int pos_y;
	int flag;

};

struct quest_loot_item
{
	int id;
	int entry;
	int count;
	int possibility;
	int creature_id;
	int quest_id;

};

struct goods
{
	int id;
	int type;
	int item;
	union
	{
		float cost [40];
		goods_cost s_cost[ GOODS_COST_LEN ];
	};
	int discount;
	int flags;
	int sort_index;
	int npc;
	int start_time;
	int end_time;

};

struct creature_template
{
	int id;
	char name [50];
	int npcflag;
	int monster_type;
	int level;
	union
	{
		float pro [40];
		creature_template_pro s_pro[ CREATURE_TEMPLATE_PRO_LEN ];
	};
	union
	{
		float spell [40];
		creature_template_spell s_spell[ CREATURE_TEMPLATE_SPELL_LEN ];
	};
	char ainame [50];
	int visionradius;
	int actionradius;
	int attack_type;
	int lockfaceto;
	int attack_range;
	int exp;
	union
	{
		float reward_id [40];
		creature_template_reward_id s_reward_id[ CREATURE_TEMPLATE_REWARD_ID_LEN ];
	};
	char dialogue [50];
	int money_min;
	int money_max;
	int money_percent;
	int money_num;
	int shouhun_min;
	int shouhun_max;
	int shouhun_percent;
	int drop_belong;

};

struct grade_up
{
	int id;
	char name [50];
	int need_level;
	union
	{
		float pro [40];
		grade_up_pro s_pro[ GRADE_UP_PRO_LEN ];
	};

};


#endif

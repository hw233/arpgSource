#include "svrcore.h"
#include "storage_loader.h"
#include <fstream>
#include "progress_bar.h"
#include <iostream>
#include "lua_helper.h"


MemoryDB< buff_template >  buff_template_db;
MemoryDB< gameobject_template >  gameobject_template_db;
MemoryDB< item_template >  item_template_db;
MemoryDB< skill_base >  skill_base_db;
MemoryDB< skill_uplevel >  skill_uplevel_db;
MemoryDB< char_level >  char_level_db;
MemoryDB< quest_template >  quest_template_db;
MemoryDB< quest_wo_relation >  quest_wo_relation_db;
MemoryDB< quest_Point_Info >  quest_Point_Info_db;
MemoryDB< quest_loot_item >  quest_loot_item_db;
MemoryDB< goods >  goods_db;
MemoryDB< creature_template >  creature_template_db;
MemoryDB< grade_up >  grade_up_db;


bool Load_buff_template (lua_State *L)
{
	std::cout<<"Load buff_template ING"<<std::endl;
	buff_template_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_buff_template");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		buff_template *info = new buff_template;
		memset(info,0,sizeof(buff_template));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "name");
		strncpy(info->name ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "recalculation");
		info->recalculation = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "die_clear");
		info->die_clear = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		buff_template_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_gameobject_template (lua_State *L)
{
	std::cout<<"Load gameobject_template ING"<<std::endl;
	gameobject_template_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_gameobject_template");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		gameobject_template *info = new gameobject_template;
		memset(info,0,sizeof(gameobject_template));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "name");
		strncpy(info->name ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "time");
		info->time = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "actionState");
		info->actionState = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "regX");
		info->regX = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "regY");
		info->regY = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "isAround");
		info->isAround = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "go_flag");
		info->go_flag = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
/*
		lua_getfield(L, -1, "quest_id0");
		info->quest_id0 = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_id1");
		info->quest_id1 = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_id2");
		info->quest_id2 = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_id3");
		info->quest_id3 = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
*/
		lua_getfield(L, -1, "trigger_width");
		info->trigger_width = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "trigger_height");
		info->trigger_height = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		gameobject_template_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_item_template (lua_State *L)
{
	std::cout<<"Load item_template ING"<<std::endl;
	item_template_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_item_template");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		item_template *info = new item_template;
		memset(info,0,sizeof(item_template));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		/*lua_getfield(L, -1, "name");
		strncpy(info->name ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);*/
		lua_getfield(L, -1, "level");
		info->level = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "rank");
		info->rank = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "type");
		info->type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "pos");
		info->pos = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quality");
		info->quality = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "sex");
		info->sex = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "price");
		info->price = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "is_transaction");
		info->is_transaction = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "max_overlap");
		info->max_overlap = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "is_slather");
		info->is_slather = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "use_result");
		info->use_result = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "basic_properties");
		int it_basic_properties = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_27 = 0;
		while(lua_next(L,it_basic_properties))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->basic_properties [ i_27++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "suit_id");
		int it_suit_id = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_28 = 0;
		while(lua_next(L,it_suit_id))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->suit_id [ i_28++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "using_effect");
		int it_using_effect = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_29 = 0;
		while(lua_next(L,it_using_effect))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->using_effect [ i_29++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "use_limit");
		info->use_limit = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "category");
		info->category = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "category_cooldown");
		info->category_cooldown = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "cooldown");
		info->cooldown = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "is_discard");
		info->is_discard = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "goods_id");
		info->goods_id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "expend_data");
		info->expend_data = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "max_durable");
		info->max_durable = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "use_object");
		info->use_object = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest");
		info->quest = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "forge_pro");
		int it_forge_pro = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_42 = 0;
		while(lua_next(L,it_forge_pro))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->forge_pro [ i_42++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "forge_pro_max");
		int it_forge_pro_max = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_43 = 0;
		while(lua_next(L,it_forge_pro_max))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->forge_pro_max [ i_43++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "is_auction");
		info->is_auction = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "auction_money");
		info->auction_money = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		item_template_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_skill_base (lua_State *L)
{	
	std::cout<<"Load skill_base ING"<<std::endl;
	skill_base_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_skill_base");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		skill_base *info = new skill_base;

		memset(info,0,sizeof(skill_base));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		lua_getfield(L, -1, "is_initiative");
		info->is_initiative = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		skill_base_db . insertVector(info);

		lua_pop(L,1);
	}
	return true;
}


bool Load_skill_uplevel (lua_State *L)
{
	std::cout<<"Load skill_uplevel ING"<<std::endl;
	skill_uplevel_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_skill_uplevel");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		skill_uplevel *info = new skill_uplevel;
		memset(info,0,sizeof(skill_uplevel));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "distance");
		info->distance = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "range");
		info->range = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "num");
		info->num = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "hurt_percent");
		info->hurt_percent = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "hurt_add");
		info->hurt_add = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "cannot_defence_hure");
		info->cannot_defence_hure = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "fight_value");
		info->fight_value = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "magic_value");
		info->magic_value = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "uplevel_cost");
		int it_uplevel_cost = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_12 = 0;
		while(lua_next(L,it_uplevel_cost))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->uplevel_cost [ i_12++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "need_level");
		info->need_level = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "type_effect");
		int it_type_effect = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_15 = 0;
		while(lua_next(L,it_type_effect))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->type_effect [ i_15++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);

		skill_uplevel_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_char_level (lua_State *L)
{
	std::cout<<"Load char_level ING"<<std::endl;
	char_level_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_char_level");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		char_level *info = new char_level;
		memset(info,0,sizeof(char_level));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		lua_getfield(L, -1, "next_exp");
		info->next_exp = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		/**
		lua_getfield(L, -1, "prop");
		int it_prop = lua_gettop(L);
		lua_pushnil(L);


		uint32 i_4 = 0;
		while(lua_next(L,it_prop))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->prop [ i_4++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		*/

		char_level_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_quest_template (lua_State *L)
{
	std::cout<<"Load quest_template ING"<<std::endl;
	quest_template_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_quest_template");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		quest_template *info = new quest_template;
		memset(info,0,sizeof(quest_template));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "method");
		info->method = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_level");
		info->quest_level = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_type");
		info->quest_type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "limit_time");
		info->limit_time = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "suggested_players");
		info->suggested_players = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_flags");
		info->quest_flags = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "special_flags");
		info->special_flags = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "prev_quest");
		info->prev_quest = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "title");
		strncpy(info->title ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "details");
		strncpy(info->details ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "objectives");
		strncpy(info->objectives ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "offer_reward_text");
		strncpy(info->offer_reward_text ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "request_item_text");
		strncpy(info->request_item_text ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "end_text");
		strncpy(info->end_text ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "start_script");
		strncpy(info->start_script ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "complete_script");
		strncpy(info->complete_script ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "min_level");
		info->min_level = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "faction");
		info->faction = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "require_min_rep_value");
		info->require_min_rep_value = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "req_item_id");
		int it_req_item_id = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_21 = 0;
		while(lua_next(L,it_req_item_id))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->req_item_id [ i_21++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "req_item_count");
		int it_req_item_count = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_22 = 0;
		while(lua_next(L,it_req_item_count))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->req_item_count [ i_22++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "req_world_object_id");
		int it_req_world_object_id = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_23 = 0;
		while(lua_next(L,it_req_world_object_id))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->req_world_object_id [ i_23++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "req_world_object_count");
		int it_req_world_object_count = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_24 = 0;
		while(lua_next(L,it_req_world_object_count))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->req_world_object_count [ i_24++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_choice_item_id");
		int it_rew_choice_item_id = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_25 = 0;
		while(lua_next(L,it_rew_choice_item_id))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->rew_choice_item_id [ i_25++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_choice_item_count");
		int it_rew_choice_item_count = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_26 = 0;
		while(lua_next(L,it_rew_choice_item_count))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->rew_choice_item_count [ i_26++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_item_id");
		int it_rew_item_id = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_27 = 0;
		while(lua_next(L,it_rew_item_id))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->rew_item_id [ i_27++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_item_count");
		int it_rew_item_count = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_28 = 0;
		while(lua_next(L,it_rew_item_count))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->rew_item_count [ i_28++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_xp");
		info->rew_xp = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_gold");
		info->rew_gold = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_gift");
		info->rew_gift = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_silver");
		info->rew_silver = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "rew_spell_cast");
		info->rew_spell_cast = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "src_item_id");
		int it_src_item_id = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_34 = 0;
		while(lua_next(L,it_src_item_id))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->src_item_id [ i_34++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "src_item_count");
		int it_src_item_count = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_35 = 0;
		while(lua_next(L,it_src_item_count))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->src_item_count [ i_35++ ] = (uint32)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "point_mapid");
		info->point_mapid = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "point_x");
		info->point_x = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "point_y");
		info->point_y = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "req_level");
		info->req_level = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "enable");
		info->enable = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "dynasty");
		info->dynasty = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "parent_id");
		info->parent_id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "subType");
		int it_subType = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_44 = 0;
		while(lua_next(L,it_subType))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->subType [ i_44++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "param0");
		int it_param0 = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_45 = 0;
		while(lua_next(L,it_param0))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->param0 [ i_45++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "param1");
		int it_param1 = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_46 = 0;
		while(lua_next(L,it_param1))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->param1 [ i_46++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "req_kill_level_info");
		int it_req_kill_level_info = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_47 = 0;
		while(lua_next(L,it_req_kill_level_info))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->req_kill_level_info [ i_47++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "manual_type");
		info->manual_type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "manual_param");
		int it_manual_param = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_49 = 0;
		while(lua_next(L,it_manual_param))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->manual_param [ i_49++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);

		quest_template_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_quest_wo_relation (lua_State *L)
{
	std::cout<<"Load quest_wo_relation ING"<<std::endl;
	quest_wo_relation_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_quest_wo_relation");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		quest_wo_relation *info = new quest_wo_relation;
		memset(info,0,sizeof(quest_wo_relation));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_id");
		info->quest_id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "wo_entry");
		info->wo_entry = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "relatetion_type");
		info->relatetion_type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		quest_wo_relation_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_quest_Point_Info (lua_State *L)
{
	std::cout<<"Load quest_Point_Info ING"<<std::endl;
	quest_Point_Info_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_quest_Point_Info");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		quest_Point_Info *info = new quest_Point_Info;
		memset(info,0,sizeof(quest_Point_Info));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "template_id");
		info->template_id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "type");
		info->type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "map_id");
		info->map_id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "pos_x");
		info->pos_x = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "pos_y");
		info->pos_y = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "flag");
		info->flag = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		quest_Point_Info_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_quest_loot_item (lua_State *L)
{
	std::cout<<"Load quest_loot_item ING"<<std::endl;
	quest_loot_item_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_quest_loot_item");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		quest_loot_item *info = new quest_loot_item;
		memset(info,0,sizeof(quest_loot_item));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "entry");
		info->entry = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "count");
		info->count = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "possibility");
		info->possibility = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "creature_id");
		info->creature_id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "quest_id");
		info->quest_id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		quest_loot_item_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_goods (lua_State *L)
{
	std::cout<<"Load goods ING"<<std::endl;
	goods_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_goods");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		goods *info = new goods;
		memset(info,0,sizeof(goods));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "type");
		info->type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "item");
		info->item = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "cost");
		int it_cost = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_5 = 0;
		while(lua_next(L,it_cost))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->cost [ i_5++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "discount");
		info->discount = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "flags");
		info->flags = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "sort_index");
		info->sort_index = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "npc");
		info->npc = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "start_time");
		info->start_time = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "end_time");
		info->end_time = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		goods_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_creature_template (lua_State *L)
{
	std::cout<<"Load creature_template ING"<<std::endl;
	creature_template_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_creature_template");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		creature_template *info = new creature_template;
		memset(info,0,sizeof(creature_template));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "name");
		strncpy(info->name ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "npcflag");
		info->npcflag = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "monster_type");
		info->monster_type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "level");
		info->level = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "pro");
		int it_pro = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_13 = 0;
		while(lua_next(L,it_pro))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->pro [ i_13++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "spell");
		int it_spell = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_14 = 0;
		while(lua_next(L,it_spell))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->spell [ i_14++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "ainame");
		strncpy(info->ainame ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "visionradius");
		info->visionradius = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "actionradius");
		info->actionradius = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "attack_type");
		info->attack_type = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "lockfaceto");
		info->lockfaceto = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "attack_range");
		info->attack_range = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "exp");
		info->exp = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "reward_id");
		int it_reward_id = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_28 = 0;
		while(lua_next(L,it_reward_id))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->reward_id [ i_28++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);
		lua_getfield(L, -1, "dialogue");
		strncpy(info->dialogue ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "money_min");
		info->money_min = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "money_max");
		info->money_max = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "money_percent");
		info->money_percent = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "money_num");
		info->money_num = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "shouhun_min");
		info->shouhun_min = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "shouhun_max");
		info->shouhun_max = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "shouhun_percent");
		info->shouhun_percent = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "drop_belong");
		info->drop_belong = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);

		creature_template_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}


bool Load_grade_up (lua_State *L)
{
	std::cout<<"Load grade_up ING"<<std::endl;
	grade_up_db . clear();

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "tb_grade_up");
	ASSERT(lua_istable(L, -1));
	int it_idx = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L,it_idx))
	{
		grade_up *info = new grade_up;
		memset(info,0,sizeof(grade_up));
		lua_getfield(L, -1, "id");
		info->id = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "name");
		strncpy(info->name ,LUA_TOSTRING(L, -1),50);
		lua_pop(L,1);
		lua_getfield(L, -1, "need_level");
		info->need_level = (int)LUA_TONUMBER(L, -1);
		lua_pop(L,1);
		lua_getfield(L, -1, "pro");
		int it_pro = lua_gettop(L);
		lua_pushnil(L);
		uint32 i_4 = 0;
		while(lua_next(L,it_pro))
		{
			if(lua_istable(L,-1))
			{
				lua_pop(L,2);
				break;
			}
			info->pro [ i_4++ ] = (float)LUA_TONUMBER(L, -1);
			lua_pop(L,1);
		}
		lua_pop(L,1);

		grade_up_db . insertVector(info);
		lua_pop(L,1);
	}
	return true;
}



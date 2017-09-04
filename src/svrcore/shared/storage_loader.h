#ifndef _STORAGE_LOADER_H_
#define _STORAGE_LOADER_H_
#include <string>
#include <vector>
#include <shared/storage_strus.h>
#include "MemoryDB.h"


TEA_SVRCORE_SPEC extern MemoryDB< buff_template >  buff_template_db;
TEA_SVRCORE_SPEC extern MemoryDB< gameobject_template >  gameobject_template_db;
TEA_SVRCORE_SPEC extern MemoryDB< item_template >  item_template_db;
TEA_SVRCORE_SPEC extern MemoryDB< skill_base >  skill_base_db;
TEA_SVRCORE_SPEC extern MemoryDB< skill_uplevel >  skill_uplevel_db;
TEA_SVRCORE_SPEC extern MemoryDB< char_level >  char_level_db;
TEA_SVRCORE_SPEC extern MemoryDB< quest_template >  quest_template_db;
TEA_SVRCORE_SPEC extern MemoryDB< quest_wo_relation >  quest_wo_relation_db;
TEA_SVRCORE_SPEC extern MemoryDB< quest_Point_Info >  quest_Point_Info_db;
TEA_SVRCORE_SPEC extern MemoryDB< quest_loot_item >  quest_loot_item_db;
TEA_SVRCORE_SPEC extern MemoryDB< goods >  goods_db;
TEA_SVRCORE_SPEC extern MemoryDB< creature_template >  creature_template_db;
TEA_SVRCORE_SPEC extern MemoryDB< grade_up >  grade_up_db;


TEA_SVRCORE_SPEC bool Load_buff_template (lua_State *L);
TEA_SVRCORE_SPEC bool Load_gameobject_template (lua_State *L);
TEA_SVRCORE_SPEC bool Load_item_template (lua_State *L);
TEA_SVRCORE_SPEC bool Load_skill_base (lua_State *L);
TEA_SVRCORE_SPEC bool Load_skill_uplevel (lua_State *L);
TEA_SVRCORE_SPEC bool Load_char_level (lua_State *L);
TEA_SVRCORE_SPEC bool Load_quest_template (lua_State *L);
TEA_SVRCORE_SPEC bool Load_quest_wo_relation (lua_State *L);
TEA_SVRCORE_SPEC bool Load_quest_Point_Info (lua_State *L);
TEA_SVRCORE_SPEC bool Load_quest_loot_item (lua_State *L);
TEA_SVRCORE_SPEC bool Load_goods (lua_State *L);
TEA_SVRCORE_SPEC bool Load_creature_template (lua_State *L);
TEA_SVRCORE_SPEC bool Load_grade_up (lua_State *L);

#endif

#ifndef _ADD_MEMDB_INDEXH_
#define _ADD_MEMDB_INDEXH_

#include "storage_loader.h"
#include "MemoryDB.h"

bool Add_Goods_Index()
{
	memdb_index *index = goods_db.create_index();
	index = goods_db.add_index_cell( index, MEMDB_INDX(goods, npc, MemDB_Compare_Handle<int>) );
	index = goods_db.add_index_cell( index, MEMDB_INDX(goods, item, MemDB_Compare_Handle<int>) );
	index = goods_db.add_index_cell( index, MEMDB_INDX(goods, id, MemDB_Compare_Handle<int>) );
	goods_db.add_index(index);
	return true;
}
bool Add_instance_material()
{
	/* TODO
	memdb_index *index = instance_material_db.create_index();
	index = instance_material_db.add_index_cell(index,MEMDB_INDX(instance_material, map_parent_id, MemDB_Compare_Handle<int>) );
	instance_material_db.add_index(index);
	*/
	return true;

}

//任务相关
bool Add_Quest_Index()
{
	//前置任务索引
	memdb_index *index_quest = quest_template_db.create_index();
	index_quest = quest_template_db.add_index_cell(index_quest,
		MEMDB_INDX(quest_template,prev_quest,MemDB_Compare_Handle<int>));
	index_quest = quest_template_db.add_index_cell(index_quest,
		MEMDB_INDX(quest_template,quest_level,MemDB_Compare_Handle<int>));				//以任务等级做key
	index_quest = quest_template_db.add_index_cell(index_quest,
		MEMDB_INDX(quest_template,enable,MemDB_Compare_Handle<uint32>));				//任务启用
	index_quest = quest_template_db.add_index_cell(index_quest,
		MEMDB_INDX(quest_template,dynasty,MemDB_Compare_Handle<uint32>));				//朝代
	index_quest = quest_template_db.add_index_cell(index_quest,
		MEMDB_INDX(quest_template,parent_id,MemDB_Compare_Handle<uint32>));				//朝代

	quest_template_db.add_index(index_quest);

	//任务关联表 任务索引
	memdb_index *index_quest_relaton = quest_wo_relation_db.create_index();
	index_quest_relaton = quest_wo_relation_db.add_index_cell(index_quest_relaton,		//以任务ID为key
		MEMDB_INDX(quest_wo_relation,quest_id,MemDB_Compare_Handle<int>));	
	index_quest_relaton = quest_wo_relation_db.add_index_cell(index_quest_relaton,		//以地图对象为key
		MEMDB_INDX(quest_wo_relation,wo_entry,MemDB_Compare_Handle<int>));
	quest_wo_relation_db.add_index(index_quest_relaton);

	return true;
}


bool Add_Telport_Index()
{
	/* TODO
	memdb_index *index = telport_db.create_index();
	index = telport_db.add_index_cell( index, MEMDB_INDX(telport, template_id, MemDB_Compare_Handle<int>) );
	telport_db.add_index(index);
	*/
	return true;
}

bool Add_Quest_Loot_Item_Index()
{
	memdb_index *index = quest_loot_item_db.create_index();
	index = quest_loot_item_db.add_index_cell(index,
		MEMDB_INDX(quest_loot_item,creature_id,MemDB_Compare_Handle<uint32>));
	quest_loot_item_db.add_index(index);
	return true;
}

bool Add_Creature_template_Index()
{
	creature_template_db.register_compare_handle(MEMDB_CMP_PTR(creature_template, name, MemDB_Compare_Handle<char *>));
	return true;
}

bool Add_Item_Template_Index()
{
	memdb_index *index = item_template_db.create_index();
	index = item_template_db.add_index_cell(index,
		MEMDB_INDX(item_template,type,MemDB_Compare_Handle<int>));
	index = item_template_db.add_index_cell(index,
		MEMDB_INDX(item_template,quality,MemDB_Compare_Handle<int>));
	index = item_template_db.add_index_cell(index,
		MEMDB_INDX(item_template,level,MemDB_Compare_Handle<int>));
	index = item_template_db.add_index_cell(index,
		MEMDB_INDX(item_template,pos,MemDB_Compare_Handle<int>));
	item_template_db.add_index(index);
	return true;
}

#endif


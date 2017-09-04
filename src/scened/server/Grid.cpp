#include "scened_app.h"
#include "ScenedObjectManager.h"
#include "scened.h"
#include "Grid.h"
#include "Unit.h"
#include "Creature.h"
#include "Player.h"
#include "scened_context.h"
#include "Map.h"
#include "object/LootMgr.h"
#include "GridUpdateMask.h"

#define COMPRESS_MIN_SIZE 100

struct SendFunc
{
	SendFunc(packet* pkt):pkt_(pkt), need_free(false)
	{ }

	SendFunc(ByteArray& byte):need_free(true)
	{
		byte.position(0);
		uint16 optcode = byte.readUnsignedShort();
		pkt_ = nullptr;
		pkt_ = external_protocol_new_packet(optcode);
		packet_write(pkt_,(char*)byte.cur_data(),byte.bytesAvailable());
		update_packet_len(pkt_);		
	}

	SendFunc(ByteArray& byte, uint16 optcode):need_free(true)
	{
		pkt_ = nullptr;
		byte.position(0);		
		pkt_ = external_protocol_new_packet(optcode);
		packet_write(pkt_,(char*)byte.cur_data(),byte.bytesAvailable());
		update_packet_len(pkt_);		
	}
	~SendFunc()
	{
		if (need_free && pkt_)
		{
			external_protocol_free_packet(pkt_);
		}
	}

	void operator()(uint32 fd)
	{
		pkt_->cid = fd;
		server_packet *dst = NULL;
		pack_packet_gate(&dst, pkt_);
		ScenedApp::g_app->SendToNetgd(dst);
		internal_protocol_free_packet(dst);
	}

	void operator()(Player* p)
	{
		if(p && p->GetSession())
		{
			p->GetSession()->SendPacket(*pkt_);
		}
		else
		{
			tea_perror("SendFunc p or p->GetSession() is null");
		}
	}
	packet* pkt_;	
	bool need_free;
};

//////////////////////////////////////////////////////////////////////////
//for Grid

Grid::Grid():active(false),x(-1),y(-1),map_inst(nullptr),index(0),loot(nullptr),start_x(0),end_x(0),start_y(0),end_y(0)
{

}

Grid::~Grid()
{
	safe_delete(loot);
	ClearBlock();
	for (auto ptr:fighting_blocks)
		ObjMgr.GridFreeByteArray(ptr);	
	
	for (auto ptr:unit_move_blocks)
		ObjMgr.GridFreeByteArray(ptr);	

	for (auto ptr:unit_jump_blocks)
		ObjMgr.GridFreeByteArray(ptr);
	
}

//对象更新
void Grid::ObjectAccess()
{
	auto f = [this](Unit *wo){
		if(wo->GetTypeId() == TYPEID_PLAYER)
			static_cast<Player*>(wo)->SyncUnitToPlayerData();
		//if(wo->BinlogEmpty())//没变化
		//	return;
		ByteArray *bytes = ObjMgr.GridMallocByteArray();
		if(wo->WriteUpdateBlock(*bytes, wo->GetUIntGuid(),gGridUpdateMask.update_int_mask_,gGridUpdateMask.update_string_mask_))
		{
			AddUpdateBlock(bytes);
			wo->Clear();
		}
		else
		{
			ObjMgr.GridFreeByteArray(bytes);
		}
	};

	for_each(players.begin(),players.end(),f);
	for_each(creatures.begin(),creatures.end(),f);
	for_each(worldobjs.begin(),worldobjs.end(),f);

	//////////////////////////////////////////////////////////////////////////	
	//Grid信息更新
	if(loot)
	{
		ByteArray *bytes = ObjMgr.GridMallocByteArray();
		if(loot->WriteUpdateBlock(*bytes, loot->GetUIntGuid()))
		{
			AddUpdateBlock(bytes);
			loot->Clear();
		}
		else
		{
			ObjMgr.GridFreeByteArray(bytes);
		}
	}
}

//发送对象
void Grid::SendObjectUpdate()
{
	if(players.size() == 0)
		return;

	static vector<ByteArray *> ud;
	ud.clear();

	//创建和离开只通知当前格
	ud.insert(ud.end(),out_area_blocks.begin(),out_area_blocks.end());
	ud.insert(ud.end(),create_blocks.begin(),create_blocks.end());	
	create_blocks.clear();

	//更新块
	for(auto p:notice_grid)
		ud.insert(ud.end(),p->update_blocks.begin(),p->update_blocks.end());		

	//如果对象没有变化
	if(ud.empty())
		return;

	ByteArray *byte = ObjMgr.GridMallocByteArray();
	byte->clear();
	for (auto bytes:ud)
	{
		bytes->position(0);
		byte->writeBytes(*bytes);
	}
	byte->position(0);
	if(byte->length() == 0)
	{
		ObjMgr.GridFreeByteArray(byte);
	}
	else if(byte->length() < COMPRESS_MIN_SIZE)
	{
		packet *pkt = external_protocol_new_packet(SMSG_GRID_UD_OBJECT_2);
		packet_write(pkt,(char*)byte->cur_data(),byte->bytesAvailable());
		update_packet_len(pkt);
		std::for_each(players.begin(),players.end(),SendFunc(pkt));
		GridManager* gridMgr = map_inst->GetGridManager();
		if (gridMgr)
			gridMgr->BroadcastToWatcher(*pkt);
		external_protocol_free_packet(pkt);
		ObjMgr.GridFreeByteArray(byte);
	}
	else
	{
		vector<int> fds;
		for (auto player:players)
		{
			if(player && player->GetSession())
				fds.push_back(player->GetSession()->GetFD());
		}
		ObjMgr.AsyncCompress(*byte, [this, fds](ByteArray *byte){
			byte->position(0);
			packet *pkt = external_protocol_new_packet(SMSG_GRID_UD_OBJECT);
			packet_write(pkt,(char*)byte->cur_data(),byte->bytesAvailable());
			update_packet_len(pkt);
			SendFunc f(pkt);
			std::for_each(fds.begin(),fds.end(),f);
			GridManager* gridMgr = map_inst->GetGridManager();
			if (gridMgr)
				gridMgr->BroadcastToWatcher(*pkt);
			external_protocol_free_packet(pkt);	
			ObjMgr.GridFreeByteArray(byte);
		});
	}
}

void Grid::ClearBlock()
{
	for (auto ptr:update_blocks)
	{
		ObjMgr.GridFreeByteArray(ptr);
	}
	for (auto ptr:out_area_blocks)
		ObjMgr.GridFreeByteArray(ptr);	
//	for_each(create_blocks.begin(),create_blocks.end(),safe_delete); 创建包内存可能存于多处，需要统一管理m_need_free
	update_blocks.clear();
	create_blocks.clear();
	out_area_blocks.clear();
}

void Grid::GetCreateBlocForNewPlayer(vector<ByteArray*>& ar, GridManager *mgr)
{
	auto f = [&mgr, &ar](Unit* wb){
		ByteArray *bytes = ObjMgr.GridMallocByteArray();
		wb->WriteCreateBlock(*bytes,gGridUpdateMask.create_int_mask_,gGridUpdateMask.create_string_mask_);
		ar.push_back(bytes);
		if(mgr)
			mgr->PushNeedFree(bytes);
	};
	for_each(players.begin(),players.end(),f);
	for_each(creatures.begin(),creatures.end(),f);
	for_each(worldobjs.begin(),worldobjs.end(),f);
	
	if(loot)
	{//将所有的Grid的信息也一起打包下去
		ByteArray *bytes = ObjMgr.GridMallocByteArray();
		loot->WriteCreateBlock(*bytes);
		ar.push_back(bytes);
		if(mgr)
			mgr->PushNeedFree(bytes);
	}
}

struct GetOutAreaBlockForPlayer_F
{
	GetOutAreaBlockForPlayer_F(vector<ByteArray*>& ar):ar_(ar)
	{
	}
	template<typename T>
	void operator()(T* wb)
	{
		ByteArray *bytes = ObjMgr.GridMallocByteArray();
		wb->WriteReleaseBlock(*bytes, wb->GetUIntGuid());
		ar_.push_back(bytes);
	}
	vector<ByteArray*>& ar_;
};

void Grid::GetOutAreaBlockForPlayer(vector<ByteArray*>& ar)
{	
	auto f = GetOutAreaBlockForPlayer_F(ar);
	for_each(players.begin(),players.end(),f);
	for_each(creatures.begin(),creatures.end(),f);
	for_each(worldobjs.begin(),worldobjs.end(),f);
	//TODO:f(loot->GetPublicObj());

	if(loot)
	{
		ByteArray *bytes = ObjMgr.GridMallocByteArray();
		loot->WriteReleaseBlock(*bytes, loot->GetUIntGuid());
		ar.push_back(bytes);
	}
}

void Grid::SendAllNoticeGridToNewPlayer(GridManager *mgr, Player* player)
{	
	//UpdateData ud;
	vector<ByteArray*> ud;

	for(GridPtrVec::iterator it = notice_grid.begin();it != notice_grid.end();++it)
		(*it)->GetCreateBlocForNewPlayer(ud, mgr);


	GridManager* gridMgr = map_inst->GetGridManager();
	if (gridMgr && player->GetSession())
	{
		gridMgr->SendUpdateDate(player->GetSession()->GetFD(), ud);
	}	
}

void Grid::Broadcast(packet& pkt,Player *player,bool self /*= false*/) const 
{
	GridPtrVec::const_iterator _it = notice_grid.begin();
	GridPtrVec::const_iterator _end = notice_grid.end();
	SendFunc f(&pkt);

	//先简单广播, 恶心的嵌套循环
	for(;_it != _end;++_it)
	{
		for(PlayerSet::const_iterator it = (*_it)->players.begin();
			it != (*_it)->players.end();++it)
		{
			Player *p = *it;
			if(!p)
				continue;
			if(p == player && self == false)
				continue;			
			f(p);		
		}

	}

	//给观察者也广播一份
	GridManager* gridMgr = map_inst->GetGridManager();
	if (gridMgr)
	{
		gridMgr->BroadcastToWatcher(pkt);
	}
}

void Grid::AsyncBroadcast(core_obj::ByteArray *pkt,Player *player /*= nullptr*/,bool self /* = false */) const
{
	GridPtrVec::const_iterator _it = notice_grid.begin();
	GridPtrVec::const_iterator _end = notice_grid.end();
	
	SendFunc f(*pkt);
	//先简单广播, 恶心的嵌套循环
	for(;_it != _end;++_it)
	{
		for(PlayerSet::const_iterator it = (*_it)->players.begin();
			it != (*_it)->players.end();++it)
		{
			Player *p = *it;			
			if(p == player && self == false)
				continue;
			f(p);
		}
	}
	//给观察者也广播一份
	GridManager* gridMgr = map_inst->GetGridManager();
	if (gridMgr)
	{
		gridMgr->BroadcastToWatcher(*pkt);
	}
}

void Grid::AddUpdateBlock(ByteArray* buf)
{
	ASSERT(buf->length() < 10240);
	update_blocks.insert(buf);
}
void Grid::AddCreateBlock(ByteArray* buf)
{
	ASSERT(active);
	ASSERT(buf->length() < 32768);
	create_blocks.insert(buf);		
}
void Grid::AddOutArea(Unit *wo)
{
	ByteArray *p = ObjMgr.GridMallocByteArray();
	wo->WriteReleaseBlock(*p,wo->GetUIntGuid());
	out_area_blocks.push_back(p);
}

//////////////////////////////////////////////////////////////////////////
//for GridManager

GridManager::GridManager(Map *m,int width,int height,int grid_width):m_width(width),m_height(height),m_grid_width(grid_width)
{
	//初始化grid
	//m_grid_width个地图网格组成一个敏感区域网格
	//宽度方向上的敏感区域网格数量
	m_gwidth = (m_width+m_grid_width-1)/m_grid_width;
	//高度方向上的敏感区域网格数量
	m_gheight = (m_height+m_grid_width-1)/m_grid_width;
	m_grids.resize(m_gwidth*m_gheight);

	int x=0,y=0;
	int index = 0;
	for (GridVec::iterator it = m_grids.begin();
		it != m_grids.end();
		++it)
	{
		it->map_inst = m;
		it->index = index;

		it->x = x;
		it->y = y;

		//更新grid的逻辑坐标
		if(m_grid_width == 65535)
		{
			ASSERT(!it->end_x);
			it->start_x = 0;
			it->end_x = width;
			it->start_y = 0;
			it->end_y = height;
		}
		else
		{
			it->start_x = m_grid_width * x;
			it->end_x = it->start_x + m_grid_width;
			it->start_y = m_grid_width * y ;
			it->end_y = it->start_y + m_grid_width;
		}

		it->active = false;		//休息状态

		//将相关联的grid存下来,便于
		CalcNoticeGrid(&(*it));

		//坐标跳跃
		x++;
		if(x !=0 && x%m_gwidth == 0)
		{
			x = 0;
			y++;
		}
		index++;
	}
}

GridManager::~GridManager()
{	
	for (auto ptr:m_need_free)
		ObjMgr.GridFreeByteArray(ptr);	
	m_need_free.clear();
	m_wo_need_refresh_grid.clear();
}

void GridManager::CalcNoticeGrid(Grid* grid)
{
	//最左上角的格子
	int sx = grid->x - 1;	
	int sy = grid->y - 1;

	for (int i=0; i< NOTICE_GRID_SIZE;i++)
	{
		int ix = sx + i%3;
		int iy = sy + i/3;

		if(ix<0 || iy<0)
			continue;

		if(ix >= m_gwidth || iy >= m_gheight)
			continue;

		grid->notice_grid.push_back(GetGrid(ix,iy));		
	}
}

//更新仿函数
struct Update_Func
{
	Update_Func(uint32 diff,GridManager& m_):diff_(diff),manager(m_)
	{}

	template<typename T>
	void operator()(T* t)	
	{
		ASSERT(t);
		t->Update(diff_);

		//TODO:这里性能还可以优化,如果坐标没变化不需要验证grid变化		
		//对grid下面的对象进行判断是否需要移动grid
		//判断坐标是否有变化,如果有变化更新grid
		ASSERT(t->GetGrid());
		if(t->GetGrid()->IsNotInThisGrid(uint16(t->GetPositionX()),uint16(t->GetPositionY())))
		{
			manager.m_wo_need_refresh_grid.push_back(t);
		}
	}

	//void operator()(Player* t)	
	//{
	//	ASSERT(t);		
	//	//TODO:这里性能还可以优化,如果坐标没变化不需要验证grid变化		
	//	//对grid下面的对象进行判断是否需要移动grid
	//	//判断坐标是否有变化,如果有变化更新grid
	//	if(t->GetGrid() && t->GetGrid()->IsNotInThisGrid(uint16(t->GetPositionX()),uint16(t->GetPositionY())))
	//	{
	//		manager.m_wo_need_refresh_grid.push_back(t);
	//	}
	//}

	uint32 diff_;
	GridManager& manager;
};

void GridManager::Update(uint32 diff)
{
	//战利品心跳
	for (auto it = m_grids.begin(); it != m_grids.end(); ++it)
	{
		if(it->loot)
			it->loot->Update(diff);
	}

	Update_Func func(diff,*this);

	//对grid的对象心跳,在此坐标会发生变化
	for (std::set<Grid*>::iterator it = m_active_grid.begin();
		it != m_active_grid.end();
		++it)
	{
		std::for_each((*it)->players.begin(),(*it)->players.end(),func);
		std::for_each((*it)->creatures.begin(),(*it)->creatures.end(),func);		
	}

	//根据坐标变化计算出现，需要刷新grid的对象
	std::for_each(m_wo_need_refresh_grid.begin(),m_wo_need_refresh_grid.end(),
		fastdelegate::MakeDelegate(this,&GridManager::RefreshGrid));
	m_wo_need_refresh_grid.clear();
	
	//观察对象变化,并且将所有对象的更新包加入到grid的更新数组
	std::for_each(m_active_grid.begin(),m_active_grid.end(),std::mem_fun(&Grid::ObjectAccess));	

	//将各个grid的对象更新包进行发送
	std::for_each(m_active_grid.begin(),m_active_grid.end(),std::mem_fun(&Grid::SendObjectUpdate));
	
	//发送完成后清空内存
	std::for_each(m_active_grid.begin(),m_active_grid.end(),std::mem_fun(&Grid::ClearBlock));	
	
	for (auto ptr:m_need_free)
	{
		ObjMgr.GridFreeByteArray(ptr);	
	}
	m_need_free.clear();
	//战斗结果包
	for(auto it:m_active_grid)
	{
		SendFightingBlocks(it);
	}
}

void GridManager::AddPlayer(Player *player)
{	
	ASSERT(player);
	player->Clear();//既然是新添加的，就没必要打更新包了
	Grid& grid = GridCoord(player->GetPositionX(),player->GetPositionY());

	//将网格上面的其他玩家信息发给他
	grid.SendAllNoticeGridToNewPlayer(this, player);	

	//激活grid
	RefreshGridIdle(grid);	

	//通知grid上面的其他玩家
	ByteArray *bytes = ObjMgr.GridMallocByteArray();
	player->WriteCreateBlock(*bytes);
	m_need_free.insert(bytes);
	
	Grid::GridPtrVec::iterator it = grid.notice_grid.begin();
	Grid::GridPtrVec::iterator end = grid.notice_grid.end();
	for(;it != end;++it)
	{
		SetGridIdleStatus(**it,true);
		(*it)->AddCreateBlock(bytes);
	}

	//增加到队列
	grid.players.insert(player);//!!
	player->SetGrid(&grid);	
	ASSERT(player->GetMap()->GetMapId() == player->GetMapId());
}

void GridManager::DelPlayer(Player *player)
{
	ASSERT(player);
	if(!player->GetGrid())
		return;

	Grid& grid = *(player->GetGrid());//GridCoord(player->GetPositionX(),player->GetPositionY());	

	//////////////////////////////////////////////////////////////////////////	
	//将九宫格里面的所有对象通知给当前玩家离开视野
	vector<ByteArray*> ud;
	for (auto g:grid.notice_grid)
	{
		g->GetOutAreaBlockForPlayer(ud);
		player->InvalidFriendlyCache(g);
	}
	
	//发送给
	if (player->GetSession())
		SendUpdateDate(player->GetSession()->GetFD(), ud);
	for (auto ptr:ud)
		ObjMgr.GridFreeByteArray(ptr);
	ud.clear();

	//////////////////////////////////////////////////////////////////////////	
	//删掉格子里面的当前玩家
	grid.players.erase(std::find_if(grid.players.begin(),grid.players.end(),
		std::bind2nd(std::equal_to<Player*>(),player)));

	//通知这个格子相关联的九宫格里面的所有玩家，当前玩家离开	
	auto it = grid.notice_grid.begin();
	auto end = grid.notice_grid.end();
	for(;it != end;++it)
	{		
		(*it)->AddOutArea(player);

		auto citer = (*it)->creatures.begin();
		auto cend = (*it)->creatures.end();
		for(; citer != cend; ++citer)
		{
			//如果怪物正在攻击此玩家，停止怪物攻击
			if((*citer)->GetTarget() == dynamic_cast<Unit*>(player))
			{
				(*citer)->SetTarget(NULL);
			}
			(*citer)->m_threatMgr.Del(player, true);

		}
	}

	//刷新网格状态
	RefreshGridIdle(grid);

	player->SetGrid(0);
}

bool GridManager::AddWatcher(uint32 fd, const string& owner_guid, uint32 map_id, uint32 instance_id)
{
	//把需要观察的地图信息告诉客户端
	packet *pkt = nullptr;
	pack_notice_watcher_map_info(&pkt, owner_guid.c_str(), map_id, instance_id);
	SendFunc f(pkt);
	f(fd);
	external_protocol_free_packet(pkt);

	for(auto it = m_active_grid.begin();it != m_active_grid.end();++it)
	{
		for (auto p:(*it)->players)
		{
			if (!p || !p->GetSession())
				continue;
			//观察者和被观察者在同一张地图
			if (p->GetSession()->GetFD() == fd)
				return false;			
		}
	}

	m_watchers.insert(fd);
	//将网格上面的其他玩家信息发给他(ps：支持观察者模式的地图必须是全图广播的)
	vector<ByteArray*> ud;
	for(auto it = m_active_grid.begin();it != m_active_grid.end();++it)
		(*it)->GetCreateBlocForNewPlayer(ud, this);

	SendUpdateDate(fd, ud);	
	return true;
}

void GridManager::DelWatcher(uint32 fd)
{
	m_watchers.erase(fd);
}

void GridManager::AddWorldObject(Unit *wo)
{
	ASSERT(wo);
	wo->Clear();//既然是新添加的，就没必要打更新包了
	Grid& grid = GridCoord(wo->GetPositionX(),wo->GetPositionY());
	if(wo->GetTypeId() == TYPEID_UNIT)
		grid.creatures.push_back(static_cast<Creature*>(wo));
	else
		grid.worldobjs.push_back(wo);
	wo->SetGrid(&grid);

	//如果可以激活grid
	if(wo->CanActiveGrid())
		RefreshGridIdle(grid);

	if(!grid.active)
		return;
	
	//加入对象更新块
	ByteArray *bytes = ObjMgr.GridMallocByteArray();
	wo->WriteCreateBlock(*bytes);
	m_need_free.insert(bytes);

	Grid::GridPtrVec::iterator it = grid.notice_grid.begin();
	Grid::GridPtrVec::iterator end = grid.notice_grid.end();
	for(;it != end;++it)	
	{
		if(!(*it)->active)
			continue;
		(*it)->AddCreateBlock(bytes);
	}
}

void GridManager::InitLoot(Grid &grid)
{
	if(grid.loot)
		return;
	grid.loot = new LootObject(&grid);
	grid.loot->Init(grid.map_inst, grid.index);

	//通知grid上面的其他玩家
	ByteArray *bytes = ObjMgr.GridMallocByteArray();
	grid.loot->WriteCreateBlock(*bytes);
	m_need_free.insert(bytes);

	Grid::GridPtrVec::iterator it = grid.notice_grid.begin();
	Grid::GridPtrVec::iterator end = grid.notice_grid.end();
	for(;it != end;++it)
	{
		SetGridIdleStatus(**it,true);
		(*it)->AddCreateBlock(bytes);
	}
}

//清空所有战利品
void GridManager::ClearAllLoot()
{
	//战利品心跳
	for (auto it = m_grids.begin(); it != m_grids.end(); ++it)
	{
		if(it->loot)
		{
			auto l = it->loot->m_has_loot;
			for (auto index : l)
			{
				it->loot->ClearLoot(index);			
			}
			it->loot->m_has_loot.clear();
		}
	}
}

Unit* GridManager::FindWorldObject(Unit* wo,const string &w_guid)
{
	ASSERT(wo);
	Grid& grid = GridCoord(wo->GetPositionX(),wo->GetPositionY());
	for(vector<Unit*>::iterator iter = grid.worldobjs.begin();
		iter!=grid.worldobjs.end();
		++iter)
	{
		if ((*iter)->GetGuid()==w_guid)
		{
			return (*iter);
		}
	}
	//当前格找不到找周围格
	vector<Grid*> ::iterator iter2 = grid.notice_grid.begin();
	vector<Grid*> ::iterator end = grid.notice_grid.end();
	for (;iter2!=end;++iter2)
	{
		for(vector<Unit*>::iterator iter_obj = (*iter2)->worldobjs.begin();
			iter_obj!= (*iter2)->worldobjs.end();
			++iter_obj)
		{
			if ((*iter_obj)->GetGuid().compare(w_guid) == 0)
			{
				return (*iter_obj);
			}
		}
	}
	return NULL;
}

void GridManager::DelWorldObject(Unit *wo)
{
	ASSERT(wo);
	ASSERT(wo->GetGrid());
	if(wo->GetTypeId() == TYPEID_UNIT)
	{
		Grid::CreatureList& cl = wo->GetGrid()->creatures;
		cl.erase(std::find_if(cl.begin(),cl.end(),std::bind2nd(std::equal_to<Creature*>(),static_cast<Creature*>(wo))));
	}
	else
	{
		Grid::WOList& wl = wo->GetGrid()->worldobjs;
		wl.erase(std::find_if(wl.begin(),wl.end(),std::bind2nd(std::equal_to<Unit*>(),wo)));
	}

	//离开视野
	if(wo->GetGrid()->active)
	{
		Grid::GridPtrVec::iterator it = wo->GetGrid()->notice_grid.begin();
		Grid::GridPtrVec::iterator end = wo->GetGrid()->notice_grid.end();
		for(;it != end;++it)		
			(*it)->AddOutArea(wo);		
	}

	wo->SetGrid(0);
}

void GridManager::RefreshGrid(Unit* wo)
{
	ASSERT(wo);
	//ASSERT(MapTemplate::GetMapTempalte(wo->GetMapId())->GetMapBaseInfo().width == m_width);
	//ASSERT(wo->GetMap()->GetMapId() == wo->GetMapId());
	if(wo->GetMap()->GetMapId() != wo->GetMapId())
	{
		tea_perror("GridManager::RefreshGrid wo->GetMap()->GetMapId() != wo->GetMapId() %u %u %u %u %s %s"
			, wo->GetMap()->GetMapId(), wo->GetMapId()
			, wo->GetEntry(), wo->GetTypeId(), wo->GetGuid().c_str(), wo->GetName().c_str());
		return;
	}
	//ASSERT(wo->GetPositionX() < m_width && wo->GetPositionX() >= 0.0f);
	if(wo->GetPositionX() >= m_width || wo->GetPositionX() < 0.0f)
	{
		tea_perror("GridManager::RefreshGrid wo->GetPositionX() < m_width && wo->GetPositionX() >= 0.0f %f %u %u %s %s"
			, wo->GetPositionX(), wo->GetEntry(), wo->GetTypeId(), wo->GetGuid().c_str(), wo->GetName().c_str());
		return;
	}
	//ASSERT(wo->GetPositionY() < m_height && wo->GetPositionY() >= 0.0f);
	if(wo->GetPositionY() >= m_height || wo->GetPositionY() < 0.0f)
	{
		tea_perror("GridManager::RefreshGrid wo->GetPositionY() < m_height && wo->GetPositionY() >= 0.0f %f %u %u %s %s"
			, wo->GetPositionY(), wo->GetEntry(), wo->GetTypeId(), wo->GetGuid().c_str(), wo->GetName().c_str());
		return;
	}
	Grid& old_grid = *wo->GetGrid();
	Grid& new_grid = GridCoord(wo->GetPositionX(),wo->GetPositionY());	

	//-------------------------------------计算两个grid关心区域的集合问题

	//创建块
	ByteArray *bytes = ObjMgr.GridMallocByteArray();
	wo->WriteCreateBlock(*bytes);
	m_need_free.insert(bytes);

	//--------------------------------------将新关心的grid中的对象通知该玩家
	vector<ByteArray*> ud;

	Grid::GridPtrVec::iterator it,it2;
	Grid::GridPtrVec::iterator end,end2;

	//---------------------------------刷新离开视野那一部分
	//先从原grid中去掉对象
	if(wo->GetTypeId() == TYPEID_PLAYER)
		old_grid.players.erase((Player*)wo);
	else if(wo->GetTypeId() == TYPEID_UNIT)
		old_grid.creatures.erase(std::find_if(old_grid.creatures.begin(),old_grid.creatures.end(),
			std::bind2nd(std::equal_to<Unit*>(),wo)));
	else
		old_grid.worldobjs.erase(std::find_if(old_grid.worldobjs.begin(),old_grid.worldobjs.end(),
			std::bind2nd(std::equal_to<Unit*>(),wo)));

	it = old_grid.notice_grid.begin();
	end = old_grid.notice_grid.end();
	for(;it != end; ++it)
	{
		//远离的3个grid
		if(abs((*it)->x - new_grid.x)>1 || abs((*it)->y - new_grid.y)>1)
		{			
			(*it)->AddOutArea(wo);

			//刷新状态
			if(!wo->CanActiveGrid())
				continue;

			//将其他grid对象离开当前玩家
			if(wo->GetTypeId() == TYPEID_PLAYER)
				(*it)->GetOutAreaBlockForPlayer(ud);

			//刷新grid状态
			RefreshGridStatus(**it);

			//让关联的这几个grid失效
			wo->InvalidFriendlyCache(*it);
		}
	}

	//---------------------------------刷新新加入部分
	it = new_grid.notice_grid.begin();
	end = new_grid.notice_grid.end();
	if(wo->CanActiveGrid() || new_grid.active)
	{
		for(; it != end; ++it)
		{
			//新接近的3个grid
			if(abs((*it)->x - old_grid.x)>1 || abs((*it)->y -old_grid.y)>1)
			{
				//设置为活动
				if((*it)->active == false && wo->CanActiveGrid())
					SetGridIdleStatus(**it,true);

				//将这些grid中的对象通知该grid
				if(wo->GetTypeId() == TYPEID_PLAYER)
					(*it)->GetCreateBlocForNewPlayer(ud);
				
				if((*it)->active)
					(*it)->AddCreateBlock(bytes);
			}
		}
	}

	//加入新的grid
	if(wo->GetTypeId() == TYPEID_PLAYER)
		new_grid.players.insert(static_cast<Player*>(wo));
	else if(wo->GetTypeId() == TYPEID_UNIT)
		new_grid.creatures.push_back(static_cast<Creature*>(wo));
	else
		new_grid.worldobjs.push_back(wo);
	wo->SetGrid(&new_grid);	

	//-----------------------------如果是玩家需要处理grid的状态
	//并且发送给他新grid的状态
	if(wo->GetTypeId() == TYPEID_PLAYER && !ud.empty())
	{
		ScenedContext *session = static_cast<Player*>(wo)->GetSession();
		if(session)
			SendUpdateDate(session->GetFD(), ud);		
	}

	//释放内存
	for (auto ptr:ud)
		ObjMgr.GridFreeByteArray(ptr);
	ud.clear();
}

void GridManager::RefreshGridStatus(Grid& grid)
{
	//刷新grid状态
	bool result =false;
	Grid::GridPtrVec::iterator it,end;
	it = grid.notice_grid.begin();
	end = grid.notice_grid.end();

	for(;it != end;++it)
	{
		if(!(*it)->players.empty())
		{
			result = true;
			break;
		}
		Grid::CreatureList::iterator citer = (*it)->creatures.begin();
		Grid::CreatureList::iterator cend = (*it)->creatures.end();
		for(;citer != cend;++citer)
		{
			if((*citer)->CanActiveGrid())
			{
				result = true;
				break;
			}	
		}
	}
	SetGridIdleStatus(grid,result);
}

void GridManager::RefreshGridIdle(Grid& grid)
{
	//如果网格附近没人则进入休眠状态

	Grid::GridPtrVec::iterator it,end;
	it = grid.notice_grid.begin();
	end = grid.notice_grid.end();	

	for(; it != end; ++it)
	{
		RefreshGridStatus(**it);
	}
}

void GridManager::SetGridIdleStatus(Grid& grid,bool active)
{
	/*if(grid.active == active)
		return;*/

	if(active)	
	{
		m_active_grid.insert(&grid);		
#ifdef _DEBUG
		for (Grid::BlockSet::iterator it = grid.create_blocks.begin();
			it != grid.create_blocks.end(); ++it)
		{
			ASSERT((*it)->length() < 10240);
		}		
#endif // _DEBUG
	}
	else
	{
		m_active_grid.erase(&grid);	
		grid.ClearBlock();
	}

	grid.active = active;
}

Grid* GridManager::GridCoordPtr(int x,int y)
{
	int gx = 0,gy = 0;
	gx = x/m_grid_width;
	gy = y/m_grid_width; 
	if(gx < 0 || gx >= m_gwidth)
		return nullptr;
	if(gy < 0 || gy >= m_gheight)
		return nullptr;
	return &m_grids[gy*m_gwidth+gx];
} 

void GridManager::SendFightingBlocks(Grid *grid)
{
	auto fun = [&](int opcode, int not_compree_opcode, vector<ByteArray*>& blocks){
		if(blocks.empty())
			return;
		
		ByteArray *buff = ObjMgr.GridMallocByteArray();
		buff->writeShort(blocks.size());
		for (auto it:blocks)
		{
			it->position(0);
			buff->writeBytes(*it);
			ObjMgr.GridFreeByteArray(it);
		}
		blocks.clear();
		buff->position(0);


		if(buff->length() < COMPRESS_MIN_SIZE)
		{
			ByteArray *pkt = ObjMgr.GridMallocByteArray();
			pkt->writeShort(not_compree_opcode);
			pkt->writeBytes(buff->cur_data(),buff->bytesAvailable());
			ObjMgr.GridFreeByteArray(buff);
			grid->AsyncBroadcast(pkt);
			ObjMgr.GridFreeByteArray(pkt);			
		}
		else
		{
			int a= 0;
			ObjMgr.AsyncCompress(*buff, [opcode, grid, this](ByteArray *buff){
				buff->position(0);
				packet *spkt = internal_protocol_new_packet(opcode); 				
 				packet_write(spkt, (char*)buff->cur_data(), buff->bytesAvailable());
				update_packet_len(spkt);
				grid->Broadcast(*spkt);
				internal_protocol_free_packet(spkt);
			});
		}
	};
	//fun(SMSG_FIGHTING_INFO_UPDATE_OBJECT_2, SMSG_FIGHTING_INFO_UPDATE_OBJECT, grid->fighting_blocks);
	fun(SMSG_GRID_UNIT_MOVE_2, SMSG_GRID_UNIT_MOVE, grid->unit_move_blocks);
	fun(SMSG_GRID_UNIT_JUMP, SMSG_GRID_UNIT_JUMP, grid->unit_jump_blocks);
}

//广播给观察者
void GridManager::BroadcastToWatcher(packet& pkt)
{
	if (m_watchers.empty())
		return;

	SendFunc f(&pkt);
	//给观察者也广播一份
	std::for_each(m_watchers.begin(), m_watchers.end(), [&](uint32 fd){
		f(fd);
	});	
}

void GridManager::BroadcastToWatcher(ByteArray& pkt)
{
	if (m_watchers.empty())
		return;

	SendFunc f(pkt);
	//给观察者也广播一份
	std::for_each(m_watchers.begin(), m_watchers.end(), [&](uint32 fd){
		f(fd);
	});
}


void GridManager::SendUpdateDate(uint32 fd, vector<ByteArray*>& ud)
{
	if (fd == 0)
		return;
	const uint16 PAGE_SIZE = 128;
	uint16 pos = 0;	
	while(pos < ud.size())
	{
		ByteArray *byte = ObjMgr.GridMallocByteArray();
		for (uint16 i=pos; i < pos+PAGE_SIZE;i++)
		{
			//当不足一页的情况
			if(i >= ud.size())
				break;
			ud[i]->position(0);
			byte->writeBytes(*ud[i]);		
		}
		byte->position(0);
		ObjMgr.Compress(*byte);
		packet *pkt = external_protocol_new_packet(SMSG_GRID_UD_OBJECT);
		byte->position(0);
		packet_write(pkt,(char*)byte->cur_data(),byte->bytesAvailable());
		ObjMgr.GridFreeByteArray(byte);
		update_packet_len(pkt);
		SendFunc f(pkt);
		f(fd);
		external_protocol_free_packet(pkt);
		pos += PAGE_SIZE;
	}
}


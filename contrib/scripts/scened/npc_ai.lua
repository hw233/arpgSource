--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--NPC AI

--增加一个选项进指定table
function AddOptionMenu(options,id,icon,title)
	table.insert(options,id)
	table.insert(options,icon)
	table.insert(options,title)
end

--------------------------------------------------------------------------------------
--npc ai
--Npc
AI_Npc_Base = {
	ainame = 'AI_Npc_Base',
	GossipMessage =
		function(self,owner,player,option_id)
			local creature = UnitInfo:new{ptr = owner}	
			local entry = creature:GetEntry()
			local options = {}
			local text = ""
			if(option_id == 0) then
				playerLib.TalkedToCreature(player,owner)				
				text = tb_creature_template[entry].dialogue or ""
				return text,options
			end
		end
}
__inherit__(AI_Npc_Base,AI_Base)

AI_duihua= {
	ainame = 'AI_duihua',	
	JustDied =									--当被别人杀死前
		function(self,map,owner,killer_ptr)
			local ownerInfo = UnitInfo:new{ptr = owner}
			outFmtError("------>AI_duihua.JustDied entry %d",ownerInfo:GetEntry())
			if killer_ptr then
				local killInfo = UnitInfo:new{ptr = killer_ptr}
				if GetUnitTypeID(killer_ptr) == TYPEID_PLAYER then
					outFmtError("------>AI_duihua.JustDied killInfo %s %s",killInfo:GetGuid(),killInfo:GetName())
				else
					outFmtError("------>AI_duihua.JustDied killInfo.entry %d",killInfo:GetEntry())
				end
			end
			--统计全服杀怪人数
			return 0							--返回尸体的存活时间
		end,
}
__inherit__(AI_duihua,AI_Npc_Base)
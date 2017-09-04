-- function PlayerInfo:Handle_questhelp_canaccept_list(pkt)
-- 	main_quest_id = 0
-- 	for i = 1,#pkt.quests,1
-- 	do
-- 		tmp_quest = quest_template.getTableById(pkt.quests[i])
-- 		if(tmp_quest~=nil and tmp_quest.quest_type==QUEST_TYPE_MAIN)then
-- 			outFmtDebug("=====goto accept quest_id:%u",tmp_quest.id)
-- 			self:call_questgiver_accept_quest(tmp_quest.accept_creature,tmp_quest.id)
-- 			break
-- 		end
-- 	end
-- end

--函数包路由表
local OpcodeHandlerFuncTable = require 'robotd.context.robotd_context_hanlder_map'

local protocols = require('share.protocols')


--包路由
function PlayerInfo:Handle(pkt)	
	local optcode = packet.optcode(pkt)
	local succeed, args = protocols.unpack_packet(optcode, pkt)

	--解包失败记一下日志
	if not succeed then
		--logLib.WriteAttackPacker(self:GetGuid(), optcode, ACCACK_PACKET_TYPE_UNPACK, '')
		outFmtDebug("=====opcode error %s",optcode)
		return
	else
		args.__optcode = optcode		
		if OpcodeHandlerFuncTable[optcode] then
			OpcodeHandlerFuncTable[optcode](self, args)
		end

		self:ActionOptHandle(args)
	end
end

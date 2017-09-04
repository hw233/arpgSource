
--------------------------------------------------------
--标签管理器
--------------------------------------------------------
local TagsManager = class('TagsManager')
TagsManager.__index = TagsManager

function TagsManager:ctor( onSet, onUnSet )
	self.tags_guids_ = {}
	self.guids_tags_ = {}
	--当设置成功时回调
	self.__onSet = onSet
	self.__onUnSet = onUnSet
end

function TagsManager:setTag( guid, tag )
	if not self.tags_guids_[tag] then
		self.tags_guids_[tag] = {}
	end
	if not self.guids_tags_[guid] then
		self.guids_tags_[guid] = {}
	end
	
	if self.guids_tags_[guid][tag] and self.guids_tags_[guid][tag] then
		return
	end

	self.tags_guids_[tag][guid] = true
	self.guids_tags_[guid][tag] = true

	--如果设置成功才触发回调
	if self.__onSet then
		self.__onSet(guid, tag)
	end
end

function TagsManager:unsetTag( guid, tag )
	--如果本来就是true的话就需要触发回调
	if not self.tags_guids_[tag][guid] and not self.guids_tags_[guid][tag] then	
		return	
	end
	self.tags_guids_[tag][guid] = nil
	self.guids_tags_[guid][tag] = nil
	if self.__onUnSet then
		self.__onUnSet(guid, tag)
	end
end

function TagsManager:remove( guid )
	local strSet = self.guids_tags_[guid]
	if (strSet == nil) then
		return
	end

	--删除存储着这个guid的tag
	for k,_ in pairs(strSet) do
		if self.tags_guids_[k][guid] then
			if self.__onUnSet then
				self.__onUnSet(guid, k)
			end
			self.tags_guids_[k][guid] = nil			
		end
	end

	--删除整个以guid为key的tag列表
	self.guids_tags_[guid] = nil
end

--遍历这个标签下面的所有记录
function TagsManager:forEach( tag, f )
	local t = self.tags_guids_[tag]
	if ( t == nil) then
		return
	end
	for k,v in pairs(t) do
		if (v) then
			f(k)
		end
    end
end

--传入查找函数，如果找得到则返回key,如果找不到则返回nil
function TagsManager:find( tag, f )
	local t = self.tags_guids_[tag]
	if ( t == nil) then
		return
	end
	for k,v in pairs(t) do
		if (v) then
			if f(k) then
				return k
			end
		end
	end
	return nil
end

return TagsManager

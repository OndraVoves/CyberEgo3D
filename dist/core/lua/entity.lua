local function new( obj )
	local cls = { name=obj.name or "EmptyName",
				  world_id=obj.world_id or -1,
				  attr=obj.attr or {},
				  components=obj.components or {},
	}
	
	function cls:setAttr( name, value )
		local old = self.attr[name]
		self.attr[name] = value
		
		--TODO: Notify component
		for k,v in pairs(self.components) do
			if v['onAttrChange' ] then
				v:onAttrChange( name, old, value )
			end
		end
	end
	
	function cls:getDataTbl()
		local r = { name=self.name or "EmptyName",
				    world_id=self.world_id or -1,
				    attr=self.attr or {},
				    components={}			
		}
		
		for k,v in pairs(self.components) do
			r.components[#r.components+1] = v.types
		end
		
		return r
	end	
	
	local mt = { __index=cls }
	return setmetatable( cls, mt)
end

return new
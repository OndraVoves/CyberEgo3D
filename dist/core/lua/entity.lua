local function new( obj )
	local cls = { name=obj.name or "EmptyName",
				  world_id=obj.world_id or -1,
				  attr=obj.attr or {},
				  components=obj.components or {},
				  data={},
	}

	function cls:initAttr( name, default_value )
		if not self.attr.name then
			self:setAttr( name, default_value )
		end
	end
		
	function cls:setAttr( name, value )
		local old = self.attr[name]
		self.attr[name] = value
		
		for k,v in pairs(self.components) do
			if v.onAttrChange ~= nil then
				v:onAttrChange( name, old, value )
			end
		end
	end
	
	function cls:toJSON()
		local r = { name=self.name or "EmptyName",
				    world_id=self.world_id or -1,
				    attr=self.attr or {},
				    components={}			
		}
		
		-- save only component types
		for k,v in pairs(self.components) do
			r.components[#r.components+1] = v.types
		end
		
		local json = require( "json" )
		local e = json.encode
		return e(r)
	end	
	
	local mt = { __index=cls.__index }
	return setmetatable( cls, mt)
end

return new
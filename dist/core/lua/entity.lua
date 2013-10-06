local function new( obj )
	local cls = { name=obj.name or "EmptyName",
				  world_id=obj.world_id or -1,
				  attr=obj.attr or {},
				  components={},
				  data={},
	}

	if obj.components then
		local cs = obj.components
		for i=1, #cs do
			local c = ce3d.Components[ cs[i] ]
			if c then
				cls.components[#cls.components+1] = c()
			end
		end
	end

	function cls:initAttr( name, default_value )
		if not self.attr[name] then
			self:setAttr( name, default_value )
		else
			local cs = self.components
			for i=1, #cs do
				local csi = cs[i]
				local c = csi.onAttrChange 
				if c then
					c( csi, name, value, value)
				end
			end
		end
	end
	
	function cls:setAttr( name, value )
		self.attr[name] = value
		
		local cs = self.components
		for i=1, #cs do
			local csi = cs[i]
			local c = csi.onAttrChange 
			if c then
				c( csi, name, value, value)
			end
		end
	end
			
	function cls:toTable()
		local r = { name=self.name or "EmptyName",
				    world_id=self.world_id or -1,
				    attr=self.attr or {},
				    components={}			
		}
		
		-- save only component types
		for k,v in pairs(self.components) do
			r.components[#r.components+1] = v.types
		end
		
		return r	
	end
			
	function cls:toJSON()
		local r = self:toTable()
		
		local json = require( "json" )
		local e = json.encode
		return e(r)
	end	
	
	local mt = { __index=cls.__index }
	return setmetatable( cls, mt )
end

return new
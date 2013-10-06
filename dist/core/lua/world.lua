local function new()
	-- Class data
	local cls = {
		entities = {}
	}
	
	-- Class fce
	
	--- add entity
	-- @param ent Entity data table
	function cls:addEntity( ent )
		local count = #self.entities+1
		
		ent.world_id = count
		
		local Entity = require( "entity" )
		local entity = Entity( ent )
		
		self.entities[count] = entity

		local components = entity.components
		for i=1, #components do
			components[i]:onEntityChange( entity )
		end
		
		return entity
	end
	
	-- Class fce
	--- get entity
	-- @param id world_id
	function cls:getEntity( id )
		return self.entities[id]
	end

	function cls:toTable()
		local r = {}
		for i,v in ipairs( self.entities ) do
			r[i] = v:toTable()
		end
		
		return r
	end

	function cls:toJSON()
		local r = self:toTable()
		
		local json = require( "json" )
		local e = json.encode
		
		return e(r)		
	end

	function cls:clientTick( dt )
		local v = self.entities
		for i = 1, #v do
			local c = v[i].components
			for j = 1, #c do
				c[j]:onClientTick( dt )
			end
		end			
	end
	
	function cls:serverTick( dt )
		local v = self.entities
		for i = 1, #v do
			local c = v[i].components
			for j = 1, #c do
				c[j]:onServerTick( dt )
			end
		end	
	end		
	
	
	-- Class end		
	local mt = { __index=cls }
	return setmetatable( cls, mt)
end

return new
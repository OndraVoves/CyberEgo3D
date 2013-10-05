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
		
		for k,v in pairs(entity.components) do
			if v.onEntityChange ~= nil then
				v:onEntityChange( entity )
			end
		end	
		
		return entity
	end
	
	-- Class fce
	--- get entity
	-- @param id world_id
	function cls:getEntity( id )
		return self.entities[id]
	end

	function cls:clientTick( dt )
		for i,v in ipairs( self.entities ) do
			for k, v in pairs( v.components ) do
				v:onClientTick( dt )
			end
		end			
	end
	
	function cls:serverTick( dt )
		for i,v in ipairs( self.entities ) do
			for k, v in pairs( v.components ) do
				v:onServerTick( dt )
			end
		end			
	end		
	-- Class end		
	local mt = { __index=cls }
	return setmetatable( cls, mt)
end

return new
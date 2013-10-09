function new( entity_class ) 
	local Entity = {
		name = 'NoName',
		id = nil,
		components = {},
		data = {},
	}

	-- Name
	function Entity:setName( name )
		self.name = name
	end
	function Entity:getName( name )
		return self.name
	end

	function Entity:save()
		local t = {}
		
		-- TODO: fce arg?
		local ingore = {
			components=1,
			data=1,
			id=1,
		}
		
		for k, v in pairs( self ) do
			if type( v ) ~= 'function' and not ingore[k] then
				t[k] = v
			end
		end		
		
		local c = {}
		for k, v in pairs(self.components) do
			c[#c+1] = k
		end
		t.components = c
				
		return t
	end

	function Entity:_onDestroy()
		-- Init components
		for k, v in pairs( self.components ) do
			v:onDestroy()
		end

		if self.onDestroy then
			self:onDestroy()
		end				
	end

	function Entity:_onSpawn()
		-- Init components
		for k, v in pairs( self.components ) do
			v:onInit( self.id )
		end
			
		if self.onSpawn then
			self:onSpawn()
		end
	end

	-- Copy table
	for k, v in pairs( entity_class ) do
		if k ~= "components" and k ~= "attributes" then
			Entity[k] = v
		end
	end
	
	local function attr_copy( table )	
		local t = {}
		
		for k, v in pairs( table ) do
			if type(value) == "table" then
				t[k] = attr_copy ( v )
			else
				t[k] = v
			end
		end
		
		return t
	end	
	
	-- copy attributes
	Entity.attributes = attr_copy( entity_class.attributes )	
	
	-- Create components	
	if entity_class.components then	
		local class_components = entity_class.components
		local cf = ComponentSystem
		
		for i=1, #class_components do
			local class = class_components[i]
			local c = cf:newInstance( class )
			Entity.components[class] = c
		end
	end	


	local mt = {
--		__index = Entity.__index
	}
	return setmetatable( Entity, mt )
end

return new
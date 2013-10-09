function new( entity_class ) 
	local Entity = {
		name = 'NoName',
		id = nil,
		components = {},
		data = {},
	}

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
		if k ~= "components" then
			Entity[k] = v
		end
	end
	
	-- Create components	
	if entity_class.components then	
		local class_components = entity_class.components
		local cf = ComponentsFactory
		
		for i=1, #class_components do
			local class = class_components[i]
			local c = cf:newInstance( class )
			Entity.components[class] = c
		end
	end	

	local mt = {
		__index = Entity
	}
	return setmetatable( Entity, mt )
end

return new
local Entity = require('Entity')

local EntitySystem = {
	_factory = {},
	_Entities = {}
}

function EntitySystem:register( cls )
	self._factory[cls.class] = cls
end

-- Spawn entity
function EntitySystem:spawn( cls )
	local ent = Entity( self._factory[cls] )
	local id = #self._Entities+1
	
	self._Entities[id] = ent
	
	ent.id = id
	ent:_onSpawn()
	
	return id
end 

function EntitySystem:remove( id )
	-- TODO
end

--get entity
function EntitySystem:getEntity( id )
	return self._Entities[id]
end

function EntitySystem:getEntityByName( name )
	local ents = sels._Entities
	local count = #sels._Entities
	for i=1, count do
		if ents[i].name == name then
			return ents[i] 
		end
	end

	return nil
end


local mt = {
	-- If key is number then return entity
	-- Else return value from EntitySystem
	__index = function( t, k )
		if type(k) == 'number' then
			return t._Entities[ k ]
		else
			return rawget( t, k )
		end
	end
}
	 
return setmetatable( EntitySystem, mt )

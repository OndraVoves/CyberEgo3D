local Entity = require('Entity')

local EntitySystem = {
	_factory = {},
	_Entities = {}
}

function EntitySystem:loadFromDirs( dir )
	local popen = io.popen
	
	for filename in popen('ls -a "'..dir..'"'):lines() do
		if filename ~= '.' and filename ~= '..' then
			local ent_tbl = dofile( dir..filename )
			self:register( ent_tbl )
		end
	end	
end

function EntitySystem:register( cls )
	self._factory[cls.class] = cls
end

function EntitySystem:reloadFactory()
	self._factory = {}
	
	self:loadFromDirs( './game/entities/' )
end

function EntitySystem:update( dt )
	local ents = self._Entities
	local count = #self._Entities
	for i=1, count do
		local e = ents[i]
		if e.onUpdate then
			e:onUpdate( dt )
		end 
	end
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

local function attr_copy( table1,  table2 )	
	for k, v in pairs( table2 ) do
		if type(v) == "table" then
			attr_copy ( table1[k],  v )
		elseif type(v) ~= "function" then
			table1[k] = v
		end
	end	
end	

function EntitySystem:spawnFromTable( table )
	local t = self._factory[table.class]
	
	-- TODO: fce arg?	
	for k, v in pairs( table ) do
		if type( v ) ~= 'function' then
			if k ~= "attributes" then
				t[k] = v
			else
				attr_copy( t[k], v )
			end
		end
	end
	
	local ent = Entity( t )
	local id = #self._Entities+1
	self._Entities[id] = ent
	ent.id = id
	ent:_onSpawn()
	
	return id
end

function EntitySystem:destroyAll()
	local ents = self._Entities
	local count = #self._Entities
	
	for i=1, count do
		ents[i]:_onDestroy()
	end
	
	self._Entities = {}
end

function EntitySystem:remove( id )
	-- TODO
end

--get entity
function EntitySystem:getEntity( id )
	return self._Entities[id]
end

function EntitySystem:getEntityByName( name )
	local ents = self._Entities
	local count = #self._Entities
	for i=1, count do
		if ents[i].name == name then
			return ents[i] 
		end
	end

	return nil
end

function EntitySystem:save()
	local ents = self._Entities
	local count = #self._Entities
	local t = {}
	for i=1, count do
		t[#t+1] = ents[i]:save()
	end
	
	return t
end

function EntitySystem:load( array )
	self:destroyAll()

	for i=1, #array do
		self:spawnFromTable( array[i] )
	end
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

local ComponentSystem = {
	_factory = {}
}

function ComponentSystem:loadFromDirs( dir )
	local popen = io.popen

	for filename in popen('ls -a "'..dir..'"'):lines() do
		if filename ~= '.' and filename ~= '..' then
			local comp_new = dofile( dir..filename )
			local comp_class = string.gsub( filename, "%.lua$", "")

			self:register(comp_class, comp_new )
		end
	end
end

function ComponentSystem:reloadFactory()
	self._factory = {}

	ComponentSystem:loadFromDirs( './core/scripts/components/' )
	ComponentSystem:loadFromDirs( './game/scripts/components/' )
end

function ComponentSystem:register( class, new )
	self._factory[class] = new
end

function ComponentSystem:newInstance( cls )
	return self._factory[ cls ]()
end

local mt = {
	__index = ComponentSystem,
}

return setmetatable( ComponentSystem, mt )

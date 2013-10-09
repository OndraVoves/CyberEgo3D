local ComponentsFactory = {
	_factory = {}
}



function ComponentsFactory:loadFromDirs( dir )
	local popen = io.popen
	
	for filename in popen('ls -a "'..dir..'"'):lines() do
		if filename ~= '.' and filename ~= '..' then
			local comp_new = dofile( dir..filename )
			local comp_class = string.gsub( filename, "%.lua$", "")

			self:register(comp_class, comp_new )
		end
	end	
end

function ComponentsFactory:register( class, new )
	self._factory[class] = new
end

function ComponentsFactory:newInstance( cls )
	return self._factory[ cls ]()
end 

local mt = {
	__index = ComponentsFactory,
}
	 
return setmetatable( ComponentsFactory, mt )

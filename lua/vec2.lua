-- version from Programming in Lua book 
vec2 = {x=0, y=0}

function vec2:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

-- vec2 test
v = vec2:new{x=10, y=15}
print('(' .. v.x .. ', ' .. v.y .. ')')

-- version from Object Orientation Tutorial
vec2_alt = {x=0, y=0}

vec2_alt.__index = vec2_alt

setmetatable(vec2_alt, {
	__call = function (cls, ...)
		return cls._init(...)
		end,
	})

function vec2_alt._init(o)
	o = o or {}
	setmetatable(o, vec2_alt)
	return o
end

-- test vec2_alt
u = vec2_alt{x=11, y=3}
print('(' .. u.x .. ', ' .. u.y .. ')')

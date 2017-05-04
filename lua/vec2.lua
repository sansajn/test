 -- implementacia vec2 modulu

vec2 = {}
vec2.__index = vec2

function vec2.new(x, y)
	return setmetatable({x = x or 0, y = y or 0}, vec2)
end

function vec2:clone()
	return vec2.new(self.x, self.y)
end

function vec2:dot(v)
	return self.x*v.x + self.y*v.y
end

function vec2:cross(v)
	-- vektorovy sucin 2d vektorou ma smer (0,0,1), tzn. ukazuje do z roviny
	return self.x*v.y - self.y*v.x 
end

function vec2:normalized()
	return self / self:length()
end

function vec2:normalize()
	local len = self:length()
	self.x, self.y = self.x / len, self.y / len
end

function vec2:length()
	return math.sqrt(self:length2())
end

function vec2:length2()
	return self.x*self.x + self.y*self.y
end

function vec2:distance(v)
	return (v - self):length()
end

function vec2:roteted(ang)
	return self:clone():rotate(ang)
end

function vec2:rotate(ang)
	local c = math.cos(ang)
	local s = math.sin(ang)
	self.x, self.y = c*self.x - s*self.y, s*self.x + c*self.y
end


function vec2.__add(u, v)
	-- scalar add
	if type(u) == 'number' then
		return vec2.new(u + v.x, u + v.y)
	elseif type(v) == 'number' then
		return vec2.new(u.x + v, u.y + v)
	else
		return vec2.new(u.x + v.x, u.y + v.y)
	end
end

function vec2.__sub(u, v)
	-- scalar sub
	if type(u) == 'number' then
		return vec2.new(u - v.x, u - v.y)
	elseif type(v) == 'number' then
		return vec2.new(u.x - v, u.y - v)
	else
		return vec2.new(u.x - v.x, u.y - v.y)
	end
end

-- scalar multiple and dot product
function vec2.__mul(u, v)
	-- scalar mul
	if type(u) == 'number' then
		return vec2.new(u*v.x, u*v.y)
	elseif type(v) == 'number' then
		return vec2.new(u.x*v, u.y*v)
	else
		return u.x*v.x + u.y*v.y
	end
end

-- scalar divide
function vec2.__div(u, v)
	assert(type(u) == 'table' and type(v) == 'number', 
		'only division by scalar supported')

	return vec2.new(u.x/v, u.y/v)
end

function vec2.__eq(u, v)
	return u.x == v.x and u.y == v.y
end

function vec2.__lt(u, v)
	return u.x < v.x or (u.x == v.x and u.y < v.y)
end

function vec2.__le(u, v)
	return u.x <= v.x and u.y <= v.y 
end

function vec2.__tostring(v)
	return '(' .. v.x .. ', ' .. v.y .. ')'
end

setmetatable(vec2, {
	__call = function(_, ...)
		return vec2.new(...)
	end
	})


return vec2

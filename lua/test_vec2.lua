-- pouzitie vec2 modulu 
require('vec2')
luaunit = require('luaunit')


function testCreate()
	local u = vec2(1, 2)
	luaunit.assertEquals(u.x, 1)
	luaunit.assertEquals(u.y, 2)
end

function testEq()
	luaunit.assertEquals(vec2(1,5), vec2(1,5))
end

function testAdd()
	local u = vec2(1, 2)
	local v = vec2(9, 18)
	local w = u+v
	luaunit.assertEquals(w.x, 10)
	luaunit.assertEquals(w.y, 20)
end

function testSub()
	local u = vec2(1, 2)
	local v = vec2(9, 18)
	local w = u-v
	luaunit.assertEquals(w.x, -8)
	luaunit.assertEquals(w.y, -16)
end

function testMul()
	local u = vec2(2, 7)
	local v = 2*u
	luaunit.assertEquals(v, vec2(4, 14))

	v = u*3
	luaunit.assertEquals(v, vec2(6, 21))

	v = vec2(2, 3)
	local w = u*v
	luaunit.assertEquals(w, 25)
end

function testDiv()
	local u =  vec2(6, 12)
	luaunit.assertEquals(u/3, vec2(2,4))
end

function testCompare()
	local v = vec2(1,2)
	local u = vec2(3,1)
	local w = vec2(1,1)
	local s = vec2(1,2)

	luaunit.assertTrue(v < u)
	luaunit.assertFalse(v > u)
	luaunit.assertFalse(v == u)
	luaunit.assertEquals(v, s)
	luaunit.assertTrue(u > v)
	luaunit.assertTrue(w < v)
end

function testNormalized()
	local v = vec2(1,1)
	local n = v:normalized()
	-- how to test implecise double values ?
end

luaunit.LuaUnit:run()

--[[
-- test
u = vec2(19, 1)
print('(' .. u.x .. ', ' .. u.y .. ')')

v = vec2(1, 19)
print('(' .. v.x .. ', ' .. v.y .. ')')

w = u+v
print('w:' .. tostring(w))
--]]

-- pouzitie vec2 modulu 
require('vec2')

-- test
u = vec2(19, 1)
print('(' .. u.x .. ', ' .. u.y .. ')')

v = vec2(1, 19)
print('(' .. v.x .. ', ' .. v.y .. ')')

w = u+v
print('w:' .. tostring(w))

-- love game template

local angle = 0
local width = 10
local height = 10

-- load game assets
function love.load()
	-- load assets there
end

-- update event
function love.update(dt)
	if love.keyboard.isDown('d') then
		angle = angle + math.pi*dt
	elseif love.keyboard.isDown('a') then
		angle = angle - math.pi*dt
	end
end

-- draw display
function love.draw()
	love.graphics.rotate(angle)
	love.graphics.setColor(0,0,255)
	love.graphics.rectangle('fill', 300, 400, width, height)
end

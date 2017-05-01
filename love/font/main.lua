-- love game template

-- load game assets
function love.load()
	local font = love.graphics.newFont(45)
	love.graphics.setFont(font)
	love.graphics.setColor(255,0,0,255)
	love.graphics.getBackgroundColor(255, 153, 0)
end

-- update event
function love.update(dt)
	-- do math
end

-- draw display
function love.draw()
	love.graphics.print('Hello World', 200, 200)
end

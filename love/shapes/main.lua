-- love game template

-- load game assets
function love.load()
	love.graphics.setColor(0, 0, 0, 255)
	love.graphics.setBackgroundColor(255, 153, 0)
end

-- update event
function love.update(dt)
	-- do math
end

-- draw display
function love.draw()
	love.graphics.circle('fill', 200, 300, 50, 50)
	love.graphics.rectangle('fill', 300, 300, 100, 100)
	love.graphics.arc('fill', 450, 300, 100, math.pi/5, math.pi/2)
end

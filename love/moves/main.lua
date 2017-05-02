
speed = 100

-- load game assets
function love.load()
	character = {}
	character.x = 300
	character.y = 400
	love.graphics.setBackgroundColor(255, 153, 0)
	love.graphics.setColor(0, 0, 255)
end

-- update event
function love.update(dt)
	-- left/right
	if love.keyboard.isDown('d') then
		character.x = character.x + speed*dt
	elseif love.keyboard.isDown('a') then
		character.x = character.x - speed*dt
	end

	-- up/down
	if love.keyboard.isDown('w') then
		character.y = character.y - speed*dt
	elseif love.keyboard.isDown('s') then
		character.y = character.y + speed*dt
	end
end

-- draw display
function love.draw()
	love.graphics.rectangle('fill', character.x, character.y, 100, 100)
end


speed = 100

-- load game assets
function love.load()
	character = {}
	character.x = 300
	character.y = 400

	sprite = love.graphics.newImage('sprite.jpg')
	character.sprite = sprite

	character.quad = love.graphics.newQuad(
		character.x, character.y, sprite:getWidth(), sprite:getHeight(), sprite:getDimensions())
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
	love.graphics.draw(character.sprite, character.x, character.y, 0, 0.25, 0.25)
end

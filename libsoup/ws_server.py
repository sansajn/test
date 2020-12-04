# wesockets server sample from *getting started tutorial*
# install websockets library with `sudo pip3 install websockets` command
import asyncio
import websockets

async def hello(websocket, path):
	while True:
		name = await websocket.recv()
		print(f'< {name}')

		greeting = f'Hello {name}!'

		await websocket.send(greeting)
		print(f'> {greeting}')

start_server = websockets.serve(hello, 'localhost', 40001)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()

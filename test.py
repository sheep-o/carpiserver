import asyncio
import websockets
from PIL import Image
import io

async def send_image(websocket, image_path):
    try:
        # Open and read the image
        with open(image_path, 'rb') as image_file:
            image_data = image_file.read()

        # Send the image as binary data
        await websocket.send(image_data)

        print(f"Image sent: {image_path}")
    except Exception as e:
        print(f"Error sending image: {str(e)}")

async def main():
    server_uri = "ws://localhost:8000"  # Replace with the WebSocket server URI
    image_path = "lane.png"  # Replace with the path to your image

    async with websockets.connect(server_uri) as websocket:
        await send_image(websocket, image_path)

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())

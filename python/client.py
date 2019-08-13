# client
import socket

TCP_IP = '127.0.0.1'
TCP_PORT = 5005
BUFFER_SIZE = 1024
MESSAGE = "Hello, World!".encode()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv(BUFFER_SIZE)
s.close()

print("received data:" + data.decode())

# Test client with Python3. Polls the Python3 server.
#from sockets.python3.client import Client
#client = Client()
#response, addr = client.poll_server(11, server=('127.0.0.1', 11113))
#print(response, addr)

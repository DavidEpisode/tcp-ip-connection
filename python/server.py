# server
import socket

TCP_IP = '127.0.0.1'
TCP_PORT = 5005
BUFFER_SIZE = 20
MESSAGE = "I got!".encode()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)


conn, addr = s.accept()
print('Connection address:%s, %d' % (TCP_IP, TCP_PORT))
while 1:
 data = conn.recv(BUFFER_SIZE)
 if not data: break
 print("received data:" + data.decode())
 
 conn.send(MESSAGE)
conn.close()
#server
#from sockets.python3.server import Server
#class MyServer(Server):
#    def act_on(self, data, addr):
#        # Do something with data (in bytes) and return a string.
#        return "find a request!"
#server = MyServer(listening_address=('127.0.0.1', 11113))
#server.listen()

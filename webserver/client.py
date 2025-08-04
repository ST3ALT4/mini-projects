import socket

#create a socket connection to a server
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.coonnect(('localhost'), 8888)

# send and recive some data
sock.sendall(b'test')
data = sock.recv(1024)
print(data.decode())
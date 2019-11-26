import sys, socket

host = '127.0.0.1'
port = 50000

fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_addr = (host, port)
message = b'Hello! I am udp client'
fd.sendto(message, server_addr)
print('send %s to ' % message, server_addr)
data = fd.recv(1024)
print('receive %s' % data)
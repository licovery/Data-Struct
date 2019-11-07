import sys, socket

host = '127.0.0.1'
port = 50000


cons = [socket.socket(socket.AF_INET, socket.SOCK_STREAM) for i in range(20)]
for fd in cons:
    fd.connect((host, port))

for fd in cons:
    fd.recv(1024)
    fd.close()
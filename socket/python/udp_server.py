import sys, socket

# host = sys.argv[1]
# port = int(sys.argv[2])

host = '127.0.0.1'
port = 50000

fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
fd.bind((host, port))
print('bing udp on ', (host, port))
while True:
    data, addr = fd.recvfrom(1024)
    print('receive from', addr)
    print('receive %s' % data)
    fd.sendto(b'response from server: %s' % data, addr)
    print('send back to client')
    print('end datagram\n')
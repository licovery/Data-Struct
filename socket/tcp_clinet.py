import sys, socket

# host = sys.argv[1]
# port = int(sys.argv[2])

host = '127.0.0.1'
port = 50000

fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
fd.connect((host, port))

try:
    message = b'hello world! I am Neo'
    fd.send(message)
    total = len(message)
    cur = 0
    # raise BaseException('hh')
    while cur < total:
        data = fd.recv(1024)
        cur += len(data)
        print('receive from server: %s' % data)
        if data == None:
            print('no data from server')
            break
except BaseException as e:
    print(e)
finally:
    print('ready to close connection')
    fd.close()

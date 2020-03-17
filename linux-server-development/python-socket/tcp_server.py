import sys, socket, threading

def proc(con, addr):
    print('connect from ', addr)
    try:
        while True:
            data = con.recv(1024)
            if data:
                print('receive %s' % data)
                con.send(data)
                print('send back to client')
            else:
                break
    except BaseException as e:
        print(e)
    finally:
        print('end connection\n')
        con.close()



# host = sys.argv[1]
# port = int(sys.argv[2])

host = '127.0.0.1'
port = 50000

fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
fd.bind((host, port))
print('bing tcp on ', (host, port))
fd.listen(10)

while True:
    connection, client_addr = fd.accept()
    t = threading.Thread(target=proc, args=(connection, client_addr))
    t.start()

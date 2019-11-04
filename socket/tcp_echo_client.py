import sys, socket, socks

host = sys.argv[1]
port = int(sys.argv[2])


sock = socks.socksocket(socket.AF_INET, socket.SOCK_STREAM)

sock.set_proxy(socks.SOCKS5, "127.0.0.1", 1080)
sock.connect((host, port))


try:

    # Send data
    message = 'This is the message.  It will be repeated. hello world'
    print >>sys.stderr, 'sending "%s"' % message
    sock.sendall(message)

    # Look for the response
    amount_received = 0
    amount_expected = len(message)

    while amount_received < amount_expected:
        data = sock.recv(16)
        amount_received += len(data)
        print >>sys.stderr, 'received "%s"' % data

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
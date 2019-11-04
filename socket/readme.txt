一个简单的socket测试，客户端向服务器发送一个字符串，服务器收到后原样返回给客户端。然后通信结束

server: python tcp_echo_server.py <serverIp> <serverPort>

clinet: python tcp_echo_client.py <serverIp> <serverPort>
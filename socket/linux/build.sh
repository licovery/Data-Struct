if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build
gcc -g -std=c11 tcp_server.c comm.c -o build/time_server
gcc -g -std=c11 tcp_clinet.c comm.c -o build/time_client

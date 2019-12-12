set -e
cd $(dirname $0)

complieflag=" -g -Wall -I../comm -L../comm/build "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

gcc ${complieflag} server.c -lcomm -o build/echo_server 
gcc ${complieflag} client.c -lcomm -o build/echo_client
gcc ${complieflag} select_tcp_udp_server.c -lcomm -o build/echo_server_tcp_udp
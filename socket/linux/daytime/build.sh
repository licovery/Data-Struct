set -e
cd $(dirname $0)

complieflag=" -g -Wall -I../comm -L../comm/build "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

gcc ${complieflag} tcp_server.c -lcomm -o build/time_server 
gcc ${complieflag} tcp_client.c -lcomm -o build/time_client 
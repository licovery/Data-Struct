set -e
cd $(dirname $0)

complieflag=" -g -Wall -I../comm -L../comm/build "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

gcc ${complieflag} select_server.c -lcomm -o build/select_server 
gcc ${complieflag} select_client.c -lcomm -o build/select_client
gcc ${complieflag} poll_server.c -lcomm -o   build/poll_server 
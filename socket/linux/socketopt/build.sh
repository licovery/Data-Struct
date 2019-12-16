set -e
cd $(dirname $0)

complieflag=" -g -Wall -I../comm -L../comm/build "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

gcc ${complieflag} server.c -lcomm -o build/server 
gcc ${complieflag} client.c -lcomm -o build/client

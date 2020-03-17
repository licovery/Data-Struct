set -e
cd $(dirname $0)

complieflag=" -g -Wall -I../comm -L../comm/build "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

gcc ${complieflag} gethost.c -lcomm -o build/gethost
gcc ${complieflag} getaddr.c -lcomm -o build/getaddr

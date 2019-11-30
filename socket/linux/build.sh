if [ $# -ne 1 ]
then
    echo $0 "<dir>"
    exit
fi

if [ ! -d $1 ]
then
    echo "$1 not exist"
    exit
fi

cd $(dirname $0)
cd $1
if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build
gcc -g -std=c11 tcp_server.c comm.c -o build/time_server
gcc -g -std=c11 tcp_clinet.c comm.c -o build/time_client

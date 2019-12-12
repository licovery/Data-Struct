set -e
cd $(dirname $0)

complieflag=" -g -Wall -I../comm -L../comm/build "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

gcc $complieflag call_process.c -lcomm -o build/call_process 
gcc $complieflag multiprocess.c -lcomm -o build/a.out 
gcc $complieflag echo_server.c -lcomm -o build/echo_server 

#为了让exec的相对路径寻找成功，必须在build文件夹里面运行
#cd build
#./a.out
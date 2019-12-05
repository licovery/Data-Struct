set -e
cd $(dirname $0)

complieflag=" -g -std=c11 -Wall -I../comm -L../comm/build "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

gcc $complieflag call_process.c -lcomm -o build/call_process 2>> build/log.txt
gcc $complieflag multiprocess.c -lcomm -o build/a.out 2>> build/log.txt

#为了让exec的相对路径寻找成功，必须在build文件夹里面运行
#cd build
#./a.out
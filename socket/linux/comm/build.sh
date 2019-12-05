set -e
cd $(dirname $0)

complieflag=" -g -std=c11 -Wall "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

cd build
gcc ${complieflag} -c ../*.c 2>> log.txt
ar crv libcomm.a *.o >> log.txt
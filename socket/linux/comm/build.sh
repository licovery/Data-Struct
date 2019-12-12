set -e
cd $(dirname $0)

complieflag=" -g -Wall "

if [ -d "build" ]
then
    rm -rf build/
fi
mkdir build

cd build
gcc ${complieflag} -c ../*.c 
ar crv libcomm.a *.o 
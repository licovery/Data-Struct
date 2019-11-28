cd $(dirname $0)
cd project
rm -rf build/
mkdir build
cd build
platform=$(uname)
if [ "$platform" = "Linux" ]
then
    cmake ..
else
    cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
fi
make
./a



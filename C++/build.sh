cd $(dirname $0)
cd project
rm -rf build/
mkdir build
cd build
platform=$(uname)

if [ "$platform" = "Linux" ]
then
    cmake ..
    make
    ./a.out
else
    cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
    make
    ./a.exe
fi
#!/bin/sh

#错误马上退出
set -e

if [ $# -eq 0 ]
then
    echo $0 "<testname>"
    exit
fi

cd $(dirname $0)
cd $1
g++ -g -O0 $1.cpp -o $1
# --leak-check=full 显示详细内存泄漏相关信息
# -s 最后列出所有错误
valgrind --leak-check=full -s ./$1 2> $1.valgrind
rm $1
if [ $# -eq 0 ]
then
    echo $0 "<dir1> <dir2> ..."
    exit
fi

cd $(dirname $0)

red_begin="\033[31m"
red_end="\033[0m"

green_begin="\033[32m"
green_end="\033[0m"

for target in $*
do
    if [ ! -d $target ]
    then
        echo $red_begin"$target not exist"$red_end
        continue
    fi

    # 标准输出和标准错误一起重定向
    cd $target 
    make all > log.txt 2>&1
    cd ..

    if [ $? -ne 0 ]
    then
        echo $red_begin"$target build fail"$red_end
    else
        echo $green_begin"$target build succ"$green_end
    fi
done




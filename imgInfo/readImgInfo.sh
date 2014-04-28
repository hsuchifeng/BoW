#!/bin/sh
#recursivly collect image(*.jpg) information from specific directory
#the image information is output to stdout using CSV format

getInfo() {
    #get the picture information inside the dir

    if [ $# -ne 2 ]; then #if与 [之间要有空格, ]与;之间不能有空格, [与]之间的条件要有空格
	echo "Ussage : $0 <filename> <lable>"
	exit 1  #不用加括号
    fi

    if [ ! -f $1 ]; then
	echo "$1 was not a directory" #因为要引用$1所以加上""
	exit 1
    fi


    #calc sha256
    shas=`sha256 -q $1` #=两边的表达式不能有空格
    
    #basename
    bname=`basename $1`

    #dir
    #dirn=`dirname $1`

    sz=`identify -format "%b" $1 | grep -oE '[0-9]+'`
    w=`identify -format "%w" $1`
    h=`identify -format "%h" $1`

    #
    echo "$2,$1,$shas,$bname,$sz,\"{$w,$h}\""
}

if [ $# -ne 2 ]; then #if与 [之间要有空格, ]与;之间不能有空格, [与]之间的条件要有空格
    echo "Ussage : $0 <directory> <image class lable>"
    exit 1  #不用加括号
fi
#header line of CSV file
echo 'imageClass,imagePath,imageSHA,imageName,imageSize,imageResolution'
#also find soft link
find -L $1 -type f -name "*.jpg" |
while read imgf ; do getInfo "$imgf" $2; done

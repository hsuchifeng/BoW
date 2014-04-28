#!/bin/sh
#copy the image information to the database
#ussage: ./inputImgInfo.sh <file.csv>
if [ $# -ne 1 ]; then
    echo "ussage: $0 <file.csv> "
    exit 1;
fi
set -x
csv=`pwd`"/$1"
sql="COPY imageInfo(imageClass,imagePath,imageSHA,imageName,imageSize,imageResolution)\
     FROM '$csv' CSV HEADER;"
psql -d imret -U pgsql -c "$sql" #import to database 


#!/bin/sh
#copy the SIFT information to the database
if [ $# -ne 1 ]; then
    echo "ussage: $0 <CSV_file_with_header_line> "
    exit 1;
fi
set -x
csv=`pwd`"/$1"
sql="COPY SIFTFeature(imageID,pos,feature)\
     FROM '$csv' CSV HEADER;"
psql -d imret -U pgsql -c "$sql" #import to database

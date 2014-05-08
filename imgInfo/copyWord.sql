--复制数据库的信息到标准输出
COPY  ( select imageid,visualword from imagevisualword where k=1024 AND condition ~ '10Dir' order by imageid) to STDOUT WITH CSV HEADER ;

--复制数据库的信息到标准输出
COPY  ( select imageid,visualword from imagevisualword where condition ~ '256Obj' order by imageid) to STDOUT WITH CSV HEADER ;

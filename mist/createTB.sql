-- 表实现

--图像信息表
create table if not exists imageInfo(
imageID serial PRIMARY KEY, --图像ID，主键
imagePath text UNIQUE NOT NULL , --图像路径
imageClass text NOT NULL, --图像分类
imageSHA  text NOT NULL ,--SHA-256
imageName text , --图像原始文件名
imageSize real NOT NULL CHECK (imageSize>0), --图像大小,单位byte
imageResolution integer[2] NOT NULL --图像分辨率,w*h
);

--SIFT特征表
create table if not exists SIFTFeature(
featureID serial PRIMARY KEY, --特征ID,主键
imageID integer NOT NULL REFERENCES imageInfo(imageID), --引用图像ID
--calcMethod  character[10] NOT NULL DEFAULT 'OPENCV2',
--提取SIFT特征的函数,目前是opencv里的
feature double precision[] NOT NULL , --特征,128维数组
pos double precision [] NOT  NULL --SIFT特征的坐标
);

--SIFT特征聚类簇表
create table if not exists SIFTClusterCenter( 
clusterID serial PRIMARY KEY, --簇ID,
d integer NOT NULL CHECK (d>0), --簇的维度(特征维度)
k integer NOT NULL CHECK (k>0), --聚类的k（簇的个数)
condition text NOT NULL , --聚类的图片集,SQL语句中的where 子句
center double precision[] NOT NULL --簇中心点
--featureID integer [] NOT  NULL --属于这个簇的featureID 
);


--视觉词汇表
create table if not exists imageVisualWord(
wordID serial PRIMARY KEY, --wordID
imageID integer NOT NULL REFERENCES imageInfo(imageID), --引用图像ID
k integer NOT NULL check (k>0), --词汇维度
condition text NOT NULL , --同SIFTClusterCenter
clusterID integer[] NOT  NULL , --该词汇引用的簇 ID,用于建立反向索引
visualWord double precision[] NOT NULL --视觉词汇,k个元素
);

--HS颜色直方图
create table if not exists hsHist(
hsID serial PRIMARY KEY, --hsID
imageID integer NOT NULL REFERENCES imageInfo(imageID),
condition text NOT NULL , --where子句选择的图片集
hbin integer NOT NULL, --h块的数目
sbin integer NOT NULL, --s块的数目
hs  double precision [] NOT NULL --'二'维数组,h*s
);

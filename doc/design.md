# 基于内容的图像检索系统 #
## 基于视觉词汇模型 ##

+ 项目名称
  - 基于内容的图像检索系统


+ 开发者
  - 徐志锋
  - 谢旭双
  - 黄家明


+ 指导老师
  - 温雯

---
**修订记录**  
<table border="1">  
<tr> <td>修订人</td> <td> 修订日期</td> <td>修订内容</td> </tr>
<tr> <td>徐志锋</td> <td> 2014/3/15 </td>  <td>初始版本</td>
</table>
---


### 需求分析 ###
#### 功能需求 ####
* 系统能根据用户提供的图像，从图像库中检索出与用户图像相似的图像
* 用户可根据颜色信息对相似图像进行筛选，可筛选的内容有、用户希望出现的颜色集、用户不希望出现的颜色集


### 概要设计 ###
#### 概要 ####
+ 因为开源数据库中只有PostgreSQL支持数组类型及自定义类型，所以数据库选用PostgreSQL
+ 为方便在不同的平台间移植及减少开始时间，检索平台使用B/S框架
+ 使用google的protobuf来传输检索请求、检索结果、数据库数据
+ 数据库存放图像的SIFT特征、SIFT聚类后的簇中心、视觉词汇频率直方图、一些图像信息  
  图像的HS颜色直方图  
  为了便于Web服务器的引用，图像内容(文件)存放与本地文件系统中，而不存放在数据库里
+ 检索系统的搭建可大致分为两个阶段
  - 搭建图像库检索库
  - 搭建检索平台，包括检索页面以及后台服务器的搭建
  
#### 图像检索库的搭建 ####
+ 生成视觉词汇库流程
  - 提取图像的SIFT特征
  - 对所有图像的SIFT特征进行聚类，使用kmeans算法
  - 根据每张图像所有SIFT特纳以及聚类后的簇中心计算出每张图像的视觉词汇频率直方图
  - 将以上信息存入数据库
+ 生成HS颜色直方图库
  - 提取图像的HSV直方图
  - 将以上信息存入数据库
  
#### 检索平台 ####

##### 检索平台由以下服务器组成部分 #####
  + Web服务器
	用于接受用户图像，展示检索结果
  + 图像检索服务器
	用于处理来自Web服务器的检索请求，由三个模块组成:
	- TCP/IP模块, 用于处理Web发来的请求
	- 视觉词汇检索模块
	- HS颜色颜色直方图检索模块
	- 颜色过滤模块
  + PostgreSQL服务器
	用于存储检索信息


##### 视觉词汇检索模块流程图 #####
  - 读取用户图像
  - 计算用户图像SIFT特征
  - 生成用户图像的视觉词汇频率直方图
  - 计算用户图像视觉词汇频率直方图与图像库每张图像视觉词汇频率直方图的余弦相似性，
	并按相似度递减次序排序



### 详细设计 ###

#### 数据库表 ####
	-- 表实现

	--图像信息表

	create table if not exists imageInfo(

	imageID serial PRIMARY KEY, --图像ID，主键

	imagePath text NOT NULL , --图像路径
	
	imageSHA  text NOT NULL ,--SHA-256
	
	imageName text , --图像原始文件名
	
	imageSize real NOT NULL CHECK (imageSize>0), --图像大小,单位byte
	
	imageResolution integer[2] NOT NULL --图像分辨率,w*h
	
	);
	
	
	--SIFT特征表
	
	create table if not exists SIFTFeature(
	
	featureID serial PRIMARY KEY, --特征ID,主键
	
	imageID integer NOT NULL REFERENCES imageInfo(imageID), --引用图像ID
	
	--calcMethod  character[10] NOT NULL DEFAULT 'OPENCV2', --提取SIFT特征的函数,目前是opencv里的
	
	feature double precision[] NOT NULL , --特征,128维数组
	
	pos double precision [] NOT NULL --SIFT特征的坐标
	
	);
	
	--SIFT特征聚类簇表
	
	create table if not exists SIFTClusterCenter( --一次聚类产生只产生一个记录
	clusterID serial PRIMARY KEY, --簇ID,
	
	d integer NOT NULL CHECK (d>0), --簇的维度
	
	k integer NOT NULL CHECK (k>0), --簇的个数
	
	center double precision[][] NOT NULL ,--中心点,2维数组(k*d)
	
	featureID integer [][] NOT NULL --属于这个簇的featureID ,--2维数组(k*d)
	);
	
	
	--视觉词汇表
	
	create table if not exists imageVisualWord(
	
	imageID int PRIMARY KEY REFERENCES imageInfo(imageID), --主键，引用图像ID
	
	clusterID integer NOT NULL REFERENCES SIFTClusterCenter(clusterID), --该词汇引用的簇
	
	k integer NOT NULL check (k>0), --词汇维度
	
	visualWord double precision[] NOT NULL --视觉词汇,k个元素
	
	) ;
	
	--HS颜色直方图
	create table if not exists hsHist(
	hsID serial primary key, --ID
	imageID integer REFERENCES imageInfo(imageID), 
	hbin integer NOT NULL, --h块的数目
	sbin integer NOT NULL, --s块的数目
	h  double precision [] NOT NULL,
	s  double precision [] NOT NULL
	)

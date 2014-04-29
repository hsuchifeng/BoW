/****************************************
最近修改20140427

+ 类exData包含公有的成员
1. vector<ImageInfo> image  ,储存图像信息,image[0,n-1],与数据库中的imageID[1,n]一一对应
2. Matrix clusterCenter     ,聚类后的簇中心点
3. hbin, sbin               ,HS直方图的划分块数

+ ImageInfo类exData的私有类型，提供每张图像对应的信息,
1. string imagePath         ,图像路径
2. string imageName         ,原始图像文件名
3. vector<float>  word     ,图像的视觉词汇频率直方图
4. Matrix  SIFTFeat         ,图像SIFT特征
5. vector<float>  hs       ,HS直方图中

+ Matrix 为 vector<vector<float> > 的typedef

+ 初始化
exData对象创建时,默认构造函数会从数据库中读取图像路径信息到image中

+ 公有成员函数
1. readSIFT()
 读取数据库中的SIFT特征

2. readCenter(int centerCount = 128)
 读取簇的中心点
 @centerCount 为簇的个数

3. readWord(int wdDem = 128)
 读取视觉词汇频率直方图
 默认的@k 为128维

4. readHS(int hb, int sb)
 读取HS(V)颜色直方图
 @hb,@sb 为直方图的对应的bins

5. writeHS()
 重写HS直方图表
 数据由image[i].hs, hbin,sbin提供
 数组image[0,n-1]与数据库中的imageID[1,n]一一对应

6. writeSIFT()
 重写SIFT表
 数据由exData::image提供
 数组image[0,n-1]与数据库中的imageID[1,n]一一对应

7. writeWord()
 重写视觉词汇表
 数据由数组image[i].word提供
 数组image[0,n-1]与数据库中的imageID[1,n]一一对应

8. writeCenter()
 重写聚类簇中心表
 数据由clusterCenter提供
****************************************/
#ifndef EXDATA_HPP
#define EXDATA_HPP

#include <string>
#include <vector>
#include <map> //<key,value>

extern "C"
{
#include <libpq-fe.h>
} //libpq

//连接数据库参数
#define CONNECT_INFO "postgresql://pgsql:pgsql@10.21.23.104/imret"
//初始化图像信息语句
#define SQL_IMAGE_INFO "select imagePath,imageName,imageID from imageInfo "



class  exData { //数据库信息类
private:

  enum CMDT {connection,tuple,task};   //数据库SQL语句类型
  typedef   std::vector<std::vector<float> >  Matrix;  //二维数组
  std::string condition; //图片集
  struct ImageInfo  //单张图像的信息
  {
    std::string imagePath; //图像路径
    std::string imageName; //原始图像文件名
    std::vector<float>  word; //图像的视觉词汇频率直方图
    std::vector<float>  hs; //HS直方图中,hbin=hs.size(), sbin=hs[0].size()
    Matrix  SIFTFeat; //图像SIFT特征
    std::vector<float> SIFTx; //SIFT特征对应的坐标
    std::vector<float> SIFTy;

    void clear()//清空数据
    {
    imagePath.clear();
    imageName.clear();
    SIFTFeat.clear();
    SIFTx.clear();
    SIFTy.clear();
    word.clear();
    hs.clear();
  }//clear
  };//ImageInfo
public:
  Matrix  clusterCenter; //每个簇对应的簇中心
  //图像数据image[0,n-1]与数据库中的imageID[1,n]一一对应
  std::vector<ImageInfo> image;
  int hbin, sbin; //HS直方图块数

  std::map<long,long> idToSub; //映射 数据库中的imageID 到 exData中数据的数组下标
  std::map<long,long> subToID; //与idToSub相反
  
  //默认构造函数，从数据库中加载所有图像的路径及文件名(不含扩展名)
  //s 选择图像数据的where语句
  exData(const char *s =NULL):condition(s),hbin(0),sbin(0)
                             ,pgConn(NULL), pgRes(NULL)
  {
    getInfo(s); //获取图像路径信息
    //temp !!!
    //image.resize(350);
  }
  //析构函数
  ~exData()
  {
    if(pgRes)//free result
      PQclear(pgRes);
    if(pgConn)//free connection
      PQfinish(pgConn);
  }

  //check wheter given is valid
  bool validID(long id){
    return idToSub.end() != idToSub.find(id);
  }
    
  //从CSV文件中读取SIFT特征
  //返回读取的个数
  // long readSIFTFromFile(const char *s);
  long readSIFTFromFile(const char *csv);
  
  //读取数据库中SIFT特征
  //返回读取的个数
  long readSIFTFromDB();
  long readSIFT(const char* s=NULL){
    if(!s) return readSIFTFromDB();
    else return readSIFTFromFile(s);
  }

  //读取簇的中心点
  //@centerCount 为簇的个数
  void readCenter(int centerCount = 128);
  
  //读取视觉词汇频率直方图
  //默认的@k 为128维
  void readWord(int wdDem = 128);
  
  //读取HS(V)颜色直方图
  //@hbin,@sbin 为直方图的对应的bins
  void readHS(int hbin, int sbin);
  
  //重写HS直方图表
  //数据由image[i].hs提供, 函数把hs.size()作为hbin, hs[0].size()作为sbin
  //数组image[0,n-1]与数据库中的imageID[1,n]一一对应
  void writeHS();

  //重写SIFT表
  //数据由exData::image提供
  //数组image[0,n-1]与数据库中的imageID[1,n]一一对应
  void writeSIFT();
    
  //重写视觉词汇表
  //数据由数组image[i].word提供
  //数组image[0,n-1]与数据库中的imageID[1,n]一一对应
  void writeWord();

  //重写聚类簇中心表
  //数据由clusterCenter提供
  void writeCenter();
  
private:
  PGconn *pgConn;   //数据库连接
  PGresult *pgRes;  //数据库查询结果
   //建立与数据库的连接 
  void connectDB(const char *connInfo)
  {
    if(!pgConn) //未建立连接
      pgConn = PQconnectdb(connInfo);
    //检测执行状态
    statusOK("error:connection to postgresql",connection);
  }

  //用空格替换不能被stringstream 解析的特殊字符
  void clearS(std::string &s)
  {
    int j;
    for(j=0;j < s.size(); ++j) 
    {
      if(s[j] == ',' || s[j] == '{' || s[j] == '}')
        s[j] =' ';
    }
    
  }//clearS

  //test whether the command is execd ok
  //@cmdt is the type of command
  void statusOK(const char *mesg,CMDT ct );

  //get image information
  void getInfo(const char *c=NULL);
  
};//exData

#endif

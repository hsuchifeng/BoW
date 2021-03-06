#include "exData.hpp"

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>  //atoi
#include <map>
extern "C"
{
#include <libpq-fe.h>
} //libpq

//test whether the command is execd ok
//@cmdt is the type of command
void exData::statusOK(const char *mesg,CMDT ct )
{
#define    THROW throw std::invalid_argument(std::string(mesg) + std::string("\n") +PQerrorMessage(pgConn) )
  switch(ct)
  {
  case connection:
    //检测状态
    if(CONNECTION_OK !=  PQstatus(pgConn))
      THROW;
    break;
  case tuple :
    if(PGRES_TUPLES_OK != PQresultStatus(pgRes))
      THROW;
    break;

  case task:
    if(PGRES_COMMAND_OK != PQresultStatus(pgRes))
      THROW;
    break;
  default : break;
  }//switch
#undef THROW
}

//get image information
void exData::getInfo(const char * condiction)
{
  int nrow; //结果的行数和列数
  int i,id; //循环变量
  int nameIndex, pathIndex, IDIndex; //检索结果对应的列号
  std::stringstream sst; //用于提取字符串
  ImageInfo imageTmp;
  std::string sql=SQL_IMAGE_INFO;
  sql += condiction;
  
  connectDB(pgURI.c_str()); //连接数据库
  pgRes = PQexec(pgConn,sql.c_str());
  //is command exec ok
  statusOK("error:exec select from imageInfo", tuple);
  //检索结果
  nameIndex = PQfnumber(pgRes,"imageName");
  pathIndex = PQfnumber(pgRes,"imagePath");
  IDIndex = PQfnumber(pgRes,"imageID");
  nrow = PQntuples(pgRes);
  //resize the data
  image.resize(nrow);
  for(i = 0; i < nrow; ++i)
  {
    imageTmp.clear();
    //name
    imageTmp.imageName = PQgetvalue(pgRes,i,nameIndex);
    //path
    imageTmp.imagePath =PQgetvalue(pgRes,i ,pathIndex);
    //ID
    id = atoi(PQgetvalue(pgRes,i,IDIndex));
    //i为索引值,存储映射
    idToSub[id]=i;
    subToID[i] =id;
    image[i] = imageTmp;
  }//for
}//getInfo()

//读取数据库中的所有SIFT特征
//返回读取的个数
int exData::readSIFTFromDB()
{
  int res = 0; //返回值
  int nrow; //结果的行数和列数
  int i; //循环变量
  int n,nimage = image.size(); //图像数目
  int featIndex; //检索结果对应的列号
  std::stringstream sst; //用于提取字符串
  std::string sqlStmt, s;
  std::vector<float> tmp; //储存特征
  float d;

  connectDB(pgURI.c_str()); //连接数据库
  for(n = 0; n < nimage; ++n){ //读取每张图像
    std::cerr<<"reading image " << n+1 << "," <<image[n].imagePath <<"\n";
    PQclear(pgRes);
    sst.str(""); //清空内容
    sst.clear(); //清空标志位
    sst << "select feature from SIFTFeature where imageID="
        << subToID[n] <<";";
    pgRes = PQexec(pgConn, sst.str().c_str());
    //is command exec ok
    statusOK("error:exec select from SIFTfeature",tuple);
    //检索结果
    featIndex = PQfnumber(pgRes,"feature");
    nrow = PQntuples(pgRes);
    //清空原有数据
    image[n].SIFTFeat.clear();
    for(i = 0; i < nrow; ++i) {//每行一个特征
      s = PQgetvalue(pgRes,i,featIndex);
      clearS(s); //清空一些特殊符号
      tmp.clear();
      sst.str("");
      sst.clear();
      sst << s;
      while(sst >> d)
        tmp.push_back(d);
      //保存到image中
      image[n].SIFTFeat.push_back(tmp);
      //结果数+1
      ++res;
    }//for
    std::cerr << image[n].SIFTFeat.size()
              <<" features in the image\n";
  }//for
  return res;
}//readSIFT

//重写SIFT表
//数据由exData::image提供
//数组exData::image 的索引与数据库中的imageID一一对应(-1)
void exData::writeSIFT()
{
  if(image.empty())//无数据
    return ;
  
  int i,j,k; //循环变量
  int n = image.size(); //图像数目
  std::string sql; //待执行的SQL语句
  std::stringstream sst;
  std::string s; 

  connectDB(pgURI.c_str());
  //keep atom
  pgRes = PQexec(pgConn,"BEGIN");
  statusOK("error:exec BEGIN",task);
  for(i = 0; i < n; ++i) //每张图像
  {
    if(image[i].SIFTFeat.empty()) {//null image
      std::cerr<<"empty " <<i <<std::endl;
      continue;
    }
    
    //先删除原来数据
    sst.str("");
    sst.clear();
    sst <<"delete from SIFTFeature where imageID="<<subToID[i] <<";";
    pgRes = PQexec(pgConn,sst.str().c_str());
    statusOK("error:exec delete from SIFTFeature", task);
    for(j = 0; j < image[i].SIFTFeat.size(); ++j) {//每行
      sst.str("");
      sst.clear();
      sst << "insert into SIFTFeature(imageID,pos,feature) values "
          <<"(" << subToID[i] << "," ;
      sst<<"\'{";
      sst<<image[i].SIFTx[j] <<","<<image[i].SIFTy[j];
      sst<<"}\'" <<",";
      //start with '{
      sst << "\'{";
      for(k = 0; k < image[i].SIFTFeat[j].size() -1; ++ k)//每个元素
        sst << image[i].SIFTFeat[j][k] << ",";
      //last
      sst << image[i].SIFTFeat[j][k];
      //end with }')
      sst << "}\')";
      //exec command
      pgRes = PQexec(pgConn,sst.str().c_str());
      statusOK("error:insert into SIFTFeature", task);
    }
  }
  pgRes = PQexec(pgConn, "END");
  statusOK("error:END", task);

}//writeSIFT

//读取HS颜色直方图
//@hb,@sb 为直方图的对应的bins
void exData::readHS(int hb, int sb)
{
  int nrow; //结果的行数
  int i,j; //循环变量
  int id; //imageID
  int idindex,hsindex; //检索结果对应的列号
  std::stringstream sst; //用于提取字符串
  std::string  s;
  std::vector<float> tmp; //储存视觉词汇频率
  float d;
  char *cons = PQescapeLiteral(pgConn,condition.c_str(),condition.length());
  
  connectDB(pgURI.c_str()); //连接数据库
  //查询颜色直方图
  sst.str("");
  sst.clear();
  sst << "select imageID,hs from hsHist "
      << " where hbin=" <<hb << "AND sbin=" <<sb
      <<" AND condition=" << cons <<";";
  pgRes = PQexec(pgConn,sst.str().c_str());
  //is command exec ok
  statusOK("error:exec select from hsHist", tuple);
        
  //检索结果
  idindex = PQfnumber(pgRes,"imageID");
  hsindex= PQfnumber(pgRes,"hs");
  nrow = PQntuples(pgRes);  //行数
  if(nrow != image.size()) {//检查图像数与结果数是否相等
    std::cerr << "condition="<<condition << "h="<<hb<< "s=" <<sb<<"\n";
    std::cerr << nrow << " row result,image size=" << image.size() <<"\n";
    throw std::invalid_argument("error:image number doesn't match hs number");
  }
  for(i = 0; i < nrow; ++i)
  {
    //转换id为整数
    id = atoi(PQgetvalue(pgRes,i,idindex));
    s = PQgetvalue(pgRes,i,hsindex);
    clearS(s); //清除字符 , {  }
    sst.str("");
    sst.clear();
    sst << s;
    //清空原有数据
    image[idToSub[id]].hs.clear();
    j = 0; //计数
    while(sst >> d){//存入新值
      image[idToSub[id]].hs.push_back(d);
      ++j;
    }
    //检验数据完整
    if(j != hb*sb)
      throw std::invalid_argument("error: data from database doesn't match the argument");
  }
  hbin = hb;
  sbin = sb;
}//readHS

//重写HS表
//数据由exData::image提供
//数组exData::image 的索引与数据库中的imageID一一对应(-1)
void exData::writeHS()
{
  if(image.empty())//无数据
    return ;
  
  int i,j; //循环变量
  int n = image.size(); //图像数目
  std::string sql; //待执行的SQL语句
  std::stringstream sst;
  std::string s; 
  char *cons = PQescapeLiteral(pgConn,condition.c_str(),condition.length());
  
  connectDB(pgURI.c_str());
  //keep atom
  pgRes = PQexec(pgConn,"BEGIN");
  statusOK("error:exec BEGIN",task);
  //先删除
  sst.str("");
  sst.clear();
  sst << "delete from hsHist where "
      <<"hbin=" << hbin << " AND sbin=" << sbin
      << " AND condition=" << cons <<";";
  pgRes = PQexec(pgConn,sst.str().c_str());
  statusOK("error:exec delete from hsHist", task);
  for(i = 0; i < n; ++i) //遍历插入
  {
    sst.str("");
    sst.clear();
    sst << "insert into hsHist(imageID,hbin,sbin,condition,hs) values "
        << "(" << subToID[i] << "," << hbin << "," << sbin
        << "," << cons <<  ",";
    //start with '{
    sst <<"\'{";
    //hue and saturation
    for(j = 0; j < image[i].hs.size() - 1; ++j) //h*s
      sst << image[i].hs[j] << ",";
    //last element
    sst << image[i].hs[j];
    //end with }')
    sst << "}\')";
    //exec command
    pgRes = PQexec(pgConn,sst.str().c_str());
    statusOK("error:insert into hsHist", task);
  }
  pgRes = PQexec(pgConn, "END");
  statusOK("error:END", task);

}//writeHS


//读取簇的中心点,以及反向索引信息
//@k 为簇的个数
void exData::readCenter( int k)
{
  int nrow; //结果的行数和列数
  int i,j; //循环变量
  int d; //中心点的维度
  int dindex,cindex,idindex,tfindex; //检索结果对应的列号
  std::stringstream sst,idsst,tfsst; //用于提取字符串
  std::string  s,ids,tfs; 
  std::vector<float> tmp; //储存特征
  float t; //临时变量
  int id; //图像id
  std::map<int,float> rev; //单个反向索引信息
  char *cons = PQescapeLiteral(pgConn,condition.c_str(),condition.length());
  
  connectDB(pgURI.c_str()); //连接数据库
  //    std::cerr <<"connect ok\n";
  sst.str("");
  sst.clear();
  sst << "select d,center,reverseImageID,reverseImageTF from "
      <<  "SIFTClusterCenter where k=" << k
      << " AND condition=" << cons<<" ;";
  pgRes = PQexec(pgConn,sst.str().c_str());
  //is command exec ok
  statusOK("error:exec select from SIFTClusterCenter",tuple);
  //    std::cerr<<"exec select ok\n";
  //检索结果
  dindex = PQfnumber(pgRes,"d");
  cindex = PQfnumber(pgRes,"center");
  tfindex =PQfnumber(pgRes,"reverseImageTF");
  idindex =PQfnumber(pgRes,"reverseImageID");
  nrow = PQntuples(pgRes); //
  if(nrow == 0)
    throw std::invalid_argument("no cluster center read in database");
  //中心维度第一个元素为准
  d = atoi(PQgetvalue(pgRes,0,dindex));
  //清空原有数据
  clusterCenter.clear();
  reverseIndex.clear(); 
  for(i = 0; i < nrow;++i) {//检索每个结果
    //center
    s = PQgetvalue(pgRes,i,cindex);
    clearS(s); //清空一些特殊符号
    sst.str("");
    sst.clear();
    sst << s;
    tmp.clear();
    j = 0; //计数
    while(sst >> t) //每个元素
      ++j,tmp.push_back(t);
    if(j != d) //读取个数不一样
      throw std::invalid_argument("cluster center demension");
    clusterCenter.push_back(tmp);
    //reverse info
    tfs = PQgetvalue(pgRes,i,tfindex);
    ids = PQgetvalue(pgRes,i,idindex);
    clearS(tfs);
    clearS(ids);
    tfsst.clear();
    tfsst.str("");
    tfsst << tfs;
    idsst.clear();
    idsst.str("");
    idsst << ids;
    rev.clear();
    while(idsst >> id) 
      tfsst >> rev[id];
    std::cerr<<rev.size() << " image(s) in cluster " << i+1 << "\n";
    reverseIndex.push_back(rev);
  }//for
}

//重写聚类簇中心表
//数据由exData::clusterCenter 提供
void exData::writeCenter()
{
  if(clusterCenter.empty())//无数据
    return ;
  
  int i,j; //循环变量
  int k = clusterCenter.size(); //簇的数目
  int d = clusterCenter[0].size(); //中心点的维度
  std::string sql; //待执行的SQL语句
  std::stringstream sst;
  std::string s;
  char *cons = PQescapeLiteral(pgConn,condition.c_str(),condition.length());
  connectDB(pgURI.c_str());
  //keep atom
  pgRes = PQexec(pgConn,"BEGIN");
  statusOK("error:exec BEGIN",task);
  //先删除
  sst.str("");
  sst.clear();
  sst << "delete from SIFTClusterCenter where k=" << k
      <<" AND d="<<d <<" AND condition="   <<cons <<";" ;
  pgRes = PQexec(pgConn, sst.str().c_str());  
  statusOK("error:exec delete from SIFTClusterCenter", task);    
  for(i = 0; i < k; ++i){ //每个特征
    std::cerr<<"center " << i+1 << "\n";
    //insert
    sst.str("");
    sst.clear();
    sst << "insert into SIFTClusterCenter(k,d,condition,reverseImageID,reverseImageTF,center) values "
        <<"(" << k << "," << d << "," << cons <<",";
    //反向索引信息
    sst << "\'{";
    std::map<int,float>::iterator mit = reverseIndex[i].begin();
    while( mit != reverseIndex[i].end()){ //imageID
      sst << mit->first;
      // std::cerr<<"\t" << mit->first ; 
      ++mit;
      if(mit != reverseIndex[i].end()) sst << ",";
    }
    sst << "}\',\'{";
    mit = reverseIndex[i].begin();
    while(mit != reverseIndex[i].end()){ //imageTF
      sst << mit->second;
      // std::cerr<<"\t" << mit->second ;
      ++mit;
      if(mit  != reverseIndex[i].end()) sst << ",";
    }//for
    sst << "}\',"; //end this
    
    //center
    sst << "\'{"; //start with '{
    for(j = 0; j < d - 1; ++j) //leave last element
      sst << clusterCenter[i][j] << "," ; //seperated by comma
    //end with }'
    sst << clusterCenter[i][j] << "}\');" ;
    //exec command
    pgRes = PQexec(pgConn,sst.str().c_str());
    statusOK("error:insert into SIFTClusterCenter", task);
  }

  pgRes = PQexec(pgConn, "END");
  statusOK("error:END", task);
}//writeCenter

//读取视觉词汇频率直方图
//默认的@k 为128维
void exData::readWord(int wdDem)
{
  int nrow; //结果的行数
  int i; //循环变量
  int id; //imageID
  int idindex,vindex; //检索结果对应的列号
  std::stringstream sst; //用于提取字符串
  std::string  s;
  std::vector<float> tmp; //储存视觉词汇频率
  float d;
  char *cons = PQescapeLiteral(pgConn,condition.c_str(),condition.length());
  
  connectDB(pgURI.c_str()); //连接数据库
  //查询词汇,默认k=128
  sst.str("");
  sst.clear();
  sst << "select imageID,visualWord from " << "imageVisualWord "
      <<"where k=" << wdDem << " AND condition="<< cons<<";";
  pgRes = PQexec(pgConn,sst.str().c_str());
   
  //is command exec ok
  statusOK("error:exec select from imageVisualWord",tuple);
  //检索结果
  idindex = PQfnumber(pgRes,"imageID");
  vindex = PQfnumber(pgRes,"visualWord");
  nrow = PQntuples(pgRes);  //行数
  if(nrow != image.size()) //数据表为空或数据表有错误
    throw std::length_error("error:table imageVisualWord empty\n");
  for(i = 0; i < nrow; ++i)
  {
    //转换id为整数
    id = atoi(PQgetvalue(pgRes,i,idindex));
    s = PQgetvalue(pgRes,i,vindex);
    clearS(s); //清空一些特殊符号
    sst.str("");
    sst.clear();
    sst << s;
    tmp.clear();
    while(sst >> d)
      tmp.push_back(d);
    //存入新值
    image[idToSub[id]].word = tmp;
  }
} //readWord

//重写视觉词汇表
//数据在数组exData::image中
void exData::writeWord()
{
  if(image.empty())//无数据
    return ;
  
  int i,j; //循环变量
  int n = image.size(); //图像数目
  int k; //词汇的维度
  std::string sql; //待执行的SQL语句
  std::stringstream sst;
  std::string s;
  char *cons = PQescapeLiteral(pgConn,condition.c_str(),condition.length());
  connectDB(pgURI.c_str());
  k = image[0].word.size(); //以第一个大小初始化
  //keep atom
  pgRes = PQexec(pgConn,"BEGIN");
  statusOK("error:exec BEGIN",task);
  //先删除已经存在的
  sst.str("");
  sst.clear();
  sst << "delete from imageVisualWord where k=" << k
      <<" AND condition=" << cons <<";";
  pgRes = PQexec(pgConn,sst.str().c_str());
  statusOK("error:exec delete from imageVisualWord", task);
  for(i = 0; i < n; ++i) //遍历插入
  {
    if( k != image[i].word.size()) //数据有错误
      throw std::invalid_argument("error: data size");
    sst.str("");
    sst.clear();
    sst << "insert into imageVisualWord(imageID,k,condition,visualWord) "
        <<"values (" << subToID[i] << "," << k << "," << cons <<","
        <<"\'{";
    //word
    for(j = 0; j < image[i].word.size() - 1; ++j) //important -1
      sst << image[i].word[j] << ",";
    sst<< image[i].word[j] << "}\'";
    sst << ");"; //do not forget this
    //exec command
    pgRes = PQexec(pgConn,sst.str().c_str());
    statusOK("error:insert into imageVisualWord", task);
  }
  pgRes = PQexec(pgConn, "END");
  statusOK("error:END", task);
}//writeWord

//move to next digit
bool nextDigit(std::stringstream &ist){
  int c;
  while(ist.good()){
    c = ist.get();
    if(isdigit(c)){
      ist.putback(c);
      break;
    }
  }
  return ist.good();
}//nextDigit

//read SIFT features from file
//the file is CSV format with header "imageID,pos,feature"
//SIFT features are read to exData::image[i].SIFTFeat
//return the number of features read
int exData::readSIFTFromFile(const char *csv){
  int r=0; //return value
  int i; //index
  int n = image.size(); //image count
  int imageID; //imageID in database
  float f;
  std::ifstream ifs(csv);
  std::string s;
  std::stringstream sst;
  std::vector<float> feat;
  
  if(!ifs) //check parameter
    return 0;
  for(i = 0; i < n; ++i){//clear origin data
    image[i].SIFTFeat.clear();
    image[i].SIFTx.clear();
    image[i].SIFTy.clear();
  }
  getline(ifs,s); //ignore first line(header)
  while(getline(ifs,s)){//deal every line in the CSV file
    sst.clear(); //clear status flag
    sst.str(""); //empty content
    sst << s;
    sst >> imageID; //extract current imageID
    if(!validID(imageID)) //check the imageID
      continue;
    nextDigit(sst);
    sst >> f; //SIFTx
    image[idToSub[imageID]].SIFTx.push_back(f);
    nextDigit(sst);
    sst >> f; //SIFTy
    image[idToSub[imageID]].SIFTy.push_back(f);
    //extract features
    feat.clear();
    while(nextDigit(sst)){
      sst >> f;
      feat.push_back(f);
    }
    image[idToSub[imageID]].SIFTFeat.push_back(feat);
    ++r; //count + 1
  }//while
  return r;
}//readSIFTFromFile

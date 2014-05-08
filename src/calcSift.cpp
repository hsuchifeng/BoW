/****************************************
读取SIFT特征，输出到标准输出,CSV格式(no header)
从配置文件中读取图片集信息
Zhifeng 201404 
****************************************/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "iutility.hpp"
#include "exData.hpp"

using namespace std;
int
main(int argc, char **argv)
{
  if(argc != 2) //check parameter
  {
    cerr<<"calculate SIFT feature and output CSV format to stdout\n"
        <<"ussage:" <<argv[0] << " <configuration_file>\n";
    return 1;
  }
  
  const char *confFile=argv[1];
  map<string,string> conf; //配置信息
  readConf(conf, confFile); //read configure file
  if(hasValue(conf,"PGURI")) //check connection URI
    cerr<<"connection URI:" << conf["PGURI"] <<"\n";
  else{
    cerr<<"error:no connection URI specific\n";
    return 1;
  }
  
  try  {
    exData  db(conf["image"].c_str(), conf["PGURI"].c_str());  //图像信息数据
    int nimg;  //图像数目
    int i,j,k; //循环变量
    vector<int> failID; //计算SIFT失败的图像ID
    vector<vector<float> > SIFTFeat; //SIFT特征
    vector<float> x,y; //feature position
    nimg = db.image.size();
    for(i = 0; i < nimg; ++i) {//处理每张图像
      cerr<<"processing image " << i+1 << " "
          << db.image[i].imagePath<< endl;
      if(calcSiftFeats(db.image[i].imagePath.c_str(),SIFTFeat,x,y)){//fail +1
        failID.push_back(i);
        continue;
      }
      //write to ifs using CSV format
      //imageID,pos, feature
      for(j = 0; j < x.size(); ++j){ //输出每个特征到标准输出
        cout<<db.subToID[i]<<","<<"\"{" <<x[j]<<"," <<y[j] <<"}\""
            <<",\"{";
        for(k = 0; k < SIFTFeat[j].size()-1;++k)
          cout<<SIFTFeat[j][k] <<",";
        cout<<SIFTFeat[j][k] <<"}\"";
        cout<<endl;
      } //for
    }//for

    //输出统计信息
    cerr << "finished\n";
    cerr << "total " << i <<  " image" << endl;
    if(!failID.empty()){
      cerr << "fail(s) imageID:\n";
      for(i = 0;i < failID.size(); ++i)
        cerr << db.subToID[failID[i]] << endl;
    }//if
  }//try
  catch(exception &e)  {
    cerr  << e.what();
  }//catch
  
  return 0;  
}//main end

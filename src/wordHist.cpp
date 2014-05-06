/****************************************
计算视觉词汇频率直方图
@k为kmeans聚类时的簇的数目
****************************************/
#include <iostream>
#include <vector>
#include <stdexcept>

#include "iutility.hpp" //calcCodewords(...)
#include "exData.hpp"

using namespace std;

int
main(int argc, char **argv)
{
  if(argc != 2){
    cerr <<"generate visual word for selected images \n";
    cerr <<"usage:" << argv[0] << " <configuration_file> \n";
    return 1;
  }
  map<string,string> conf; //配置信息
  readConf(conf,argv[1]); //读取配置信息
  //check configuration
  if(!hasValue(conf,"k")) {// no k
    cerr <<"error:no k specific\n";
    return 1;
  }
  if(!hasValue(conf,"image"))
    cerr<<"using all image in database\n";
    
  try {
    exData db(conf["image"].c_str()); //数据类
    map<int,vector<float> > mword; //临时结果
    map<int,float>::iterator mit;
    vector<float> vtmp;
    int i; //循环变量
    int k; //k in kmeans
    int nimg; //图像数目
    vector<float> idf; //not used

    k = atoi(conf["k"].c_str());
    cerr<<"reading cluster center\n";
    db.readCenter(k); //读取簇中心及反向索引信息
    nimg = db.image.size();
    //清除原有数据
    for(i = 0; i < nimg; ++i){
      db.image[i].word.clear();
      db.image[i].word.resize(k);
    }

    for(i = 0; i< db.reverseIndex.size(); ++i){//遍历每个簇
      //遍历属于该簇的图像
      for(mit=db.reverseIndex[i].begin();mit!=db.reverseIndex[i].end();++mit)
        db.image[db.idToSub[mit->first]].word[i] = mit->second;
    }
    //将结果存入数据库
    db.writeWord();
  }//try
  catch (exception &e){
    cerr <<"error:"<<e.what();
  }
  return 0;
}



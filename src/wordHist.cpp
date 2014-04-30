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
  if(argc != 3){
    cerr <<"generate visual word histogram for every image in database\n";
    cerr <<"ussage:" << argv[0] << " <configuration_file> <sift_file>\n";
    return 1;
  }
  map<string,string> conf; //配置信息
  readConf(conf,argv[1]); //读取配置信息
  //check configuration
  if(!hasValue(conf,"k")) {// no k
    cerr <<"error:no k specific";
    return 1;
  }
  if(!hasValue(conf,"image"))
    cerr<<"using all image in database\n";
    
  try {
    exData db(conf["image"].c_str()); //数据类
    long i; //循环变量
    int k; //k in kmeans
    vector<float> idf; //not used

    k = atoi(conf["k"].c_str());
    cerr<<"reading cluster center\n";
    db.readCenter(k); //读取簇中心
    //读取每张图像的SIFT特征
    cerr<<"reading SIFT features\n";
    db.readSIFT(argv[2]);
    //计算视觉词汇频率直方图
    for(i = 0; i < db.image.size(); ++i){
      cerr <<"dealing image " << i+1 <<  endl;
      //计算视觉词汇频率直方图,并归一化为TF
      if(calcCodewords(db.image[i].SIFTFeat,db.clusterCenter, //result
                       db.image[i].word,idf) ){
            cerr << "error:calulate codewords" << endl;
            return 1;
      }
    }//for
    //将结果存入数据库
    db.writeWord();
  }//try
  catch (exception &e){
    cerr <<"error:"<<e.what();
  }
  return 0;
}



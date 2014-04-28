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
  if(argc != 2)
  {
    cerr <<"generate visual word histogram for every image in database\n";
    cerr <<"ussage:" << argv[0] << " <k> \n";
    return 1;
  }
  try
  {
    exData db; //数据类
    int i; //循环变量
    int k; //k in kmeans
    vector<float> idf; //not used
    //读取簇中心
    k = atoi(argv[1]);
    db.readCenter(k);
    //读取每张图像的SIFT特征
    db.readSIFT();
    //计算视觉词汇频率直方图
    for(i = 0; i < db.image.size(); ++i)
    {
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
  catch (invalid_argument &e)
  {
    cerr <<"error:"<<e.what();
  }
  return 0;
}



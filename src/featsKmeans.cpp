/****************************************
对SIFT特征进行聚类,并将结果写入数据库
聚类参数从配置文件获取,包括图像集，k,精度，最大迭代次数
****************************************/

#include <iostream>
#include <vector>
#include <stdexcept>
#include <map>
#include <string>
#include <cstdlib> //atoi(), atof() 

#include "opencv2/core/core.hpp" //Mat...
#include "iutility.hpp" //聚类
#include "exData.hpp" //exData类

using namespace std;
int
main(int argc, char **argv){
  if(argc != 2 && argc != 3){
    cerr <<"features cluster using kmeans\n";
    cerr <<"ussage:" << argv[0] << " <configure_file> [sift_csv_file]\n";
    cerr <<"if no sift_csv_file specific,features in database will be used\n";
    return 1;
  }
  map<string,string> conf; //配置信息
  readConf(conf,argv[1]);
  char *siftFile=NULL;
  if(argc == 3 )
    siftFile = argv[2];
  //测试配置信息
  if(conf["image"].empty()) //图像集
    cerr<<"using all image in database\n";
  else
    cerr<<"using image set with :" <<conf["image"] << "\n";
  if(conf["k"].empty()) {//k
    cerr<<"error: read k\n";
    return 1;
  }
  else
    cerr<<"k="<<conf["k"] <<"\n";
  if(conf["precision"].empty())  { //precision
    cerr<<"error: read precision\n";
    return 1;
  }
  else
    cerr<<"precison=" << conf["precision"] <<"\n";
  if(conf["maxCount"].empty()) {//max count in recursive
    cerr<<"error: read max count\n";
    return 1;
  }
  else
    cerr<<"maxCount="<<conf["maxCount"] <<"\n";
  
  try{
    exData db(conf["image"].c_str()); //数据集
    int k = atoi(conf["k"].c_str()); //簇的数目
    float precision = atof(conf["precision"].c_str()); //精度
    int maxCount = atoi(conf["maxCount"].c_str()); //最大迭代次数
    int i,j,n; //循环变量
    int nimg = db.image.size(); //图像数目
    int row,col; //所有特征的行和列
    int crow; ///当前行
    float *f;    //临时变量
    vector<float> vecf;  //临时变量
    cv::Mat mcenter,mlabel,mfeats; //簇中心，图像所属的簇，用于聚类的特征
    //kmeans收敛准则
    cv::TermCriteria tmcri(cv::TermCriteria::COUNT +
                           cv::TermCriteria::EPS,maxCount,precision);

    //读取SIFT,返回读取的SIFT特征的数目
    cerr<<"reading SIFT feature\n";
    row = db.readSIFT(siftFile);
    col = db.image[0].SIFTFeat[0].size();
    //初始化用于聚类的矩阵mfeats
    mfeats = cv::Mat(row,col,cv::DataType<float>::type); 
    //将特征类型vector<vector<float> > 转换为 cv::Mat
    for(crow = i = 0; i < nimg;++i) {//每张图片
      for(j = 0; j < db.image[i].SIFTFeat.size(); ++j, ++crow){//每个特征
        if(db.image[i].SIFTFeat[j].empty()) //no features element
          throw invalid_argument("error:no features");
        f = mfeats.ptr<float>(crow); //data is plain in Mat
        for(n = 0; n < db.image[i].SIFTFeat[j].size(); ++n) //每个元素
          f[n] = db.image[i].SIFTFeat[j][n];
      }
    }//for
    
    //cluster
    std::cerr << "using opencv kmeans\n";
    //attemps = 1,尝试一次
    std::cerr << "clustering\b";
    cv::kmeans(mfeats,k, mlabel,tmcri,1,cv::KMEANS_PP_CENTERS,mcenter);
    
    //将中心点cv::Mat类型装换为 vector<vector<double> >
    cerr<<"converting Mat\n";
    db.clusterCenter.clear();
    for(i = 0; i < mcenter.rows; ++i){ // 每行
      vecf.clear();
      f = mcenter.ptr<float>(i);
      for(j = 0; j < mcenter.cols; ++j)
        vecf.push_back(f[j]);
      db.clusterCenter.push_back(vecf);
    }//for
    
    //建立反向索引信息
    cerr<<"building reverse index information\n";
    db.reverseIndex.clear();
    db.reverseIndex.resize(k);
    for(crow = i = 0; i < nimg; ++i) //处理每张图像,"单词计数"
      for(j = 0; j < db.image[i].SIFTFeat.size(); ++j, ++crow)
        db.reverseIndex[mlabel.at<int>(crow)][db.subToID[i]] +=1;
    //转化为词汇频率，归一化
    for(i = 0; i < k; ++i){
      map<long,float>::iterator mit=db.reverseIndex[i].begin();
      for(; mit != db.reverseIndex[i].end(); ++mit)
        mit->second /= db.image[i].SIFTFeat.size(); //除以图像特征数目
    }//for
    
    //store to database
    cerr << "writing to database\n";
    db.writeCenter();
    cerr << "finished.\n";
  }
  catch(exception &e)
  {
    cerr  << e.what();
  }  
  return 0;
}

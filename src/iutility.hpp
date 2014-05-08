#ifndef IUTILITY_HPP

#define IUTILITY_HPP
/*this file include some utility function
  to use them you should include the utility.hpp
*/

#include <cmath>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <map>
#include <queue>
#include <iterator>
#include <algorithm>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "exData.hpp" //exData

enum FEATTYPE{ENUM_SIFT,ENUM_SURF};

template<class P>
float
squarCosSimilarity(const P &a1,const  P &a2, int d)
{
  //calculate the squar cosin similarity of two d dimension vector
  //@param d is the demenstration of the vector
  //type P must operator[]
  int i;
  float as1 = 0.0, as2 = 0.0, b1 = 0.0;
  for(i = 0; i < d; ++i){
    b1 += a1[i] * a2[i];
    as1 += a1[i] * a1[i];
    as2 += a2[i] * a2[i];
  }
  return (b1 * b1) / (as1 * as2);
    
}//squarCosSimilarity() end

template<class P>
inline float
cosS(const P &a1,const P &a2, int d)
{
  return sqrt(squarCosSimilarity(a1,a2,d));
}//


template<class P>
float jointSimilarity(const P &a1,const P &a2, int d) 
{
  //calculate the joint similarity of two d dimension vector
  int i;
  float  ups, lows;
  for(lows = ups = 0.0, i = 0; i < d; ++i){
    ups += (a1[i] >= a2[i] ? a2[i] : a1[i]);
    //lows +=(a1[i] >= a2[i] ? a1[i] : a2[i]);
    //lows +=a2[i];
    lows += a1[i];
  }

  return ups/lows;
}

  

template<class T> inline float
squarDistance(const T &a1, const T &a2, int d)
{
  //template function  squar of Euclidean distance
  //assume type or class T has operator[], "point"
  //@parameter d is the dimension of the 'vector'
  int i;
  float sum;
  for(sum = i = 0; i < d; ++i )
    sum += (a1[i] - a2[i])*(a1[i] - a2[i]);
  return sum;
}//function squarDistance() end

int
calcCodewords(const std::vector<std::vector<float> > &feats, 
              const std::vector<std::vector<float> >  &center,
              std::vector<float> &res,std::vector<float> &idf,
              int fDemension = 0, bool usingtf=true)
	 
{
  //calculate the codewords for an image
  //final result is k dimension which also means center has 'k' line
  //return 0 if success
  //if usingtf is true(not 0) then use tf default use tf
  int i,nearest_index;
  float nearest_d, cur_d;
  std::vector< std::vector<float> >::const_iterator it;
  int cwDemension,featsDemension;
    
  cwDemension = center.size();
  if(!cwDemension)//center is empty
    return -1;
  featsDemension =(fDemension == 0)?center[0].size(): fDemension;
  //initialize the result vector
  res.clear();
  res.assign(cwDemension, 0);
  //go through all features
  //O(n^2)
  std::cerr << "feat's demension = " << featsDemension
            << "\tcodewords demension = " << cwDemension
            << "\timage feats count = " << feats.size() << std::endl;
  for(it = feats.begin(); it != feats.end(); ++it){
    //init use the center[0]
    nearest_index = 0;
    nearest_d = squarDistance(*it, center[0],featsDemension);
    //find nearest center
    for(i = 1; i < cwDemension; ++i){
      cur_d = squarDistance(*it, center[i], featsDemension);
      if(nearest_d >= cur_d){
        nearest_d = cur_d;
        nearest_index = i;
      }
    }
    //store result int
    ++res[nearest_index];
  }
#if 0
  //calculate idf
  for(i = 0; i < cwDemension; ++i)
    if(res[i] > 0)
      ++idf[i];
#endif

  //judge if use tf
  if(usingtf){
    //transfer to 100% -> term fruency(tf)
    for(i = 0; i < cwDemension; ++i)
      res[i] = res[i] / feats.size();
  }
    
  return 0;
}//calcCodewords end


//计算特征结果放入feats,位置信息放在pts
int
calcFeats(const char *img_file, std::vector<std::vector<float> > &feats,
          std::vector<float> &x,std::vector<float> &y,int type)
{
  //the features are stored in feats
  //return 0 if success
  //type can be 0(SURF) or 1(SIFT)
  cv::Mat img = cv::imread(img_file,0);
  cv::Mat mask, descr;
  cv::SIFT sift;
  cv::SURF surf;
  std::vector<cv::KeyPoint> kps;
  float *dp;
  int n, m, i, j; //m = 128
  if(!img.data){
    return -1;
  }

  //calculate  features
  if(type == ENUM_SIFT)
    sift(img,mask,kps,descr); //descr.depth() == sizeof(float)
  else if(type == ENUM_SURF)
    surf(img,mask,kps,descr);
  else
    return 1;
  n = descr.rows;
  m = descr.cols;
  //check whether the image has features
  if(n == 0 || descr.empty())
    return 2;
  //store features
  feats.clear();
  feats.resize(n);
  x.clear();
  y.clear();
  float f;
  for(i = 0; i < n; ++i){
    dp = descr.ptr<float>(i);
    // std::cerr<<"("<<kps[i].pt.x<<","<<kps[i].pt.y<<")\n";
    f=kps[i].pt.x;
    x.push_back(f);
    f=kps[i].pt.y;
    y.push_back(f);
    for(j = 0; j < m; ++j)
      feats[i].push_back(dp[j]);
  }
  return 0;
}//calcFeats(...) end
inline int
calcSiftFeats(const char *img, std::vector<std::vector<float> > &res)
{
  std::vector<float> x,y;
  return calcFeats(img,res,x,y,ENUM_SIFT);
}
inline int
calcSiftFeats(const char *img, std::vector<std::vector<float> > &res,
              std::vector<float> &x, std::vector<float> &y)
{
  return calcFeats(img,res,x,y,ENUM_SIFT);
}
inline int 
calcSurfFeats(const char *img,std::vector<std::vector<float> > &res,
              std::vector<float> &x, std::vector<float> &y)
{
  return calcFeats(img,res,x,y,ENUM_SURF);
}

int
calcGrtFeats(char *imgPath, std::vector<std::vector<float> > &feats)
{
  /*
    calcute gradient features of @imgPath
    Suppose that p(x,y) is the value of point (x,y) in the image.
    Define gradient features of point(x,y) as a two demension vector
    whose first element is
    sqrt((p(x+1,y)-p(x-1,y))^2+(p(x,y+1)-p(x,y-1))^2)
    and second element is the direction of the gradient, that is
    arctan((p(x+1,y)-p(x-1,y)) / (p(x,y+1)-p(x,y-1)))
    Note that if the first element shoud not include 0
    if success return 0
  */
  std::vector<float> featElem; // a feat
  cv::Mat kernel; //convolution kernel 
  cv::Mat xgrt, ygrt; //x and y coordinate gradient
  cv::Mat m = cv::imread(imgPath, 0); //load image with gray scale mode
  if(!m.data)//cannot read imgPath
    return -1;
  //init feats
  feats.clear();
  return 0;
    
}



struct resultIndex
{
  int id; //图像ID
  float sim; //相似度
  bool operator<(const resultIndex &o2) const{
    return this->sim < o2.sim;
  }
};
bool cmp(const resultIndex &o1, const resultIndex &o2){
  return o1.sim > o2.sim;
}
struct revType{ //用于从反向索引计算余弦相似度
  float up;  //分子
  float l1,l2; //分母
};

//match visual word histogram in exData::image
//@n is the top n image to return
std::vector<int>
matchWord(const std::vector<std::vector<float> > &userFeats,
          exData &db,int n = 10, float s = 0.1){
  int i; // 循环变量
  int d; //词汇维度
  resultIndex tmp; //临时结果
  std::vector<resultIndex> vtmp; //临时结果
  std::priority_queue<resultIndex>  resPQ; //临时结果,反向索引实现
  std::vector<float> idf; //not use
  std::vector<int>  result;//result
  std::vector<float> userWord;

  std::map<int,revType> revSim; //中间结果
  std::map<int,revType>::iterator rmit;
  std::map<int,float>::iterator mit,mend; //遍历反向索引

  //计算userfeats 词汇频率
  i = calcCodewords(userFeats,db.clusterCenter,userWord,idf);
  if(i || userWord.empty())
    throw std::invalid_argument("empty  user image word");
  if(userWord.size() != db.reverseIndex.size())
    throw std::invalid_argument("size of user word != cluster size");
  clock_t t=clock();
#if 0 //使用反向索引
  std::cerr<<"\nusing reverse information\n";
  for(i = 0; i < db.reverseIndex.size(); ++i){ //遍历每个簇中心
    if(userWord[i] == 0) //ignore
      continue;
    for(mit = db.reverseIndex[i].begin(),mend=db.reverseIndex[i].end();
        mit != mend; ++mit){ //计算属于这个簇的图像相似度
      revSim[mit->first].up += userWord[i]*mit->second;
      revSim[mit->first].l1 += userWord[i]*userWord[i];
      revSim[mit->first].l2 += mit->second*mit->second;
    }
  }//for

  //计算最终结果,revType::sim
  for(rmit = revSim.begin();rmit != revSim.end(); ++rmit){ //计算余弦相似度^2
    tmp.sim=(rmit->second.up*rmit->second.up)/(rmit->second.l1*rmit->second.l2);
    tmp.id= rmit->first;
    resPQ.push(tmp);
  }
  //提取结果，使用优先队列
  result.clear();
  for(i = 0; i < n && resPQ.top().sim >= s; ++i){
    std::cerr<<db.image[db.idToSub[resPQ.top().id]].imagePath<<"\t"<<resPQ.top().sim<<"\n";
    result.push_back(resPQ.top().id);
    resPQ.pop();
  }
  std::cerr<<"time used:"<<(float)(clock()-t)/CLOCKS_PER_SEC << "\n";
  
#endif 
#if 1 //没使用反向索引
  t = clock();
  std::cerr<<"\nusing plain compare\n";
  //使用用户词汇频率作为维度
  d = userWord.size();
  vtmp.clear();
  std::cerr <<"db size:" <<db.image.size() <<std::endl;
  for(i = 0; i < db.image.size(); ++i){//每张图像
    //using cosin similarity
    if(db.image[i].word.size() != d)  {
      std::cerr <<"cur image " << i<< std::endl;
      std::cerr <<"word size :" <<db.image[i].word.size() <<std::endl;
    }
      
    tmp.sim = cosS(db.image[i].word, userWord, d);
    tmp.id = i; 
    vtmp.push_back(tmp);
  }
  std::cerr <<"image number" << i << std::endl;
  //sort the result
  std::sort(vtmp.begin(), vtmp.end(),cmp);
  //提取结果
  result.clear();
  for(i = 0; i < n && vtmp[i].sim > s; ++i){
    result.push_back(vtmp[i].id);
    std::cerr<<vtmp[i].sim << "\n";
  }
  std::cerr<<"time used:"<<(float)(clock()-t)/CLOCKS_PER_SEC << "\n";
  
#endif 
  
  return result;
}

#if 0
int
calcHumoments(const char *imgPath, std::vector<float> &vec){
  /*calculate all hu moments  to vec
    if success return 0
  */
  float hu[7];
  cv::Moments mnts;
  cv::Mat m = cv::imread(imgPath, 0);

  //check
  if(!m.data)
    return -1;

  mnts = cv::moments(m);
  cv::HuMoments(mnts, hu);
  vec.clear();
  for(int i = 0; i < 7; ++i)
    vec.push_back(hu[i]);   //  res.push_back(vec);
  
  return 0;
}
#endif

//read configure file information
//configure file format
//key="value"
//value should not contain quote "
//spaces are ignored
//#开头的为注释
void readConf(std::map<std::string,std::string> &res,
              const char *file)
{
  std::ifstream ifs(file);
  std::stringstream sst;
  std::string s, key,value;
  std::size_t st,st2;
  bool assign=false;
  char c;
  int i = 0; //read line
  if(!ifs){
    std::cerr<<"error:open configuration file:" <<file << "\n";
    exit(1);
  }
  
  while(getline(ifs,s)){
    ++i;
    sst.str("");
    sst.clear();
    sst << s;
    while(sst.good()){ //跳过空格 
      c = sst.get();
      if(c == ' ' || c == '\n' || c =='\t')
        continue;
      sst.putback(c);
      break;
    }//while
    getline(sst,s);
    if(s[0] == '#') //skip comment
      continue;
    //读取key
    //找到第一个 =
    st = s.find('=');
    if(st == std::string::npos){
      std::cerr<< "error:read configure file:line" << i <<"\n";
      exit(1);
    }
    s[st] = ' ';
    sst.str("");
    sst.clear();
    sst << s;
    sst >> key;
    getline(sst,s);
    //读取value
    st = s.find_first_of('\"');
    if(st == std::string::npos){
      std::cerr<< "error:read configure file:line" << i <<"\n";
      exit(1);
    }
    s[st] = ' ';
    st2 = s.find_last_of('\"');
    if(st2 == std::string::npos){
      std::cerr<< "error:read configure file:line" << i <<"\n";
      exit(1);
    }
    value = s.substr(st+1, st2-st-1);
    res[key] = value;
  }//while
}//readConf

//test the configuration for string s
bool hasValue(std::map<std::string,std::string> conf, const char *s){
  return conf.end() != conf.find(s);
}
#endif//iutility.hpp end

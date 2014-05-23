//输出视觉词汇频率直方图，计数信息
#include <iostream>
#include <string>
#include <map>
#include "iutility.hpp" //matchWord
#include "exData.hpp"  //class exData

using namespace std;
int main(int argc,char **argv){
  if(argc != 2) {
    cerr << "以带CSV格式输出视觉词汇计数直方图到标准输出,CSV头为\nimagepath,visual_word_count\n";
    cerr << "usage: " << argv[0] << " <configuration_file>\n";
  }

  try{
    int k,i,j;
    map<string,string> conf; //配置信息
    readConf(conf,argv[1]); //读取配置信息
    //read from configuration file
    k = atoi(conf["k"].c_str());

    exData db(conf["image"].c_str(),conf["PGURI"].c_str()); //数据库数据
    map<int,float>::iterator mit;
    cerr<<"reading cluster center and reverse information\n";
    db.readCenter(k); //get cluster center

    //gen data
    cout << "imagepath,visual_word_count\n";
    for(i = 0; i < db.image.size(); ++i){
      db.image[i].word.resize(k);
      for(j = 0; j < k ;++j)
        db.image[i].word[j] = db.reverseIndex[j][db.subToID[i]];
      cout << db.image[i].imagePath << ",\"{";
      for(j = 0; j< k-1; ++j)
        cout << db.image[i].word[j] << ",";
      cout << db.image[i].word[j] << "}\"";
      cout << endl;
    }
  }
  catch (exception e){
    cerr<<e.what();
  }
  return 0;
}

/****************************************
图像检索服务器
****************************************/
extern "C"{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
}

#include <iostream>
#include <string>

#include "iutility.hpp" //matchWord
#include "exData.hpp"  //class exData
#include "imgret.pb.h" //google protobuf

//Xushuang's HS match
#include "ColorFilter.h"
#include "cFilter.h"
#include "ColorHist.h"
#include "matchBaseColor.h"


#define DEBUG 1
#if DEBUG
#define DEBUG_OUTPUT   {                                            \
    for(i = 0; i < result.size(); ++i)                              \
      cerr << i+1 <<"\t" << db.image[result[i]].imagePath << endl;}
#endif

void dealRequest(int fd, exData &db); //deal request
#define TMP_FILE "userData/cache/server_XXXXXXXXXX" //for mkstemp()
const int MAXSOCKETBUF= 10240;  //socket最大缓冲长度
const int DEFAULTPORT = 9991; //默认监听端口
extern int errno;

using namespace std;
int main(int argc, char** argv)
{

  if(argc != 4 && argc != 3 && argc != 2){
    cerr <<"image retrieval server\n";
    cerr <<"ussage:" << argv[0]
         << " <configuration_file> [k] [port] \n";
    return 1;
  }
  int port = DEFAULTPORT; //监听端口
  int k=0;  //视觉词汇维度
  int hbin = 30,sbin = 32; //HS直方图的h,s维度,默认为30,32
  map<string,string> conf; //配置信息
  readConf(conf,argv[1]); //读取配置信息

  if(argc == 3 || argc == 4) {//set k and port
    k = atoi(argv[3]);
    if(argc == 4) //set port
      port = atoi(argv[2]);
  }
  else { //read from configuration file
    if(!hasValue(conf,"k")){
      cerr<<"error:no k in configuration file\n";
      return 1;
    }
    k = atoi(conf["k"].c_str());
    if(!hasValue(conf,"port"))
      cerr<<"info:using default port:"<<DEFAULTPORT <<"\n";
    port = atoi(conf["port"].c_str());
  } //port and k
  if(!hasValue(conf,"image"))
    cerr<<"info:using all image in database\n";
    
  try{
    exData db(conf["image"].c_str()); //数据库数据
    int    listenfd, cltfd;
    struct sockaddr_in     servaddr;
    int pid;  //进程id
    //initialize
    cerr<<"reading cluster center and reverse information\n";
    db.readCenter(k); //get cluster center
    cerr<<"reading imagevisual word\n";
    db.readWord(k); //get word for every image
    // cerr<<"reading hs histogram\n";
    //db.readHS(hbin,sbin);
          
    //create master socket
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
      cerr <<"error:create socket" <<strerror(errno) <<endl;
      return 1;
    }
    //initialize server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //accept any user
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    //bind to the port
    if( bind(listenfd, (struct sockaddr*)&servaddr,
             sizeof(servaddr) ) == -1){
      cerr<<"error:bind socket" <<strerror(errno)<<endl;
      return 1;
    }
    //listening
    if( listen(listenfd, 10) == -1){
      cerr<<"error:listen socket "<< strerror(errno) <<endl;
      return 1;
    }
    cerr<<"info:listening\n";
    while(1){
      //accept request
      //client's addr information is not used
      cltfd = accept(listenfd,(struct sockaddr*)NULL, NULL);
      //accept error
      if(cltfd== -1){
        cerr<<"error:accept socket "<< strerror(errno) << endl;
        continue;
      }

      //deal request
      pid = fork();
      if(!pid){ //child for deal request
        close(listenfd);
        dealRequest(cltfd, db);
        exit(0); //child exit
      }
      else if(pid > 0) //parent
        close(cltfd);
      else//fork() error
        cerr << "error:fork() " << strerror(errno) << endl;
    }//while
    close(listenfd);
  }//try
  catch (invalid_argument &e)
  {
    cerr <<"error:"<<e.what();
  }
  return 0;
}


void
dealRequest(int fd, exData &db){
  using namespace dmir; //protobuf
  char buf[MAXSOCKETBUF];
  char imgPath[]=TMP_FILE; 
  vector<vector<float> > userFeats;  //用户图像特征
  size_t n;
  int i;
  Request  rq;  //查询请求
  Result   rs;  //查询结果
  vector<int> result;
  vector<int> originID; //原始id
  string s; //temp string
  
  //read request
  n  = read(fd, buf, MAXSOCKETBUF);
  if(n == 0){//no data
    cerr << "warn:recieve 0 byte data from client:"
         << strerror(errno) << endl;
    write(fd,"",0);
    return;
  }//if
  //parse data
  rq.ParseFromString(string(buf,n));
  
  if(rq.has_image())   //first request
  {
    int imgfd; //图像临时文件
    //生成临时文件
    imgfd=mkstemp(imgPath);
    if(fd == -1) {//生成文件失败
      cerr <<"error: generate tmp file\n";
      return;
    }
    //写入临时图像文件
    if(-1 == write(imgfd,rq.image().c_str(),rq.image().size())){
      cerr<<"error:write tmp file\n";
      return;
    }
    close(imgfd);
    cerr <<"info:user image path:" <<imgPath << endl;
    calcSiftFeats(imgPath, userFeats);
    //match word histogram 相似度至少为0.1 最多500张
    result = matchWord(userFeats,db,500,0.1);
#if DEBUG
    cerr<<"info:word match result\n";
    DEBUG_OUTPUT
#endif
      }else     //filter
    for(i =0 ; i < rq.id_size(); ++i)
      result.push_back(rq.id(i));
  originID = result; //save originID


#if 1 //hs match
  matchBaseColor(imgPath, db,result);
#if DEBUG
  cerr<<"info:hs histogram match result\n";
  DEBUG_OUTPUT
#endif //debug
#endif //hs match

#if 1 //hs filter
    vector<CvScalar> acolor(0); //必须出现的颜色,RGB
  vector<CvScalar> dcolor(0); //不能出现的颜色, RGB
  CvScalar tmpcvs;
  //accept color
  for(i = 0; i < rq.needcolor_size() ;++i)
  {
    const Request::Color& ac=rq.needcolor(i);
    tmpcvs.val[0] = ac.r();
    tmpcvs.val[1] = ac.g();
    tmpcvs.val[2] = ac.b();
    acolor.push_back(tmpcvs);
  }
  //deny color
  for(i = 0; i < rq.denycolor_size() ;++i)
  {
    const Request::Color& ac=rq.denycolor(i);
    tmpcvs.val[0] = ac.r();
    tmpcvs.val[1] = ac.g();
    tmpcvs.val[2] = ac.b();
    dcolor.push_back(tmpcvs);
  }
  cerr <<"info:start hs filter\n";
  //filter color
  cFilter(result, acolor, dcolor, db);
#if DEBUG
  cerr<<"info:color filter result\n";
  DEBUG_OUTPUT
#endif //debug
#endif
    //set result
    //image path
    for(i = 0; i < result.size(); ++i)
      rs.add_imgpath(db.image[result[i]].imagePath);
  //save origin id
  for(i = 0; i< originID.size(); ++i)
    rs.add_id(originID[i]);
  //precision miss...
  //sent to client
  rs.SerializeToString(&s);
  n = write(fd,s.c_str(),s.length());
  if(n == -1){//sent error
    cerr <<"error:send to client :" <<strerror(n)<<endl;
  }
  close(fd);
}


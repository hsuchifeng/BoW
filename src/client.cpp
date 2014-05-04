//image retrival client
extern "C"{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "imgret.pb.h" //protobug

#define DEFAULTSERVER "10.21.23.104"
const int MAXSOCKETBUF= 9600;
const int DEFAULTPORT= 9991;
using namespace std;

int main(int argc, char** argv)
{
  int sockfd, n;
  int iport = DEFAULTPORT;
  struct sockaddr_in servaddr;
  char *imgPath, *serip = DEFAULTSERVER, buf[MAXSOCKETBUF];
  string s;
  string img; //image file content
  dmir::Request rq; //proto request
  ifstream ifs;

  //check parameter
  if( argc != 4 && argc != 3 && argc != 2){
    printf("ussage:%s <path-to-image> [server] [port]\n",argv[0]);
    exit(0);
  }
  imgPath = argv[1];
  if(argc == 3 || argc == 4){
    serip = argv[2];
    if(argc == 4)
      iport = atoi(argv[3]);
  }

  //create socket
  if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    cerr<<"create socket error:"<< strerror(errno) << endl;
    exit(0);
  }

  //initialize socket
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(iport);
  if( inet_pton(AF_INET, serip, &servaddr.sin_addr) <= 0){
    cerr<<"inet_pton error for " << serip;
    exit(0);
  }

  //connect server
  if( connect(sockfd, (struct sockaddr*)&servaddr,
              sizeof(servaddr)) < 0){
    cerr<<"connect error:"<< strerror(errno);
    exit(0);
  }

  //set request
  ifs.open(imgPath,ios::in); //read image file
  if(ifs) 
    while(!ifs.eof())
      img.push_back(ifs.get());
  ifs.close();
  rq.set_image(img);
  rq.SerializeToString(&s);
  
  //sent request
  printf("sending...\n");
  if( write(sockfd, s.c_str(), s.length()) < 0){
    cerr<<"send request error:"<< strerror(errno);
    exit(0);
  }
  //receive result
  if((n = read(sockfd, buf, sizeof(buf) ))< 0)
    cerr<<"read error\n";
  close(sockfd);  

  return 0;
}

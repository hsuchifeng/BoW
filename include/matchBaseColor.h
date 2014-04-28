#ifndef _MATCHBASECOLOR
#define _MATCHBASECOLOR
#include <iostream>
#include <fstream>
#include "ColorHist.h"
#include <ctime>

using namespace cv;
using namespace std;
/*
参数：pictureName[文件路径]； database:数据库对象（成员image已被赋值）；indexResult:查找结果图片索引
功能：查找数据库中与pictureName图片匹配的图片，查找到的图片索引保存到indexResult中
*/
void matchBaseColor(const char pictureName[], exData &database, vector<int> &indexResult);
#endif
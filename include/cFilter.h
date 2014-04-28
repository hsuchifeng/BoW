#ifndef _CFILTER
#define _CFILTER
#include "ColorFilter.h"
#include <iostream>


using namespace std;
using namespace cv;
/*
参数：match：将被筛选的数组；rgb:必须有的颜色数组；notRgb:不能有的颜色数组; database:数据库对象（成员image已经被赋值）
*/
void cFilter(vector<int> &match, vector<CvScalar> &rgb, vector<CvScalar> &notRgb, exData &database);
#endif

#ifndef _COLORFILTER
#define _COLORFILTER
#include "ColorHist.h"
#include <cv.h>
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream>

using namespace std;

typedef struct PCM
{
	string loc;	//图片路径
	long index;//索引
	double matchValue;	//跟颜色比较获得的权值
} PCM;

class ColorFilter 
{
public:
	vector<PCM> pcm;	//保存图片信息
private:
	ColorHist hist;
	int h_bins, s_bins;
public:
	//给ColorHist::h_bins, ColorHist::s_bins赋值
	void setBins(int h, int s);
	//rgb颜色转换为hsv颜色
	void RGBToHSV(vector<CvScalar> &rgb, vector<CvScalar> &hsv);
	//hsv颜色转hsv颜色
	void HSVToRGB(vector<CvScalar> &hsv, vector<CvScalar> &rgb);
	//pL：图片集索引, hsv:颜色集
	//功能：图片集与hsv集进行匹配，对图片集进行重新排序
	void match(vector<int> &pL, vector<CvScalar> &hsv, vector<CvScalar> &notHsv, exData &database);
	~ColorFilter();
};
#endif

#ifndef _COLORHIST
#define _COLORHIST
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "exData.hpp"

using namespace std;


//保存在文件中的结构体
typedef struct nameAndHist
{
	string loc;	//图片文件路径	
	CvHistogram *hist;//颜色直方图
	int index;	//图片在内存数组中对应的下标
} NAH;
//匹配结果结构体
typedef struct matchResult
{
	string loc;	//图片路径
	long index;	//图片在内存数组中对应的下标
	double d;	//匹配度
} MR;

class ColorHist
{
public:
	//图片匹配结果
	vector<MR> result;
	//图片库直方图
	vector<NAH> data;
private:
	vector<float *> dataVec;	//保存mat中的data数据指针
	void add(MR t);	//将t添加到result中恰当的位置
	int h_bins, s_bins;	//h分量跟s分量的划分
	int hist_size[2];
	/** H 分量的变化范围 */
	float h_ranges[2]; 	
	/** S 分量的变化范围*/
	float s_ranges[2];
	float *hist_ranges[2];
private:
public:
	//获取一张图片的直方图，并把直方图保存在hist中
	//pictureName:图片路径,hist：保存图片直方图
	void pictureHist(const char * pictureName, CvHistogram *& hist);
	//给h_bins, s_bins赋值
	void setBins(int h, int s);	
	//读取数据库中的数据到data中
	void readData(exData &database);
	//读取数据库中部分数据到data中
	//fewPictue:图片在数据库中的索引，database:数据库类对象
	void readFewData(vector<int> &fewPicture, exData &database);	
	//toMatch直方图与直方图数据库进行比较，匹配结果保存到属性result中，
	//如果n >= 0 使用n，否则使用精度，返回匹配匹配结果图片数
	//toMatch匹配的图片直方图，n匹配结果数量，lowestPre最小精度
	int match(CvHistogram* toMatch, int n, double lowestPre = 0);
	//toMatch直方图与数据库中索引在index中数组中的直方图进行比较，匹配结果保存到属性result中，
	//如果n >= 0 使用n，否则使用精度，返回匹配匹配结果图片数
	//toMatch匹配的图片直方图，n匹配结果数量，lowestPre最小精度
	int match(CvHistogram *toMatch, vector<int> &index, int n, double lowestPre);
	//写数据到数据库中
	void writeData(exData &database);
	//建立直方图,失败返回false
	void buildHist(exData &database);
	//建立多张图片的直方图
	//imgs:多张图片路径	
	void buildHist(vector<string> &imgs);
	//释放数据空间
	void release();
	~ColorHist();
};
#endif

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
	//exData database;	//数据库对象
	vector<float *> dataVec;	//保存mat中的data数据指针
	void add(MR t);	//将t添加到result矢量中恰当的位置
	int h_bins, s_bins;	//h分量跟s分量的划分
	int hist_size[2];
	/** H 分量的变化范围 */
	float h_ranges[2]; 
	
	/** S 分量的变化范围*/
	float s_ranges[2];
	float *hist_ranges[2];
private:
public:
	//获取一张图片的直方图
	void pictureHist(const char * pictureName, CvHistogram *& hist);
	void setBins(int h, int s);	//给h_bins, s_bins赋值
	//读取全部数据到data中
	bool readData(const char *db, exData &database);
	//读取部分数据到data中
	bool readFewData(vector<int> &fewPicture, exData &database);
	//fileName匹配的图片路径，index:sift匹配结果的索引,n匹配结果数量，lowestPre最小精度
	//对fileName建立直方图并与索引库index进行比较，如果n >= 0 使用n，否则使用精度，返回匹配图片数量
	int match(CvHistogram *toMatch, vector<int> &index, int n, double lowestPre);
	//写数据到db文件中
	bool writeData(const char *db, exData &database);
	//根据plLoc占位符无其他作用,创建数据库中图片路径的颜色直方图
	bool buildHist(const char * plLoc, exData &database);
	//imgs:多张图片路径
	//建立多张图片的直方图
	void buildHist(vector<string> &imgs);
	//释放数据空间
	void release();
	~ColorHist();
};
#endif
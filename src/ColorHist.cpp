
#include "ColorHist.h"

void ColorHist::readFewData(vector<int> &fewPicture, exData &database)
{
	data.clear();
	dataVec.clear();
	for(int i = 0; i < fewPicture.size(); i++)
	{
		NAH t;
		t.loc = database.image[fewPicture[i]].imagePath;
		dataVec.push_back(&(database.image[fewPicture[i]].hs[0]));
		t.hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, hist_ranges, 1);
		cvMakeHistHeaderForArray(2, hist_size, t.hist, &(database.image[fewPicture[i]].hs[0]), hist_ranges, 1);
		t.index = fewPicture[i];
		data.push_back(t);
	}
}

void ColorHist::readData(exData &database)
{
	data.clear();
	dataVec.clear();
	for(int i = 0; i < database.image.size(); i++)
	{
		NAH t;
		t.loc = database.image[i].imagePath;
		dataVec.push_back(&(database.image[i].hs[0]));
		t.hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, hist_ranges, 1);
		cvMakeHistHeaderForArray(2, hist_size, t.hist, &(database.image[i].hs[0]), hist_ranges, 1);
		t.index = i;
		data.push_back(t);
	}
}

bool cmp(const MR &a,const MR &b)
{
	return a.d > b.d;
}

void ColorHist::add(MR t)
{
	int i;
	for(i = result.size() - 2; i >= 0; i--)
	{
		if((int)(result[i].d * 10000)< (int)(t.d * 10000))
			result[i + 1] = result[i];
		else 
		{
			result[i + 1] = t;
			break;
		}
	}
	if(i == -1)
		result[0] = t;
}

int ColorHist::match(CvHistogram *toMatch, vector<int> &index, int n, double lowestPre)
{
	//提取n张匹配图片
	result.clear();
	MR t; 
	if(n > 0)
	{
		for(int i = 0; i < index.size(); i++)
		{
			t.loc = data[index[i]].loc;
			t.index = data[index[i]].index;
			t.d = cvCompareHist(toMatch, data[index[i]].hist, CV_COMP_INTERSECT);
			if(i < n)
			{
				result.push_back(t); continue;
			}
			else if(i == n)
				sort(result.begin(), result.end(), cmp);
			if(t.d > result[result.size() - 1].d)
				add(t);	
		}	
	}
	//提取精度大于等于lowestPre的图片
	else
	{
		for(int i = 0; i < index.size(); i++)
		{
			t.loc = data[index[i]].loc;
			t.index = data[index[i]].index;
			t.d = cvCompareHist(toMatch, data[index[i]].hist, CV_COMP_INTERSECT);
			if(t.d >= lowestPre)
			{
				result.push_back(t);
			}
		}
		sort(result.begin(), result.end(), cmp);
	}
	return 0;
}

int ColorHist::match(CvHistogram *toMatch, int n, double lowestPre)
{
	//提取n张匹配图片
	result.clear();
	MR t; 
	if(n > 0)
	{
		for(int i = 0; i < data.size(); i++)
		{
			t.loc = data[i].loc;
			t.index = data[i].index;
			t.d = cvCompareHist(toMatch, data[i].hist, CV_COMP_INTERSECT);
			if(i < n)
			{
				result.push_back(t); continue;
			}
			else if(i == n)
				sort(result.begin(), result.end(), cmp);
			if(t.d > result[result.size() - 1].d)
				add(t);	
		}	
	}
	//提取精度大于等于lowestPre的图片
	else
	{
		for(int i = 0; i < data.size(); i++)
		{
			t.loc = data[i].loc;
			t.index = data[i].index;
			t.d = cvCompareHist(toMatch, data[i].hist, CV_COMP_INTERSECT);
			if(t.d >= lowestPre)
			{
				result.push_back(t);
			}
		}
		sort(result.begin(), result.end(), cmp);
	}
	return 0;
}

void ColorHist::buildHist(exData &database)
{
	NAH t;
	data.clear();
	for(int i = 0; i < database.image.size(); i++)
	{
		t.loc = database.image[i].imagePath;
		t.index = i;
		pictureHist(t.loc.c_str(), t.hist);
		data.push_back(t);
	}
}

void ColorHist::buildHist(vector<string> &imgs)
{
	NAH t;
	data.clear();
	for(int i = 0; i < imgs.size(); i++)
	{
		t.loc = imgs[i];
		pictureHist(imgs[i].c_str(), t.hist);
		data.push_back(t);
	}
}

void ColorHist::pictureHist(const char * pictureName, CvHistogram *& hist)
{
	IplImage * src= cvLoadImage(pictureName);
 
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };

	/** 输入图像转换到HSV颜色空间 */
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	/** 创建直方图，二维, 每个维度上均分 */
	hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, hist_ranges, 1 );
	/** 根据H,S两个平面数据统计直方图 */
	cvCalcHist( planes, hist, 0, 0 );
	float * f = cvGetHistValue_2D(hist, 0, 0);
	(*f) = 0;
	cvNormalizeHist(hist, 1);
	cvThreshHist(hist, 0.0001);
	cvReleaseImage(&src);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
}

void ColorHist::release()
{
	for(int i = 0; i < data.size(); i++)
	{
		cvReleaseHist(&data[i].hist);
	}
	dataVec.clear();
	data.clear();
	result.clear();
}

void ColorHist::writeData(exData &database)
{

	database.hbin = h_bins;
	database.sbin = s_bins;
	for(int i = 0; i < data.size(); i++)
	{
		for(int j = 0; j < h_bins; j++)
			for(int k = 0; k < s_bins; k++)
				database.image[i].hs.push_back(cvQueryHistValue_2D(data[i].hist, j, k));
	}
	database.writeHS();
}

void ColorHist::setBins(int h, int s)
{
	h_bins = h, s_bins = s;
	hist_size[0] = h_bins, hist_size[1] = s_bins;
	/** H 分量的变化范围 */
	h_ranges[0] = 0;h_ranges[1] = 180;
	/** S 分量的变化范围*/
	s_ranges[0] = 0;s_ranges[1] = 255;
	hist_ranges[0] = h_ranges;hist_ranges[1] =s_ranges;
}

ColorHist::~ColorHist()
{
	release();
}

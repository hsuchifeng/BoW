#include "ColorFilter.h"

//给ColorHist::h_bins, ColorHist::s_bins, ColorHist::v_bins赋值
void ColorFilter::setBins(int h, int s)
{
	h_bins = h, s_bins = s;
	hist.setBins(h, s);
}

//sort比较函数cmp
bool cmp(const PCM & a, const PCM & b)
{
	return a.matchValue > b.matchValue;
}

void ColorFilter::RGBToHSV(vector<CvScalar> &rgb, vector<CvScalar> &hsv)
{
	IplImage * src = cvCreateImage(cvSize(100, 100), 8, 3);
	IplImage * hsvt = cvCreateImage(cvGetSize(src), 8, 3);
	int k = 0;
	for(int x = 0; x < src->height && k < rgb.size(); x++)
	{
		for(int y = 0; y < src->width && k < rgb.size(); y++, k++)
		{
			cvSet2D(src, x, y, cvScalar(rgb[k].val[0], rgb[k].val[1], rgb[k].val[2]));
		}
	}
	cvCvtColor(src, hsvt, CV_RGB2HSV);
	hsv.clear();
	k = 0;
	for(int x = 0; x < hsvt->height && k < rgb.size(); x++)
	{
		
		for(int y = 0; y < hsvt->width && k < rgb.size(); y++, k++)
		{
			CvScalar color = cvGet2D(hsvt, x, y);
			hsv.push_back(color);
		}
	}
	cvReleaseImage(&src);
	cvReleaseImage(&hsvt);
}


void ColorFilter::HSVToRGB(vector<CvScalar> &hsv, vector<CvScalar> &rgb)
{
	IplImage * hsvt = cvCreateImage(cvSize(100, 100), 8, 3);
	IplImage * src = cvCreateImage(cvGetSize(hsvt), 8, 3);
	int k = 0;
	for(int x = 0; x < hsvt->height && k < hsv.size(); x++)
	{
		for(int y = 0; y < hsvt->width && k < hsv.size(); y++, k++)
		{
				cvSet2D(hsvt, x, y, cvScalar(hsv[k].val[0], hsv[k].val[1], hsv[k].val[2]));			
		}
	}

	cvCvtColor(hsvt, src, CV_HSV2RGB);
	k = 0;
	rgb.clear();
	for(int x = 0; x < src->height && k < hsv.size(); x++)
	{
		for(int y = 0; y < src->width && k < hsv.size(); y++, k++)
		{
			CvScalar color = cvGet2D(src, x, y);
			rgb.push_back(color);
		}
	}
	cvReleaseImage(&hsvt);
	cvReleaseImage(&src);
}

//pL路径集, hsv:颜色集
	//功能：图片集与hsv集进行匹配，对图片集进行重新排序
void ColorFilter::match(vector<int> &pL, vector<CvScalar> &hsv, vector<CvScalar> &notHsv, exData &database)
{
	double rateH, rateS;
	//hist.buildHist(pL, database);
	hist.readFewData(pL, database);
	pcm.clear();
	rateH = 180.0 / h_bins, rateS = 255.0 / s_bins;
	
	for(int i = 0; i < hist.data.size(); i++)
	{
		PCM p;
		bool notMatch = false;
		double t;
		t = 0;
		for(int j = 0; j < hsv.size(); j++)
		{
			int hCor = abs((hsv[j].val[0] - 1) / rateH);
			int sCor = abs((hsv[j].val[1] - 1) / rateS);
			double x;
			x = cvQueryHistValue_2D(hist.data[i].hist, hCor, sCor);
			if(x <= 0)
			{
				notMatch = true;
				break;
			}
			t += x;
		}
		for(int j = 0; j < notHsv.size(); j++)
		{
			int hCor = abs((notHsv[j].val[0] - 1) / rateH);
			int sCor = abs((notHsv[j].val[1] - 1) / rateS);
			double x;
			x = cvQueryHistValue_2D(hist.data[i].hist, hCor, sCor);
			if(x > 0)
			{
				notMatch = true;
				break;
			}
		}
		if(notMatch)
			continue;
		p.loc = hist.data[i].loc;
		p.index = hist.data[i].index;
		p.matchValue = t;
		pcm.push_back(p);
	}
	sort(pcm.begin(), pcm.end(), cmp);
	pL.clear();
	for(int i = 0; i < pcm.size(); i++)
		pL.push_back(pcm[i].index);
}

ColorFilter::~ColorFilter()
{
	pcm.clear();
	hist.release();
}
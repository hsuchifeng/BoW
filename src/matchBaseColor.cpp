#include "matchBaseColor.h"


void readargument(int &n, float &lowestPre)
{
	ifstream ifile("configuration.txt");
	string t;
	getline(ifile, t);
	getline(ifile, t);
	n = atoi(t.c_str());
	getline(ifile, t);
	getline(ifile, t);
	lowestPre = atof(t.c_str());
	ifile.close();
} 

 void matchBaseColor(const char pictureName[], exData &database, vector<int> &indexResult)
{
	ColorHist match;
	CvHistogram *hist;
	int h_bins, s_bins, n;
	float lowestPre;
	string t;

	h_bins = database.hbin;
	s_bins = database.sbin;
	readargument(n, lowestPre);
	match.setBins(h_bins, s_bins);
	match.readData(database);	
	Mat image = imread(pictureName);
	match.pictureHist(pictureName, hist);
	match.match(hist, indexResult, n, lowestPre);
	indexResult.clear();
	for(int i = 0; i < match.result.size(); i++)
	{
		indexResult.push_back(match.result[i].index);
	}
	cvReleaseHist(&hist);
}

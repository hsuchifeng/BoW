#include "cFilter.h"
#include "exData.hpp"


void cFilter(vector<int> &match, vector<CvScalar> &rgb, vector<CvScalar> &notRgb, exData &database)
{
	ColorFilter cf;
	vector<CvScalar> hsv;
	vector<CvScalar> notHsv;
	int h_bins, s_bins, t;
	float d;
  //  if(rgb.empty() && notRgb.empty() )
    // return;
	h_bins = database.hbin;
	s_bins = database.sbin;
	cf.setBins(h_bins, s_bins);
	cf.RGBToHSV(rgb, hsv);
	cf.RGBToHSV(notRgb, notHsv);
	cf.match(match, hsv, notHsv, database);
}

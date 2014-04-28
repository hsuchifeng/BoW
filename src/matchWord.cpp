#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> //cvLoadImage()

#include "iutility.hpp"
#include "match.hpp"



//features type to use
#define CALCFEATS calcSurfFeats

using namespace std;
using namespace cv;

int
main(int argc, char **argv)
{
    /*check for ussage for help*/
    if(argc != 4 && argc != 5 && argc != 6 &&argc != 7 ){
        cerr << "ussage " << argv[0] << " "
             << "cluster_center_file codewords_file "
            "[idf-mode path-idf-file] image_match "
            "[imagen(default=10)]\n";
        return 1;
    }

    
    vector<resultIndex>  result;
    char *userImagePath = NULL, *codewordsPath, *clustersCenterPath;
    char *idfPath = NULL;
    bool idfMode;

    int i, n = 10; //n is the number of images to show
  
    //translate parameter
    clustersCenterPath = argv[1];
    codewordsPath = argv[2];
    if(argc == 4 || argc == 5){
        userImagePath = argv[3];
        n = 10; //default = 10;
        if(argc == 5)
            n = atoi(argv[4]);
    }
    else if(argc == 7 || argc == 6){
        idfMode = argv[3][0] == '0' ? false:true;
        idfPath = argv[4];
        userImagePath = argv[5];
        n = 10; //default = 10;
        if(argc == 7)
            n = atoi(argv[6]);
        
    }
    
    cout<<"initializing class Match...\n";
    Match m(clustersCenterPath, codewordsPath, CALCFEATS, idfPath);
    result = m.match(userImagePath);
    //show result
    {
        Mat im;
        vector<resultIndex>::iterator rit;
        im = imread(userImagePath, CV_LOAD_IMAGE_COLOR);
        imshow("image to match", im);
        cout <<"image matched:\n";
        for(rit = result.begin(), i= 0;i < n && rit != result.end();
            ++rit, ++i){
            cout<<rit->imagePath <<endl;
            im =imread(rit->imagePath.c_str(), CV_LOAD_IMAGE_COLOR);
            imshow(rit->imagePath.c_str(), im);
        }
        waitKey(0);
    }//show result end

    return 0;
}
  

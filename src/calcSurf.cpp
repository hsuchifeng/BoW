#include <iostream>
#include <fstream>
#include <vector> 
#include <ctime>

#include "iutility.hpp"

/********************
ussage:
progname index_file path_to_result
every in @index_file is the path to image to be calulated
 ********************/
using namespace std;


int
main(int argc, char **argv)
{
    //check the argument
  if(argc != 3){
      cerr << "Usage:" << argv[0] << " index_file result_path" <<endl <<
        "exery line in the index_file is path to the image\n" << endl;
      return 1;
  }

  
  //read index file
  ifstream index(argv[1]);
  if(!index){
    cerr << "cannot open index file " << argv[1] << endl;
    return 1;
  }
  char img[300], *result = argv[2];
  vector<vector<double> > feats; //store features
  int i = 0, r;
  clock_t t = clock(); //measure time used
  while(index >> img){
      cout << "processing the " << ++i << " image" << " ..." << endl;    
      r = calcSurfFeats(img, feats); //result are stored in feats
      if(r){ //if success ,it will return 0
          cerr << "calculate surf features error."
               <<img << ".calcSurffeats() return " << r << endl;
          return 1;
      }
      //export to file @result
      r = exportFeats(result, img, feats);
      if(r){
          cerr << "export features error."
               <<img << ".exportFeats() return " << r <<endl;
          return 1;
      }
  }
  
  cout << endl << "total " << i <<  " image" << endl;
  t = clock() - t;
  cout << "finished\ntime " << "used "
       << ((double)t)/ CLOCKS_PER_SEC << " seconds" <<endl;

  index.close();
  return 0;
  
}
//main end

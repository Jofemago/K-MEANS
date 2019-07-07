
#ifndef _KMEANS_HH_
#define _KMEANS_HH_
#include <iostream>
#include <vector>
using namespace std;

class Kmeans{

private:

  int k;
  int epsilon;
  int iteraciones;
  vector<double> points;
  vector<double> c;

public:

  Kmeans(){

    k = 0;
    epsilon = 0;
    iteraciones = 0;
    points = vector<double> ();
    c = vector<double> ();
    
  }

};


#endif


#ifndef _KMEANS_HH_
#define _KMEANS_HH_
#include <iostream>
#include <vector>
using namespace std;

class Kmeans{

private:

  int k;//quantity of cluster to create
  int epsilon;
  int iteraciones;
  int dim;//space of the problem
  vector<double> points;//all point for create groupe
  vector<double> c;//clustes, len = k

public:

  Kmeans(){

    k = 0;
    epsilon = 0;
    iteraciones = 0;
    dim = 0;
    points = vector<double> ();
    c = vector<double> ();

  }

};


#endif


#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <limits>
#include <math.h>
#include <unordered_set>

#include "Kmeans.hh"

#include <omp.h>

using namespace std;


class SKmeans {
private:

  //Kmeans p;
  Kmeans s;
  //vector<Kmeans> kmeans;
  int dim;
  int k;

size_t getPos(size_t row, size_t column) {

  assert(column < dim);
  return row * dim + column;
}

public:

  SKmeans(vector <double> &_points,  int dimension, vector<double> _c ,int _k = 4, double _epsilon = 0.00001, int it = 10000)
    : //p(_points,dimension,_c, _k, _epsilon, it),
      s(_points,dimension,_c, _k, _epsilon, it)
    {
    dim = dimension;
    k = _k;

    random_device rd;
    mt19937 gen(rd());
    int cantiteofpoints = _points.size()/dimension;
    uniform_int_distribution<> dis(0, (cantiteofpoints * 0.2) - 1);

    vector<double> primero = vector<double>();
    for(int i = 0; i < cantiteofpoints * 0.2; i++){

      int row = dis(gen);
      for(size_t j = 0; j < dimension ; j ++){

          primero.push_back(_points[getPos(row,j)]);
      }
    }



    Kmeans p(primero,dimension,_c, _k, _epsilon, it);

    p.simulation();
    vector<double> newpoints = p.getC();
    vector<int> res = s.simulation(newpoints);

    cout<< "res" << endl;
    for(int &i: res){
      cout << i << "\n";

    }

    cout << endl;
    /*
    kmeans = vector<Kmeans> ();
    Kmeans p(_points,dim,_c, _k, _epsilon, it);
    Kmeans s(_points,dim,_c, _k, _epsilon, it);
    kmeans.push_back(p);
    kmeans.push_back(s);*/
  }


};

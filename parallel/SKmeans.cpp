
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
  vector <double> points;
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
    points = _points;

    vector<double> primero = dataFrameInitial();
    Kmeans p(primero,dimension,_c, _k, _epsilon, it);
    p.simulation();
    vector<double> c = p.getC();
    s.chargeTheCluster(c);
  }

  vector<int> simulation(){

    return s.simulation();
  }


  double silhouette(int i, vector<int> &groups){
    return s.silhouette(i, groups);
  }

  double intengrity(vector<int> &groups) {
    return s.intengrity(groups);
  }


  vector<int> respuesta(){
    vector<int> res = vector <int> (s.quantiteOfPoints(), 0);
    s.calcMeansOfPoints(res);
    return res;
  }

private:

  vector<double> dataFrameInitial(){

    random_device rd;
    mt19937 gen(rd());
    int cantiteofpoints = s.quantiteOfPoints();
    uniform_int_distribution<> dis(0, (cantiteofpoints * 0.2) - 1);

    vector<double> res = vector<double>();
    for(int i = 0; i < cantiteofpoints * 0.2; i++){

      int row = dis(gen);
      for(size_t j = 0; j < dim ; j ++){

          res.push_back(points[getPos(row,j)]);
      }
    }
    return res;
  }

};

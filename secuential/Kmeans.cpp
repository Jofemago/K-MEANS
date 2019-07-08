
#include <iostream>
#include <vector>
#include <random>
#include <cassert>
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

  Kmeans(vector <double> _points,  int dimension, vector<double> _c ,int _k = 4, int _epsilon = 1, int it = 10000 ){
    cout << "init the kmeans" << endl;
    cout << "num of  cluster: " << k << endl;
    points = _points;
    dim = dimension;
    k = _k;
    epsilon = _epsilon;
    iteraciones = it;
    c = _c;
    //gen(rd());

  }

private:


  size_t getPos(size_t row, size_t column) {

    assert(column < dim);
    return row * dim + column;
  }

  void chargeTheCluster(){

    cout << "quantity of points: " << points.size() / dim << endl;
    if(c.empty()){
      //go to select the center of the cluster
      //to select uniforme
      random_device rd;
      mt19937 gen(rd());
      uniform_int_distribution<> dis(0, points.size()/dim - 1);

      //cout << "ome "<< this->k <<endl;
      for(size_t i = 0; i < k; i++){
        //cout << "hola"<< endl;
        int row = dis(gen);
        cout << "row to cluster: " << row << endl;
        for(size_t j = 0; j < dim ; j ++){

            c.push_back(points[getPos(row,j)]);
        }
      }

    }else{
      cout << "ya han sido cargado los clustes" << endl;
      assert(c.size() == k);
    }

  }

public:

  void simulation(){
    chargeTheCluster();
    show(c);


    return;
  }

  void show(vector<double> &x) {

    int total = x.size()/dim;
    for(int i = 0 ; i < total; i++) {
      for(int j = 0; j < dim; j++){
        cout << x[getPos(i,j)] << " ";
      }
      cout << endl;
    }
  }

};

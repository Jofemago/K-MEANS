
#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <limits>
#include <math.h>
#include <unordered_set>

using namespace std;

class Kmeans{

private:

  int k;//quantity of cluster to create
  double epsilon;
  int iteraciones;
  int dim;//space of the problem
  vector<double> points;//all point for create groupe
  vector<double> c;//clustes, len = k



public:

  Kmeans(vector <double> &_points,  int dimension, vector<double> _c ,int _k = 4, double _epsilon = 0.001, int it = 10000 ){
    cout << "init the kmeans" << endl;
    cout << "num of  cluster: " << _k << endl;
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

  int quantiteOfPoints(){
  return points.size() / dim;
  }

  double squareFunction(double a){

    return a * a;
  }

  double euclideanDistance( vector<double> &_points, vector<double> &_means, size_t point, size_t mean){
    //tengo que hacer el calculo de la distacia
    //no paralelizar
    double res = 0;
    for(size_t i = 0; i < dim ; i++) {

      res += squareFunction(_points[getPos(point, i)] - _means[getPos(mean,i)]);
    }
    return sqrt(res);
  }

  void show(vector<double> &x) {

    int total = x.size()/dim;
    for(int i = 0 ; i < total; i++) {
      for(int j = 0; j < dim; j++){
        cout << x[getPos(i,j)] << ", ";
      }
      cout << endl;
    }
  }

  void chargeTheCluster(){

    cout << "quantity of points: " << points.size() / dim << endl;
    if(c.empty()){
      //go to select the center of the cluster
      //to select uniforme
      random_device rd;
      mt19937 gen(rd());
      uniform_int_distribution<> dis(0, quantiteOfPoints() - 1);
      unordered_set<int> incluster = unordered_set<int>();
      //cout << "ome "<< this->k <<endl;
      size_t i = 0;
      while(i < k) {
        //cout << "hola"<< endl;
        int row = dis(gen);
        if(!incluster.count(row)){
          i++;
            incluster.insert(row);
            cout << "row to cluster: " << row << endl;
            for(size_t j = 0; j < dim ; j ++){

                c.push_back(points[getPos(row,j)]);
            }
          }
      }

    }else{
      cout << "ya han sido cargado los clustes" << endl;
      assert(c.size()/dim == k);
    }

  }

  void chargeTheCluster(vector<double> &_c){
    //to test the kmeans
    assert(_c.size()/dim == k);
    c = _c;

  }

public:

  void simulation(vector<double> _c){
    cout << "simulation testing"<< endl;
    chargeTheCluster(_c);
//one of the answers, means the group to which each point corresponds
    vector<int> group = vector <int> (quantiteOfPoints(), 0);

    showC();
    calcMeansOfPoints(group);

    vector<double> newc;
    newc = recalcClusters(group);
    
    //show(newc);
    cout << "simulando" << endl;
    while(distanceClusters(newc, c) > epsilon){

      c = newc;
      calcMeansOfPoints(group);
      newc = recalcClusters(group);

    }
    showC();
    
    return;
  }
  
  void pruebas(){

    cout << "Show the distance point 0 cluster 0 iteration"<<endl;
    cout << euclideanDistance(points,c,0,0);

  }

  void simulation(){

    chargeTheCluster();

    //one of the answers, means the group to which each point corresponds
    vector<int> group = vector <int> (quantiteOfPoints(), 0);

    showC();
    calcMeansOfPoints(group);

    vector<double> newc;
    newc = recalcClusters(group);
    
    //show(newc);
    cout << "simulando" << endl;
    while(distanceClusters(newc, c) > epsilon){

      c = newc;
      calcMeansOfPoints(group);
      newc = recalcClusters(group);

    }
    showC();
    
    return;
  }
private:

  double distanceClusters(vector<double> &c1, vector<double> &c2){
    double res = 0;
    for(size_t row = 0; row < k; row++){
      res += euclideanDistance(c2,c1, row, row);

    }
    cout << "Distance into clusters: " << res << endl;
    return res;
  }

  vector<double>  recalcClusters(vector<int> &group) {
  // creo que esta tambien se puede paralelizar
  //importante inicializar newc en 0 antes de llamar a esta funcion para que ppueda funcionar
    vector<double> newc = vector <double> (k * dim, 0);
    vector<int> auxprom  = vector<int> (k, 0);

    for(size_t row = 0; row < quantiteOfPoints(); row ++){

      unsigned int mean = group[row];
      for(size_t column = 0; column < dim ; column ++) {

        newc[getPos(mean, column)] += points[getPos(row, column)];
        
      }
      auxprom[mean] += 1;
    }

    //the prom of the values in the newc
    for(size_t i = 0; i < k; i++) {
      for(size_t column = 0; column < dim ; column++) {

        if(auxprom[i])
          newc[getPos(i,column)] = newc[getPos(i,column)] / auxprom[i];

      }
    }


  return newc;

  }

  void calcMeansOfPoints(vector<int> &group){

    //pienso que esta se puede paralelizar
    cout <<"calc groups of the points " << endl;
    int canpoints = quantiteOfPoints();
    for(size_t row = 0; row < canpoints; row ++) {

      double min = std::numeric_limits<double>::max();
      //cout << "min " << min << endl;
      for(int mean = 0; mean < k ; mean++){
        //cout << "cluster: " << mean<< endl;
        double dist = euclideanDistance(points, c, row, mean);
        //cout << "distancia " <<  dist << endl;
        if(dist < min){
          //cout << "cluster; " << mean << " "<< dist<< endl;
          min = dist;
          group[row] = mean;

        }
      }

    }

  }
  //cout << "Caculate the mean of the points" << group.size() << endl;

public:
  void showC(){
    cout << "show the actual cluster in the kmeans" << endl;
    show(c);
  }

  void showPoints(){

    cout << "show the points of the problem"<< endl;
    show(points);
  }






};

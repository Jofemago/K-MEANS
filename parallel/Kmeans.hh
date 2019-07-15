
#ifndef _KMEANS_HH_
#define _KMEANS_HH_
#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <limits>
#include <math.h>
#include <unordered_set>
#include <algorithm>

#include <omp.h>

using namespace std;
int threads = omp_get_max_threads();



class Kmeans{

private:

  int k;//quantity of cluster to create
  double epsilon;
  int iteraciones;
  int dim;//space of the problem
  vector<double> points;//all point for create groupe
  vector<double> c;//clustes, len = k



public:

  Kmeans(vector <double> &_points,  int dimension, vector<double> _c ,int _k = 4, double _epsilon = 0.00001, int it = 10000 ){
    //cout << "init the kmeans" << endl;
    //cout << "num of  cluster: " << _k << endl;
    points = _points;
    dim = dimension;
    k = _k;
    epsilon = _epsilon;
    iteraciones = it;
    c = _c;
    //gen(rd());

  }
  vector<double> getC(){
    return c;
  }



  int quantiteOfPoints(){
  return points.size() / dim;
  }

private:


  size_t getPos(size_t row, size_t column) {

    assert(column < dim);
    return row * dim + column;
  }


  double squareFunction(double a){

    return a * a;
  }

  double euclideanDistance( vector<double> &_points, vector<double> &_means, size_t point, size_t mean){
    //tengo que hacer el calculo de la distacia
    //no paralelizar
    double res = 0;
    //#pragma omp parallel for num_threads(threads) reduction(+:res)
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

  void chargeTheCluster(bool prueba = false){

    //cout << "quantity of points: " << points.size() / dim << endl;
    if(c.empty()){
      //go to select the center of the cluster
      //to select uniforme
      random_device rd;
      mt19937 gen;
      if(prueba) {
        gen =  mt19937(0);
      }else {
        gen= mt19937(rd());
      }
      //random_device rd;
      //mt19937 gen(rd());
      uniform_int_distribution<> dis(0, quantiteOfPoints() - 1);
      //unordered_set<int> incluster = unordered_set<int>();
      //cout << "ome "<< this->k <<endl;
      //size_t i = 0;
      for(size_t i = 0; i < k; i++) {
        //cout << "hola"<< endl;
        int row = dis(gen);

        //i++;
        //incluster.insert(row);
        //here selection the cluster initial
        //cout << "row to cluster: " << row << endl;
        for(size_t j = 0; j < dim ; j ++){

            c.push_back(points[getPos(row,j)]);
        }

      }

    }else{
      //cout << "ya han sido cargado los clustes" << endl;
      assert(c.size()/dim == k);
    }

  }



public:
  void chargeTheCluster(vector<double> &_c){
    //to test the kmeans
    assert(_c.size()/dim == k);
    c = _c;

  }

  vector<int> simulation(vector<double> _c){
    //cout << "simulation testing"<< endl;
    chargeTheCluster(_c);
//one of the answers, means the group to which each point corresponds
    vector<int> group = vector <int> (quantiteOfPoints(), 0);

    //showC();
    calcMeansOfPoints(group);

    vector<double> newc;
    newc = recalcClusters(group);

    //show(newc);
    //cout << "simulando" << endl;
    while(distanceClusters(newc, c) > epsilon  && iteraciones > 0){

      c = newc;
      calcMeansOfPoints(group);
      newc = recalcClusters(group);
      iteraciones--;
    }
    //showC();

    return group;
  }

  void pruebas(){

    cout << "Show the distance point 0 cluster 0 iteration"<<endl;
    cout << euclideanDistance(points,c,0,0);

  }

  vector<int> simulation(){

    chargeTheCluster(false);

    //one of the answers, means the group to which each point corresponds
    vector<int> group = vector <int> (quantiteOfPoints(), 0);

    //showC();
    calcMeansOfPoints(group);// calcula a que grupo pertenece cada punto

    vector<double> newc;
    newc = recalcClusters(group); //saca prommedio de puntos que pertenecen a cluster para mover los cluster

    //show(newc);
    //cout << "simulando" << endl;
    while(distanceClusters(newc, c) > epsilon  && iteraciones > 0){

      c = newc;
      calcMeansOfPoints(group);
      newc = recalcClusters(group);
      iteraciones--;
    }
    //showC();

    /*
    cout << "grupos" << endl;
    for (int &i: group){
      cout << i << " ";
    }
    cout << endl;*/
    return group;
  }
private:

  double distanceClusters(vector<double> &c1, vector<double> &c2){
    double res = 0;
    //#pragma omp paralel for num_threads(threads) reduction(+:res)
    for(size_t row = 0; row < k; row++){
      res += euclideanDistance(c2,c1, row, row);

    }
    //cout << "Distance into clusters: " << res << endl;
    return res;
  }

  vector<double>  recalcClusters(vector<int> &group) {
  // creo que esta tambien se puede paralelizar
  //importante inicializar newc en 0 antes de llamar a esta funcion para que ppueda funcionar

    unsigned int cantpoint = quantiteOfPoints();
    vector<int> auxprom(k, 0);
    /*#pragma omp declare reduction(vec_double_plus : std::vector<double> : \
                            std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), std::plus<double>())) \
                  initializer(omp_priv = omp_orig)*/
    vector<double> newc(k * dim, 0);

    /*#pragma omp declare reduction(vec_int_plus : std::vector<int> : \
                              std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), std::plus<int>())) \
                    initializer(omp_priv = omp_orig)*/



    //#pragma omp parallel for num_threads(threads) reduction(vec_double_plus:newc)// reduction(vec_int_plus:auxprom)
    for(size_t row = 0; row < cantpoint; row ++){

      unsigned int mean = group[row];


      for(size_t column = 0; column < dim ; column ++) {

        newc[getPos(mean, column)] += points[getPos(row, column)];

      }
      auxprom[mean] += 1;
    }

/*
    vector<int> auxprom(k, 0);
    for(size_t row = 0; row < cantpoint; row ++){
      unsigned int mean = group[row];
      auxprom[mean] += 1;

    }
*/


    //#pragma omp parallel for
    for(size_t i = 0; i < k; i++) {
      for(size_t column = 0; column < dim ; column++) {

        if(auxprom[i])
          newc[getPos(i,column)] = newc[getPos(i,column)] / auxprom[i];

      }
    }


  return newc;

  }

public:
  void calcMeansOfPoints(vector<int> &group){
    /*this function calc the ubication of the point in the clusters*/

    //pienso que esta se puede paralelizar
    //cout <<"calc groups of the points " << endl;
    int canpoints = quantiteOfPoints();


    //int p = omp_get_num_procs();
    //#pragma omp paralel for num_threads(p) schedule(dynamic,10)
    #pragma omp parallel for num_threads(threads)  //schedule(static, canpoints/threads)
    for(size_t row = 0; row < canpoints; row ++) {

      //cout << to_string(threads) +  " . " +  to_string(omp_get_thread_num()) + "\n";
      double min = std::numeric_limits<double>::max();
      //cout << "min " << min << endl;
      //#pragma omp paralel for num_threads(threads)
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

private:

  double cantElementGroup(int group, vector<int> &groups){
    double res = 0;
    for(int &i:groups ){
      if(i == group){
        res+=1.0;
      }
    }
    return res;
  }
  //calc off the silhouette
  //{1,2,1,1,2}
  double a(int i, vector<int> &groups){

    int grupo = groups[i];
    double catElementG = cantElementGroup(grupo, groups);
    //cout << "cantidad elementos grupo " << grupo << " es: "<< catElementG << endl;
    double cc= 0.0;
    if(catElementG > 1){
      cc = 1.0 / ( catElementG - 1 );
    }else{
      cc = 1.0;
    }
    //cout << "cc " << cc << endl;

    double res = 0;
    //paralelizar
    //#pragma omp parallel for num_threads(threads) reduction(+:res) schedule(dynamic)
    for(size_t j = 0; j < groups.size(); j++){
      if(j != i){
        if(groups[j] == grupo){
          res += euclideanDistance(points,points,i,j);
        }
      }
    }
    return cc * res;
  }

  double b(int i, vector<int> &groups) {

    double min = std::numeric_limits<double>::max();
    //int resgroup = 0;
    double aux = 0;
    //#pragma omp parallel for num_threads(threads) reduction(+:aux) //reduction(/:aux)
    for(size_t g = 0; g < k; g++){
      double catElemGroup = cantElementGroup(g, groups);
      if(groups[i] != g && catElemGroup > 0) {//no puede ser el mismo grupo porque es la disimilitud
        aux =  0;

        //paralelizar
        //#pragma omp parallel for num_threads(threads) reduction(+:aux)
        for(size_t j = 0; j < groups.size(); j++){
          if(groups[j] == g){
            aux += euclideanDistance(points,points,i,j);

          }
        }
        //#pragma omp atomic
        aux = aux/catElemGroup;
        if(aux < min){
          //#pragma omp atomic
          min = aux;
          //resgroup = g;
        }
      }
    }
    return min;
  }

public:

  double silhouette(int i, vector<int> &groups){

    double res = 0.0;
    double A = a(i,groups);
    double B = b(i,groups);

    if(A < B){
      res = 1 - (A/B);
    }else if (A < B){
      res = (B/A) - 1;
    }
    return res;
  }

  double intengrity(vector<int> &groups){

    double res = 0.0;
    int qp = quantiteOfPoints();
    #pragma omp parallel for num_threads(threads) reduction(+:res) schedule(dynamic)
    for(size_t i =  0; i < qp; i++ ) {
      res += silhouette(i, groups);
    }
  }
};
#endif

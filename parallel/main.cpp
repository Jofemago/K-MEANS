#include "Kmeans.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

using namespace std;


void showMatrix(vector<double> &x , int dim){

  int total = x.size()/dim;
  for(int i = 0 ; i < total; i++) {
    for(int j = 0; j < dim; j++){
      cout << x[i * dim + j] << " ";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  /*
    arguments

    1. name of file
    2. dim of the point
    3. Cantite of epsilon

  */
  //cout << "cantidad de argumentos: "<< argc << endl;
  //Kmeans a();

  vector<double> points = vector<double>();

  //load the args
  string nameFile = argv[1];
  int dim =  stoi(argv[2]);
  int k = stoi(argv[3]);
  //string nameFile = "./../data/iris.csv"
  //cout << "name of file points: "<<nameFile << endl;
  //cout << "dimension of the points: " << dim  <<endl;


  //load the file
  string linea;
  ifstream fileofpoints(nameFile);

  //travel line to line in the file
  if( fileofpoints.is_open()) {

    while( getline( fileofpoints, linea)){
      //cout << linea << endl;

      int n = 0;
      string token;
      string delimiter = ",";
      size_t pos = 0;
      while ((pos = linea.find(delimiter)) != std::string::npos) {

        token = linea.substr(0, pos);
        //cout << stod(token) << " ";
        points.push_back(stod(token));
        linea.erase(0, pos + delimiter.length());
        n++;

        if(n >= dim)
          break;

      }
      //cout << endl;
    }

    //fileofpoints.close();
  }
    vector <double> c = {6.5, 3, 5.8,2.2 ,
                        4.8 ,3.4 ,1.9, 0.2,
                        5, 3.3, 1.4, 0.2,
                        //6.7, 3.3 ,5.7, 2.1
                        };

    vector <double> c2 = {5.7, 2.9 ,4.2 ,1.3 ,
                          5.5 ,4.2 ,1.4, 0.2 ,
                          5.5, 4.2, 1.4, 0.2 };

  // show the results
  //showMatrix(points, dim);
  Kmeans a(points, dim, vector<double> (), k);
  vector<int> res = a.simulation();

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  //double silhouette = a.silhouette(500 ,res);
  //cout << silhouette  << endl;

  //double integridad = a.intengrity(res);
  //cout << "valor de la integridad: " << integridad << endl;

  double ssd = a.calcSSD();
  cout << "calculo del ssd: " << ssd << endl;
  //cout << ssd << "\n";
  end = std::chrono::system_clock::now();
 double time = std::chrono::duration_cast<std::chrono::milliseconds>
                          (end-start).count();

  //cout << "time: "<<time << "," << endl;
  //cout <<time << "," ;

/*
  cout<< "res" << endl;
  for(int &i: res){
    cout << i << " ";

  }
  cout << endl;
*/
/*
  for(size_t i = 1 ; i <= 3000 ; i+=25){
    Kmeans a(points, dim, vector<double> (), i);
    a.simulation();
    cout << a.calcSSD() << endl;

  }
*/

  //a.simulation(c);
  //a.showC();

  //a.showPoints();
  //a.pruebas();

  return 0;
}

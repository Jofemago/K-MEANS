#include "Kmeans.cpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>

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
  //string nameFile = "./../data/iris.csv"
  cout << "name of file points: "<<nameFile << endl;
  cout << "dimension of the points: " << dim  <<endl;


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

  // show the results
  //showMatrix(points, dim);
  Kmeans a(points, dim, vector<double> (), 4);
  a.simulation();
  //a.showMatrix();

  return 0;
}

#include "Kmeans.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <zmqpp/zmqpp.hpp>



using namespace std;
using namespace zmqpp;



void worker(string ip, vector<double> &points, int dim){

  context ctx;
  socket dealer(ctx, socket_type::dealer);

  cout << "conectando al broker\n";
  dealer.connect("tcp://" + ip + ":5000");


  while(true) {
    cout << "enviando que ando libre"<< endl;
    message m;

    m << "serverFREE";
    dealer.send(m);

    cout << " esperando respuesta para trabajar" << endl;
    message response;
    dealer.receive(response);
    string operation,k;
    cout << "sacando del mensaje \n";
    response >> operation;
    response >> k;//es un string se debe convertir a entero cuidado!!
    //response >> probar;

    if(operation == "KMEANS") {
      cout<< "K que el servidor me pide resolver: "<< k << endl;//" y " << probar << endl;

      string op = "WorkDone";
      double ssd = 0;

      //enviar k y ssd


      Kmeans a(points, dim, vector<double> (),stoi(k));

      a.simulation();
      ssd = a.calcSSD();

      cout << "el ssd es: " << ssd << endl;

      message responseSSD;
      responseSSD << op;
      responseSSD << k;
      responseSSD << to_string(ssd);
      dealer.send(responseSSD);

    } else if(operation == "WAIT"){
      cout <<"no hay trabajo, se vuelve a pedir hasta que me den o me digan que termine"<< endl;
      continue;
    } else {
      break;
    }
  }
  cout << "el servidor ha terminado por orden de broker, se termina el programa" << endl;

}


void chargeTheCluster(string nameFile, int dim, vector<double> &points) {

  //vector<double> points = vector<double>();
  cout << "cantidad de elementos otroooo : "<< points.size() /dim <<endl;
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
  cout << "cantidad de elementos otroooo 2: "<< points.size() /dim <<endl;
  //return points;

}

int main(int argc, char *argv[]){


  string nameFile = argv[1];
  int dim =  stoi(argv[2]);
  //int k = stoi(argv[3]);
  string ip = argv[3];
  //solicitar la ip

  vector<double> points =vector<double> ();
  chargeTheCluster(nameFile, dim, points);
  cout << "dataset: " << nameFile <<" Dimension to points; "<< dim <<endl;
  cout << "Broker in the ip: " << ip << endl;
  cout << "cantidad de elementos: "<< points.size() /dim <<endl;
  worker(ip, points, dim);
  return 0;
}

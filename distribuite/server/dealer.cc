#include <iostream>
#include <string>
#include <iostream>
#include <zmqpp/zmqpp.hpp>



using namespace std;
using namespace zmqpp;



void worked(){

  context ctx;
  socket dealer(ctx, socket_type::dealer);

  cout << "conectando al broker\n";
  dealer.connect("tcp://127.0.0.1:5000");


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
      int ssd = 0;

      //enviar k y ssd
      message responseSSD;
      responseSSD << op;
      responseSSD << k;
      responseSSD << to_string(ssd);

      dealer.send(responseSSD);
      //cout << response << endl;

      //operar kmeans para obener el k
      //valirdar que un grupo no qude vacio
      //obtener el SSD
    }else {
      break;
    }
  }
  cout << "el servidor ha terminado por orden de broker, se termina el programa" << endl;

}

int main(){
  worked();
  return 0;
}

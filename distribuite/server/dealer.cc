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

    m << "IFREE";
    dealer.send(m);

    cout << " esperando respuesta para trabajar" << endl;
    message response;
    dealer.receive(response);
    string k,probar;
    cout << "sacando del mensaje \n";
    response >> k;
    response >> probar;
    cout<< "la respuesta fue: "<< k << " y " << probar << endl;
    //cout << response << endl;
    int j;
    cin >> j;


  }
  //para recibir
  /*
   message response;
   dealer.receive(response);
   cout << response << endl;
   */

}

int main(){
  worked();
  return 0;
}

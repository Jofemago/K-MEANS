#include <iostream>
#include <string>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main(int argc, char** argv) {
	if(argc != 2) {
		cerr << "Wrong number of arguments\n"
		     << "./client name\n";
		return 1;
	}

	string clientName(argv[1]);
	cout << "Echo client (" << clientName << ")" << endl; ;

	context ctx;
	socket s(ctx, socket_type::req);

	cout << "Connecting to tcp port 5555\n";
	s.connect("tcp://localhost:5555");

	message m;
	string text("Hello world server, I am " + clientName);
	m <<  text;
	s.send(m);

	message response;
	s.receive(response);
	string responseText;
  int a = 0;
	response >> responseText;
  response  >> a;

	cout << "Server answer: " << responseText << a << endl;
	return 0;
}

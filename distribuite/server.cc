#include <iostream>
#include <string>
#include <iostream>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main() {
	cout << "This is the server\n";

	context ctx;
	socket s(ctx, socket_type::rep);

	cout << "Binding socket to tcp port 5555\n";
	s.bind("tcp://*:5555");

	while(true) {
		message m;
		s.receive(m);

		string text;
		m >> text;
		cout << "Received " << text << endl;

		message response;
    int n = 42;
		response << text << n;
		s.send(response);
	}
    cout << "Finished\n";
	return 0;
}

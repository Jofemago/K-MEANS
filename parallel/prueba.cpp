
#include <iostream>
#include <random>
#include <unordered_set>
using namespace std;


int main(){
  random_device rd;
  //mt19937 gen(rd());
  mt19937 gen(10);
  uniform_int_distribution<> dis(0, 200);
  for(int i = 0; i < 10; i++){
    cout << dis(gen) << endl;
  }

    return 0;
}

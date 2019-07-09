
#include <iostream>
#include <unordered_set>
using namespace std;


int main(){
    unordered_set<int> k = unordered_set<int>();
    k.insert(1);
    k.insert(1);

    cout << k.size() << endl;

    if (k.count(5)){
        cout << "verdadera"<< endl;
    }else{
        cout << "falsa" << endl;
    }

    return 0;
}
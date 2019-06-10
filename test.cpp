#include<iostream>
#include<vector>
#include<string>
using namespace std;

int main() {
    vector<int>a = {0,1,2,3};
    a.insert(a.begin() + 1, 20000);
    for (auto x : a) {
        cout << x << endl;
    }
    return 0;
}

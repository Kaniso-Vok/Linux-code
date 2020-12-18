#include <iostream>
using namespace std;

union A{
    int a;
    char b;
};

int main(){
    A b;
    b.a=1;
    if(b.b==1){
        cout<<"small"<<endl;
        return 0;
    }
    cout<<"big"<<endl;
    return 0;
}


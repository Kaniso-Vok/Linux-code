#include<iostream>
using namespace std;
int main() {
  int n;
  while(cin>>n){
    if(1&n){
      cout<<n*(n-1)*(n-2)<<endl;
    }
    else if(n%3==0){
      cout<<(n-1)*(n-2)*(n-3)<<endl;
    }
    else{
      cout<<n*(n-1)*(n-3)<<endl;
    }
  }
  return 0;
}

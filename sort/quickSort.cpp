#include<iostream>
using namespace std;
void quickSort(int *arr,int left,int right){
  if(left<right){
    int key=arr[left];
    int low=left,high=right;
    while(low<high){
      while(key<=arr[high]&&low<right){
        high--;
      }
      arr[low]=arr[high];
      while(key>=arr[low]&&low<high){
        low++;
      }
      arr[high]=arr[low];
    }
    arr[low]=key;
    quickSort(arr,left,low-1);
    quickSort(arr,low+1,right);
  }
}
int main() {
    int a[]={44,12,4,35,22,553,3,4,1,2};
    quickSort(a,0,sizeof(a)/sizeof(a[0])-1);
    for(int i=0;i<sizeof(a)/sizeof(a[0]);++i){
      cout<<a[i]<<' ';
    }
    return 0;
}

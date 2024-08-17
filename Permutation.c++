#include <bits/stdc++.h>
using namespace std;

int factorial(int n){
    int f=1;
    while(n>0){
        f = f * n;
        n--;
    }
    return f;
}
//"Finding the Permutation."
int main(){
    int n,r;
    cout<<"Enter the value of n : ";
    cin>>n;
    cout<<"Enter the value of r : ";
    cin>>r;
    if(n<r){
        cout<<"Permutation cannot be found."<<endl;
    }
    else{
        int permutation = (factorial(n))/(factorial(n-r));
        cout<<"The permutation for entered n & r is: "<<permutation<<endl; 
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int prime(int x){
    int c=0;
    int i=2;
    while(i<x){
        if(x%i==0){
            c++;
        }
        i++;
    }
    return c;
}

int main(){
    int n1,n2,k;
    cout<<"Enter the first number: ";
    cin>>n1;
    cout<<"Enter the second number: ";
    cin>>n2;
    if(n1-n2>0){
        k = n1-n2;
    }
    else if(n1-n2<0){
        k = n2-n1;
    }
    if(prime(n1)==0 and prime(n2)==0 and k==2){
        cout<<"The two numbers are prime."<<endl;
    }
    else{
        cout<<"The two numbers are not prime."<<endl;
    }

}
//Only works for POSITIVE NUMBERS.

#include <bits/stdc++.h>
using namespace std;

int isPalindrome(int num) {
    int originalNum = num;
    int reversedNum = 0;
    while (num > 0) {
        int digit = num % 10;
        reversedNum = reversedNum * 10 + digit;
        num /= 10;
    }
    if(originalNum==reversedNum){
        return 1;
    }
    else{
        return 0;
    }
}

int main() {
    int n;
    cout << "Enter the number: ";
    cin >> n;
    if(n<0){
        cout<<"PALINDROME CAN'T BE CHECKRD FOR NEGATIVE NUMBERS."<<endl;
    }
    if (isPalindrome(n)==1) {
        cout << "PALINDROME NUMBER." << endl;
    } 
    else {
        cout << "NOT A PALINDROME NUMBER." << endl;
    }
    return 0;
}

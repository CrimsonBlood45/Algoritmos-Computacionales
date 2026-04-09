#include <iostream>

using namespace std;

int main(){
  int num, res = 1;
  cout << "Valor Para Factorial: \n > ";
  cin >> num;
  for(int i = 1; i <= num; i++){
    res *= i;
  }
  cout << "El factorial de (" << num << ") es: " << res << endl;
}
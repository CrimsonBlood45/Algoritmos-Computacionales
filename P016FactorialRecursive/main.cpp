#include <iostream>

using namespace std;

int factorial(int num);

int main(){
  int num;
  cout << "Valor Para Factorial: \n > ";
  cin >> num;
  cout << "El factorial de (" << num << ") es: " << factorial(num) << endl;
}

int factorial(int num){
  return num == 1 ? 1 : num*(factorial(num -1));
}
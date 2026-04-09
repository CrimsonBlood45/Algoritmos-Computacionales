#include <iostream>

using namespace std;

long long sumaNaturales(int n);

int main(){
    setlocale(LC_ALL, "");

    int n;
    cout << "Ingrese el valor de n (número de términos a sumar): ";
    cin >> n;

    if (n <= 0) {
        cout << "El número debe ser mayor que cero." << endl;
        return 1; // Finaliza el programa con error
    }

    long long resultado = sumaNaturales(n);

    cout << "La suma de los primeros " << n << " números naturales es: " << resultado << endl;
}
long long sumaNaturales(int n){
    return n == 1 ? 1 : n + sumaNaturales(n - 1);
}
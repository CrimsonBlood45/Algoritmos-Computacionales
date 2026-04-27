#include <iostream>
using namespace std;

int main() {
int valor = 10;
int *p = &valor; // Declaración e inicialización: p apunta a la dirección de 'valor'

cout << "Valor inicial de variable 'valor': " << valor << endl;
cout << "Direccion de memoria de 'valor': " << &valor << endl;
cout << "Direccion almacenada en puntero p: " << p << endl;
cout << "Valor al que apunta p (desreferenciando): " << *p << endl;

// Modificar el valor a través del apuntador
*p = 20;
cout << "Nuevo valor de 'valor' tras modificar via *p: " << valor << endl;

return 0;
}
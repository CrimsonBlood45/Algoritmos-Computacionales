#include <iostream>
using namespace std;
int main() {
    int arreglo[5] = {1, 2, 3, 4, 5};
    int *p = arreglo; // Apunta al primer elemento
    int i;
    int suma = 0;
    cout << "Valores del arrego ";
    for (i = 0; i < 5; i++){
        cout << arreglo [i] << " ";
    }
    // Recorrido con apuntadores
    cout << endl;
    for(i = 0; i < 5; i++) {
        cout << "Valor original: " << *(p + i) << endl;
        // Modificar valor (multiplicar por 2)

        *(p + i) = *(p + i) * 2;
        cout << "Valor modificado: " << *(p + i) << endl;
        suma += *(p + i);
    }
    cout << "Suma total: " << suma << endl;
    return 0;
}
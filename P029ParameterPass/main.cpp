#include <iostream>
using namespace std;
void intercambiar(int *a, int *b) {
    // Intercambia los valores apuntados por a y b
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 5, y = 8;
    cout << "Antes de intercambiar: x=" << x << ", y=" << y << endl;
    intercambiar(&x, &y); // Pasamos las direcciones de x e y a la función
    cout << "Despues de intercambiar: x=" << x << ", y=" << y << endl;
}
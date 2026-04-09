#include <iostream>
using namespace std;

void torresDeHanoi(int n, char origen, char destino,char auxiliar);

int main(){
    int n;
    cout << "Ingrese el número de discos: ";
    cin >> n;
    torresDeHanoi(n, 'A', 'C', 'B');
}

void torresDeHanoi(int n, char origen, char destino,char auxiliar){
    if (n == 1) {
        cout << "Mover disco 1 de " << origen << " a " <<
        destino << endl;
        return;
    }
    torresDeHanoi(n - 1, origen, auxiliar, destino);
    cout << "Mover disco " << n << " de " << origen << " a " << destino << endl;
    torresDeHanoi(n - 1, auxiliar, destino, origen);
}
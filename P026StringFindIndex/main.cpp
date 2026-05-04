#include <iostream>
using namespace std;

// Funci�n para buscar un texto dentro de otro usando '\0' como fin de cadena
int buscarTexto(const char* texto, const char* subtexto) {
    int i = 0, j = 0;
    
    while (texto[i] != '\0') {
        // Compara subtexto car�cter por car�cter
        while (texto[i + j] != '\0' && subtexto[j] != '\0' && texto[i + j] == subtexto[j]) {
            j++;
        }
        // Si lleg� al final del subtexto, encontr� la coincidencia
        if (subtexto[j] == '\0') {
            return i; 
        }
        j = 0;
        i++;
    }
    return -1; // No encontrado
}

int main() {
    char texto[100], subtexto[50];
    cout << "Ingrese el texto principal: ";
    cin.getline(texto, 100);
    cout << "Ingrese el subtexto a buscar: ";
    cin.getline(subtexto, 50);

    int posicion = buscarTexto(texto, subtexto);

    if (posicion != -1) {
        cout << "El subtexto se encontro en la posicion " << posicion << " del texto principal." << endl;
    } else {
        cout << "El subtexto no se encontro en el texto principal." << endl;
    }
    return 0;
}
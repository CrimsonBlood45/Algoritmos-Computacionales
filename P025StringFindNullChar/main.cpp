#include <iostream>

using namespace std;

int buscarTexto(const char* texto, const char* subtexto) {
    int i = 0, j = 0;
    // Ciclo principal para recorrer el texto
    while (texto[i] != '\0') {
        // Si se encuentra una coincidencia inicial, comparar subtexto completo
        while (texto[i + j] != '\0' && subtexto[j] != '\0' && texto[i + j] ==
            subtexto[j]) {
            j++;
        }
        // Si se alcanzó el fin del subtexto, es una coincidencia completa
        if (subtexto[j] == '\0') {
            return i; // Retorna la posición de inicio de la coincidencia
        }
        // Reiniciar j y avanzar al siguiente carácter
        j = 0;
        i++;
    }
    // Si no se encontró el subtexto, retornar -1
    return -1;
}
int main() {
    // Entradas del usuario
    char texto[100], subtexto[50];

    cout << "Ingrese el texto principal: ";
    cin.getline(texto, 100);

    cout << "Ingrese el subtexto a buscar: ";
    cin.getline(subtexto, 50);
    // Llamada a la función de búsqueda
    int posicion = buscarTexto(texto, subtexto);
    // Resultado
    if (posicion != -1) {
        cout << "El subtexto se encontró en la posición " <<
        posicion << " del texto principal." << endl;
    } 
    else{
        cout << "El subtexto no se encontró en el texto principal." << endl;
    }

    return 0;
}
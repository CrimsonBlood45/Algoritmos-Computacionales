#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <list> // Para manejar colisiones con listas enlazadas

using namespace std;

// Estructura para representar un estudiante
struct Estudiante {
    string boleta;
    string nombre;
};

// Clase para implementar la tabla hash
class TablaHash {
private:
    static const int tamanoTabla = 100; // Tamaño de la tabla hash
    vector<list<Estudiante>> tabla;     // Tabla hash implementada con encadenamiento

    // Función de hash para calcular el índice
    int funcionHash(const string& boleta) {
        int suma = 0;

        for (char c : boleta) {
            suma += c; // Suma los valores ASCII de los caracteres de la boleta
        }

        return suma % tamanoTabla; // Retorna el índice dentro del rango
    }

public:
    // Constructor de la clase
    TablaHash() : tabla(tamanoTabla) {}

    // Función para agregar un estudiante a la tabla hash
    void agregarEstudiante(const string& boleta, const string& nombre) {
        int indice = funcionHash(boleta);

        // Verificar si ya existe un estudiante con la misma boleta
        for (const auto& estudiante : tabla[indice]) {
            if (estudiante.boleta == boleta) {
                cout << "Error: El estudiante con boleta "
                     << boleta << " ya existe." << endl;
                return;
            }
        }

        // Agregar el estudiante al índice correspondiente
        tabla[indice].push_back({boleta, nombre});

        cout << "Estudiante agregado: Boleta = "
             << boleta
             << ", Nombre = "
             << nombre
             << endl;
    }

    // Función para buscar un estudiante por su boleta
    void buscarEstudiante(const string& boleta) {
        int indice = funcionHash(boleta);

        // Buscar en la lista enlazada del índice correspondiente
        for (const auto& estudiante : tabla[indice]) {
            if (estudiante.boleta == boleta) {
                cout << "Estudiante encontrado:" << endl;
                cout << "Boleta: "
                     << estudiante.boleta
                     << ", Nombre: "
                     << estudiante.nombre
                     << endl;
                return;
            }
        }

        cout << "Estudiante con boleta "
             << boleta
             << " no encontrado."
             << endl;
    }

    // Función para mostrar todos los estudiantes en la tabla hash
    void mostrarTabla() {
        for (int i = 0; i < tamanoTabla; ++i) {
            if (!tabla[i].empty()) {
                cout << "Índice " << i << ":" << endl;

                for (const auto& estudiante : tabla[i]) {
                    cout << "  Boleta: "
                         << estudiante.boleta
                         << ", Nombre: "
                         << estudiante.nombre
                         << endl;
                }
            }
        }
    }

    // Función para leer datos desde un archivo
    void leerArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo "
                 << nombreArchivo
                 << endl;
            return;
        }

        string linea;

        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string boleta, nombre;

            if (getline(ss, boleta, ',') && getline(ss, nombre)) {
                agregarEstudiante(boleta, nombre);
            } else {
                cout << "Error al procesar la línea: "
                     << linea
                     << endl;
            }
        }

        archivo.close();

        cout << "Datos leídos correctamente desde el archivo."
             << endl;
    }
};

int main() {
    // Establecer el locale para aceptar caracteres con acentos y la ñ
    setlocale(LC_ALL, "");

    TablaHash tabla;
    int opcion;

    do {
        // Mostrar menú de opciones
        cout << "\nMenú:" << endl;
        cout << "1. Leer archivo" << endl;
        cout << "2. Mostrar tabla" << endl;
        cout << "3. Buscar estudiante" << endl;
        cout << "4. Finalizar" << endl;
        cout << "Seleccione una opción: ";

        cin >> opcion;

        switch (opcion) {
            case 1: {
                // Leer datos desde un archivo
                string nombreArchivo;

                cout << "Ingrese el nombre del archivo (sin extensión): ";
                cin >> nombreArchivo;

                nombreArchivo += ".txt";
                tabla.leerArchivo(nombreArchivo);

                break;
            }

            case 2:
                // Mostrar la tabla hash
                tabla.mostrarTabla();
                break;

            case 3: {
                // Buscar un estudiante
                string boletaBusqueda;

                cout << "Ingrese la boleta del estudiante a buscar: ";
                cin >> boletaBusqueda;

                tabla.buscarEstudiante(boletaBusqueda);
                break;
            }

            case 4:
                // Finalizar el programa
                cout << "Programa finalizado." << endl;
                break;

            default:
                cout << "Opción no válida. Intente nuevamente" << endl;
        }

    } while (opcion != 4);

    return 0;
}
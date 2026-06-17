// Autores: Edna Martha Miranda Chávez
// Sergio Fuenlabrada Velázquez
// Mejorado

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <list>
#include <iomanip>

using namespace std;

struct Estudiante {
    string boleta;
    string nombre;
};

bool archivoLeido = false;

const int tamanoTabla = 100;

vector<list<Estudiante>> tabla(tamanoTabla);

int totalEstudiantes = 0;
int colisiones = 0;

int funcionHash(const string& clave) {
    unsigned long hash = 5381;

    for (char c : clave) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % tamanoTabla;
}

void agregarEstudiante(const string& boleta, const string& nombre) {
    int indice = funcionHash(boleta);

    for (const auto& est : tabla[indice]) {
        if (est.boleta == boleta) {
            cout << "Error: El estudiante con boleta "
                 << boleta
                 << " ya existe"
                 << endl;
            return;
        }
    }

    if (!tabla[indice].empty()) {
        colisiones++;
    }

    tabla[indice].push_back({boleta, nombre});

    totalEstudiantes++;

    cout << "Estudiante agregado: Boleta = "
         << boleta
         << ", Nombre = "
         << nombre
         << endl;
}

void eliminarEstudiante(const string& boleta) {
    int indice = funcionHash(boleta);

    for (
        auto it = tabla[indice].begin();
        it != tabla[indice].end();
        ++it
    ) {
        if (it->boleta == boleta) {
            tabla[indice].erase(it);

            totalEstudiantes--;

            cout << "Estudiante con boleta "
                 << boleta
                 << " eliminado correctamente"
                 << endl;

            return;
        }
    }

    cout << "No se encontró estudiante con boleta "
         << boleta
         << endl;
}

void buscarEstudiante(const string& boleta) {
    int indice = funcionHash(boleta);

    for (const auto& estudiante : tabla[indice]) {
        if (estudiante.boleta == boleta) {
            cout << "Estudiante encontrado:\n"
                 << "Boleta: "
                 << estudiante.boleta
                 << ", Nombre: "
                 << estudiante.nombre
                 << endl;

            return;
        }
    }

    cout << "Estudiante con boleta "
         << boleta
         << " no encontrado"
         << endl;
}

void mostrarTabla() {
    for (size_t i = 0; i < tabla.size(); ++i) {
        if (!tabla[i].empty()) {
            cout << "\nÍndice " << i << ": ";

            for (const auto& estudiante : tabla[i]) {
                cout << " Boleta: "
                     << estudiante.boleta
                     << ", Nombre: "
                     << estudiante.nombre
                     << endl;
            }
        }
    }
}

void mostrarEstadisticas() {
    cout << "\n--- Estadísticas ---" << endl;

    cout << "Total de estudiantes: "
         << totalEstudiantes
         << endl;

    cout << "Total de colisiones: "
         << colisiones
         << endl;

    double factorCarga =
        static_cast<double>(totalEstudiantes) / tabla.size();

    cout << fixed << setprecision(2);

    cout << "Factor de carga: "
         << factorCarga
         << endl;
}

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

        string boleta;
        string nombre;

        if (getline(ss, boleta, ',') &&
            getline(ss, nombre)) {

            agregarEstudiante(boleta, nombre);

        } else {
            cout << "Error al procesar la línea: "
                 << linea
                 << endl;
        }
    }

    archivo.close();

    archivoLeido = true;

    cout << "Datos leídos correctamente desde el archivo"
         << endl;
}

bool salir() {
    if (!archivoLeido) {
        cout << "No se ha leido el archivo";
        return true;
    }

    return false;
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "es_MX.UTF-8");

    int opcion;

    bool datosLeidos = false;
    bool continuar = true;

    while (continuar) {
        cout << "\nMenú:\n";
        cout << "1. Leer archivo\n";
        cout << "2. Agregar estudiante\n";
        cout << "3. Eliminar estudiante\n";
        cout << "4. Buscar estudiante\n";
        cout << "5. Mostrar tabla\n";
        cout << "6. Mostrar estadísticas\n";
        cout << "7. Finalizar\n";
        cout << "Opción: ";

        cin >> opcion;
        cin.ignore();

        if (!datosLeidos &&
            opcion != 1 &&
            opcion != 7) {

            cout << "Debe leer el archivo antes de utilizar otras opciones."
                 << endl;

            continue;
        }

        switch (opcion) {
            case 1: {
                string nombreArchivo;

                cout << "Nombre del archivo (sin extensión): ";

                getline(cin, nombreArchivo);

                nombreArchivo += ".txt";

                leerArchivo(nombreArchivo);

                datosLeidos = true;

                break;
            }

            case 2: {
                string boleta;
                string nombre;

                cout << "Boleta: ";
                getline(cin, boleta);

                cout << "Nombre: ";
                getline(cin, nombre);

                agregarEstudiante(boleta, nombre);

                break;
            }

            case 3: {
                string boleta;

                cout << "Boleta a eliminar: ";
                getline(cin, boleta);

                eliminarEstudiante(boleta);

                break;
            }

            case 4: {
                string boleta;

                cout << "Boleta a buscar: ";
                getline(cin, boleta);

                buscarEstudiante(boleta);

                break;
            }

            case 5:
                mostrarTabla();
                break;

            case 6:
                mostrarEstadisticas();
                break;

            case 7:
                continuar = salir();
                break;

            default:
                cout << "Opción no válida" << endl;
        }
    }

    return 0;
}
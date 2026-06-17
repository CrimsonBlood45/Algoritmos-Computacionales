// Autores: Edna Martha Miranda Chávez
// Sergio Fuenlabrada Velázquez
//
// Programa P042 - Fuerza bruta
// Lee números enteros desde un archivo de texto, muestra una cantidad
// solicitada por el usuario y encuentra todas las parejas cuya suma
// coincide con un valor objetivo mediante búsqueda exhaustiva.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Prototipos de funciones.
void configurarConsola();
void mostrarMenu(bool datosCargados, bool busquedaRealizada);
string leerCadenaNoVacia(const string& mensaje);
bool convertirEntero(const string& texto, int& valor);
int leerEntero(const string& mensaje, int minimo, int maximo);
bool leer(vector<int>& datos, string& nombreArchivo);
void mostrar(const vector<int>& datos);
void encontrar(const vector<int>& datos);

int main() {
    configurarConsola();
    vector<int> datos;
    string nombreArchivo;
    bool datosCargados = false;
    bool busquedaRealizada = false;
    int opcion;

    cout << "===============================================\n";
    cout << " PROGRAMA DE FUERZA BRUTA\n";
    cout << "===============================================\n";

    do {
        mostrarMenu(datosCargados, busquedaRealizada);
        opcion = leerEntero("Seleccione una opción: ", 1, 4);

        // La primera operación obligatoria es leer un archivo válido.
        if (!datosCargados && opcion != 1) {
            cout << "\nPrimero debe ejecutar la opción 1. Leer.\n";
            continue;
        }

        switch (opcion) {
            case 1:
                if (datosCargados) {
                    cout << "\nError: el archivo ya fue leído anteriormente.\n";
                    cout << "La opción Leer solo puede ejecutarse una vez durante el proceso\n";
                    cout << "Archivo actualmente cargado: " << nombreArchivo << ".txt\n";
                } else {
                    if (leer(datos, nombreArchivo)) {
                        datosCargados = true;
                        busquedaRealizada = false;
                        cout << "\nArchivo cargado correctamente: " << nombreArchivo << ".txt\n";
                        cout << "Valores válidos leídos: " << datos.size() << "\n";
                    }
                }
                break;
            case 2:
                mostrar(datos);
                break;
            case 3:
                if (datos.size() < 2) {
                    cout << "\nSe requieren al menos dos valores para formar parejas\n";
                    cout << "Cargue otro archivo mediante la opción 1. Leer.\n";
                } else {
                    encontrar(datos);
                    busquedaRealizada = true;
                }
                break;
            case 4:
                if (!busquedaRealizada) {
                    cout << "\nNo puede salir todavía. Debe ejecutar la opción 3. Encontrar\n";
                    cout << "al menos una vez sobre los datos actualmente cargados\n";
                    opcion = 0;
                } else {
                    cout << "\nProceso terminado correctamente\n";
                }
                break;
        }
    } while (opcion != 4);

    return 0;
}

// Configura la consola de Windows para mostrar correctamente caracteres UTF-8.
void configurarConsola() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// Muestra el menú y el estado funcional del programa.
void mostrarMenu(bool datosCargados, bool busquedaRealizada) {
    cout << "\n---------------- MENÚ PRINCIPAL ----------------\n";
    cout << "1. Leer archivo\n";
    cout << "2. Mostrar valores\n";
    cout << "3. Encontrar parejas por fuerza bruta\n";
    cout << "4. Salir\n";
    cout << "------------------------------------------------\n";
    cout << "Archivo cargado: " << (datosCargados ? "Sí" : "No") << "\n";
    cout << "Búsqueda realizada: " << (busquedaRealizada ? "Sí" : "No") << "\n";
}

// Lee una cadena y evita entradas vacías o formadas únicamente por espacios.
string leerCadenaNoVacia(const string& mensaje) {
    string texto;
    while (true) {
        cout << mensaje;
        getline(cin, texto);
        size_t inicio = texto.find_first_not_of(" \t\r\n");
        if (inicio == string::npos) {
            cout << "Entrada inválida. No puede dejar el dato vacío\n";
            continue;
        }
        size_t fin = texto.find_last_not_of(" \t\r\n");
        return texto.substr(inicio, fin - inicio + 1);
    }
}

// Convierte una cadena completa a entero y valida rango y caracteres sobrantes.
bool convertirEntero(const string& texto, int& valor) {
    stringstream flujo(texto);
    long long temporal;
    char sobrante;
    if (!(flujo >> temporal)) {
        return false;
    }
    if (flujo >> sobrante) {
        return false;
    }
    if (temporal < INT_MIN || temporal > INT_MAX) {
        return false;
    }
    valor = static_cast<int>(temporal);
    return true;
}

// Solicita un entero dentro de un intervalo cerrado.
int leerEntero(const string& mensaje, int minimo, int maximo) {
    string entrada;
    int valor;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);
        if (!convertirEntero(entrada, valor)) {
            cout << "Entrada inválida. Escriba un número entero\n";
            continue;
        }
        if (valor < minimo || valor > maximo) {
            cout << "Valor fuera de rango. Debe estar entre " << minimo << " y " << maximo << "\n";
            continue;
        }
        return valor;
    }
}

// Solicita el nombre sin extensión, abre el archivo y valida todos sus datos.
bool leer(vector<int>& datos, string& nombreArchivo) {
    string nombreSolicitado;
    string rutaArchivo;
    ifstream archivo;
    vector<int> datosTemporales;
    string linea;
    int numeroLinea = 0;

    cout << "\n--------------- LECTURA DE ARCHIVO ---------------\n";
    nombreSolicitado = leerCadenaNoVacia("Escriba el nombre del archivo sin la extensión .txt: ");
    
    // Evita que el usuario agregue la extensión, como solicita el programa.
    if (nombreSolicitado.length() >= 4) {
        string extension = nombreSolicitado.substr(nombreSolicitado.length() - 4);
        if (extension == ".txt" || extension == ".TXT") {
            cout << "Nombre inválido. Debe escribirlo sin la extensión .txt\n";
            return false;
        }
    }
    
    rutaArchivo = nombreSolicitado + ".txt";
    archivo.open(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        cout << "No fue posible abrir el archivo '" << rutaArchivo << "'\n";
        cout << "Verifique que exista y que se encuentre en la carpeta del programa\n";
        return false;
    }

    while (getline(archivo, linea)) {
        numeroLinea++;
        // Se permiten líneas vacías; simplemente no aportan valores.
        if (linea.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }
        stringstream flujoLinea(linea);
        string token;
        while (flujoLinea >> token) {
            int valor;
            if (!convertirEntero(token, valor)) {
                cout << "Dato inválido en la línea " << numeroLinea << ": '" << token << "'\n";
                cout << "La lectura fue cancelada y se conservaron los datos anteriores\n";
                archivo.close();
                return false;
            }
            datosTemporales.push_back(valor);
        }
    }

    if (archivo.bad()) {
        cout << "Ocurrió un error durante la lectura del archivo.\n";
        archivo.close();
        return false;
    }
    archivo.close();

    if (datosTemporales.empty()) {
        cout << "El archivo no contiene números enteros válidos.\n";
        cout << "La lectura fue cancelada y se conservaron los datos anteriores.\n";
        return false;
    }

    datos = datosTemporales;
    nombreArchivo = nombreSolicitado;
    return true;
}

// Muestra la cantidad de valores solicitada por el usuario.
void mostrar(const vector<int>& datos) {
    cout << "\n---------------- MOSTRAR VALORES ----------------\n";
    int cantidad = leerEntero(
        "¿Cuántos valores desea mostrar? ", 
        1, 
        static_cast<int>(datos.size())
    );
    
    cout << "\nPrimeros " << cantidad << " valores:\n";
    for (int i = 0; i < cantidad; i++) {
        cout << "[" << i << "] = " << datos[i] << "\n";
    }
    if (cantidad < static_cast<int>(datos.size())) {
        cout << "Se omitieron " << datos.size() - cantidad << " valores del archivo.\n";
    }
}

// Examina exhaustivamente todas las parejas de posiciones diferentes.
void encontrar(const vector<int>& datos) {
    cout << "\n------------ BÚSQUEDA POR FUERZA BRUTA ------------\n";
    int objetivo = leerEntero(
        "Ingrese el valor objetivo de la suma: ", 
        INT_MIN, 
        INT_MAX
    );
    
    long long comparaciones = 0;
    long long coincidencias = 0;
    
    cout << "\nParejas encontradas:\n";
    for (size_t i = 0; i < datos.size() - 1; i++) {
        for (size_t j = i + 1; j < datos.size(); j++) {
            comparaciones++;
            // long long evita desbordamiento al sumar dos enteros extremos.
            long long suma = static_cast<long long>(datos[i]) + datos[j];
            if (suma == objetivo) {
                coincidencias++;
                cout << coincidencias << ". posiciones [" << i << "] y [" << j
                     << "]: " << datos[i] << " + " << datos[j]
                     << " = " << objetivo << "\n";
            }
        }
    }

    if (coincidencias == 0) {
        cout << "No se encontró ninguna pareja cuya suma sea " << objetivo << ".\n";
    }

    cout << "\nResumen de la búsqueda:\n";
    cout << "Valores analizados: " << datos.size() << "\n";
    cout << "Parejas evaluadas: " << comparaciones << "\n";
    cout << "Parejas encontradas: " << coincidencias << "\n";
    cout << "Complejidad temporal: O(n^2)\n";
}
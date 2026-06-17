// Autores: Edna Martha Miranda Chávez
// Sergio Fuenlabrada Velázquez
// Mejorado para buscar y remplazar

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <locale>
#include <cctype>
#include <limits>

using namespace std;

struct PalabraFrecuencia {
    string palabra;
    int frecuencia;
};

bool archivoLeido = false;

string limpiarPalabra(const string& palabra) {
    string resultado;

    for (char c : palabra) {
        if (
            isalnum(c) ||
            c == 'ñ' || c == 'Ñ' ||
            c == 'á' || c == 'é' || c == 'í' || c == 'ó' || c == 'ú' ||
            c == 'Á' || c == 'É' || c == 'Í' || c == 'Ó' || c == 'Ú'
        ) {
            resultado += tolower(c);
        }
    }

    return resultado;
}

void reconstruirTablaHash(
    const string& textoOriginal,
    unordered_map<string, int>& tablaHash
) {
    if (!archivoLeido) {
        cout << "El archivo no a sido leido\n";
        return;
    }

    tablaHash.clear();

    stringstream ss(textoOriginal);
    string palabra;

    while (ss >> palabra) {
        palabra = limpiarPalabra(palabra);

        if (!palabra.empty()) {
            tablaHash[palabra]++;
        }
    }
}

void leerArchivo(
    const string& nombreArchivo,
    string& textoOriginal,
    unordered_map<string, int>& tablaHash
) {
    ifstream archivo(nombreArchivo + ".txt");

    if (archivo.is_open()) {
        stringstream buffer;
        buffer << archivo.rdbuf();

        textoOriginal = buffer.str();

        archivo.close();

        archivoLeido = true;

        reconstruirTablaHash(textoOriginal, tablaHash);

    } else {
        cout << "No se pudo abrir el archivo." << endl;
        archivoLeido = false;
        return;
    }
}

void mostrarTexto(const string& textoOriginal) {
    if (!archivoLeido) {
        cout << "El archivo no sea a leido\n";
        return;
    }

    cout << textoOriginal << endl;
}

void mostrarTablaHash(
    const unordered_map<string, int>& tablaHash
) {
    if (!archivoLeido) {
        cout << "El archivo no a sido leido\n";
        return;
    }

    cout << "Palabra\t\tFrecuencia" << endl;

    for (const auto& par : tablaHash) {
        cout << par.first
             << "\t\t"
             << par.second
             << endl;
    }
}

void buscarPalabra(
    const unordered_map<string, int>& tablaHash,
    const string& palabra
) {
    if (!archivoLeido) {
        cout << "El archivo no a sido leido\n";
        return;
    }

    auto it = tablaHash.find(palabra);

    if (it != tablaHash.end()) {
        cout << "La palabra '"
             << palabra
             << "' aparece "
             << it->second
             << " veces."
             << endl;
    } else {
        cout << "La palabra '"
             << palabra
             << "' no se encontró."
             << endl;
    }
}

void ordenarYMostrarPorPalabra(
    unordered_map<string, int>& tablaHash
) {
    if (!archivoLeido) {
        cout << "El archivo no a sido leido\n";
        return;
    }

    vector<PalabraFrecuencia> palabras;

    for (const auto& par : tablaHash) {
        palabras.push_back({par.first, par.second});
    }

    sort(
        palabras.begin(),
        palabras.end(),
        [](const PalabraFrecuencia& a,
           const PalabraFrecuencia& b) {
            return a.palabra < b.palabra;
        }
    );

    cout << "Palabra\t\tFrecuencia" << endl;

    for (const auto& pf : palabras) {
        cout << pf.palabra
             << "\t\t"
             << pf.frecuencia
             << endl;
    }
}

void reemplazarPalabra(
    string& textoOriginal,
    unordered_map<string, int>& tablaHash
) {
    if (!archivoLeido) {
        cout << "El archivo no a sido leido\n";
        return;
    }

    cin.ignore();

    string palabraObjetivo;
    string palabraNueva;

    cout << "Palabra a buscar y evaluar para reemplazo: ";
    getline(cin, palabraObjetivo);

    palabraObjetivo = limpiarPalabra(palabraObjetivo);

    cout << "Palabra nueva para reemplazar: ";
    getline(cin, palabraNueva);

    stringstream ss(textoOriginal);
    stringstream nuevoTexto;

    string palabra;

    bool reemplazado = false;

    while (ss >> palabra) {
        string original = palabra;
        string limpia = limpiarPalabra(original);

        if (limpia == palabraObjetivo) {
            cout << "¿Deseas reemplazar \""
                 << original
                 << "\" por \""
                 << palabraNueva
                 << "\"? (s/n): ";

            char respuesta;

            cin >> respuesta;
            cin.ignore();

            if (respuesta == 's' || respuesta == 'S') {
                nuevoTexto << palabraNueva << " ";
                reemplazado = true;
                continue;
            }
        }

        nuevoTexto << original << " ";
    }

    textoOriginal = nuevoTexto.str();

    if (reemplazado) {
        cout << "Reemplazos realizados. Tabla hash será reconstruida..."
             << endl;

        reconstruirTablaHash(textoOriginal, tablaHash);
    } else {
        cout << "No se realizó ningún reemplazo."
             << endl;
    }
}

int validarOpcion() {
    int opcion;

    while (!(cin >> opcion)) {
        cin.clear();
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        cout << "Opción no válida. Ingrese un número para la opción: ";
    }

    return opcion;
}

bool salir() {
    if (!archivoLeido) {
        cout << "El archivo no a sido leido\n";
        return true;
    }

    return false;
}

void mostrarMenu() {
    cout << "\nMenú:" << endl;
    cout << "1. Leer archivo" << endl;
    cout << "2. Mostrar texto" << endl;
    cout << "3. Mostrar conteo de palabras" << endl;
    cout << "4. Buscar palabra" << endl;
    cout << "5. Ordenar alfabéticamente y mostrar tabla hash" << endl;
    cout << "6. Reemplazar palabra" << endl;
    cout << "7. Terminar" << endl;
    cout << "Seleccione una opción: ";
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "es_MX.UTF-8");

    string textoOriginal;
    unordered_map<string, int> tablaHash;

    int opcion;
    bool continuar = true;

    while (continuar) {
        mostrarMenu();

        opcion = validarOpcion();

        switch (opcion) {

            case 1: {
                if (archivoLeido) {
                    cout << "El archivo ya fue leido\n";
                    continue;
                }

                string nombreArchivo;

                cout << "Ingrese el nombre del archivo (sin extensión): ";
                cin >> nombreArchivo;

                leerArchivo(
                    nombreArchivo,
                    textoOriginal,
                    tablaHash
                );

                break;
            }

            case 2:
                mostrarTexto(textoOriginal);
                break;

            case 3:
                mostrarTablaHash(tablaHash);
                break;

            case 4: {
                if (!archivoLeido) {
                    cout << "El archivo no a sido leido\n";
                    continue;
                }

                string palabra;

                cout << "Ingrese la palabra a buscar: ";
                cin >> palabra;

                palabra = limpiarPalabra(palabra);

                buscarPalabra(tablaHash, palabra);

                break;
            }

            case 5:
                ordenarYMostrarPorPalabra(tablaHash);
                break;

            case 6:
                reemplazarPalabra(
                    textoOriginal,
                    tablaHash
                );
                break;

            case 7:
                continuar = salir();
                break;

            default:
                cout << "Opción no válida. Intente nuevamente."
                     << endl;
        }

    } while (opcion != 7);
    return 0;
}
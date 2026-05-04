#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <cmath>

using namespace std;

#define readErr       "\t[!]Error: Fallo al leer. El archivo ya ha sido leído\n"
#define sortErr       "\t[!]Error: Fallo al ordenar. El archivo ya ha sido ordenado\n"
#define sortReadErr   "\t[!]Error: Fallo al ordenar. No se ha leido ningún archivo\n"
#define sortRecErr    "\t[!]Error: Fallo al ordenar. El archivo ya fue grabado\n"
#define recordWarn    "\t[!]Advertencia: Se sobreescribirá el archivo\n"
#define recordReadErr "\t[!]Error: Fallo al grabar. No se han leido datos\n"
#define showReadErr   "\t[!]Error: Fallo al mostrar. No se ha leído ningún archivo\n"
#define showRecErr    "\t[!]Error: Fallo al mostrar. Archivo ya fue grabado\n"
#define exitReadErr   "\t[!]Error: Fallo al salir. No se ha leído ningún archivo\n"
#define exitRecErr    "\t[!]Error: Fallo al salir. No se ha grabado el archivo\n"

void leerArchivo        (struct arreglos *arreglo);
bool tieneExtensionTxt  (string nombre);
void sort               (struct arreglos *arreglo);
void displayArray       (struct arreglos *arreglo);
void recordFile         (struct arreglos *arreglo);
bool validAction        (int opt, struct arreglos *arreglo);
void insertionSort(int* arr, int izquierda, int derecha, bool ascendente);
void heapify(int* arr, int n, int i, bool ascendente);
void heapSort(int* arr, int n, bool ascendente);
void introsortUtil(int* arr, int izquierda, int derecha, int profundidadMax, bool ascendente);
void introsort(int* arr, int n, bool ascendente);
template <typename T>
T readUSR();

struct arreglos{
    int size = 0;
    int* elems = nullptr;
    bool read = 0, sortedAsc = 0, sortedDesc = 0, recorded = 0;
    bool isEmpty(){return elems == nullptr || size == 0; }
    bool check(bool condicion, const char* mensaje) {
        if (condicion) cout << mensaje;
        return !condicion;
    }
};

int main(){
    int opt;
    struct arreglos arreglo1;
    
    do{
        cout << "\t\t\tMenú Ordenamiento por Selección\n" <<
        "Elija una Opción (Ingresar Número): \n" << 
        "1. Leer Archivo\n" <<
        "2. Ordenar\n" << 
        "3. Grabar Archivo\n" <<
        "4. Mostrar Arreglo\n" << 
        "5. Salir\n" << 
        " > ";
        opt = readUSR<int>();
        if(!validAction(opt, &arreglo1)) continue;
        switch(opt){
            case 1: leerArchivo(&arreglo1); break;
            case 2: sort(&arreglo1); break;
            case 3: recordFile(&arreglo1); break;
            case 4: displayArray(&arreglo1); break;
            case 5: return 0;
        }
    }while(true);
}

void leerArchivo(struct arreglos *arreglo){
 
    string nombreArchivo;

    do{
        delete[] arreglo->elems;
        arreglo->elems = nullptr;

        cout << "Ingrese el nombre del archivo sin extensión: ";
        cin  >> nombreArchivo;

        if(tieneExtensionTxt(nombreArchivo)){
            cout << "  [!] No incluyas la extensión. " << "Escribe solo el nombre: \n";
            arreglo->elems = nullptr;
            continue;
        }

        nombreArchivo += ".txt";

        ifstream archivo(nombreArchivo);
        if (!archivo) {
            cout << "  [!] No se pudo abrir: " << nombreArchivo << "\n";
            arreglo->elems = nullptr;
            continue;
        }
    
        int temp;
        while (archivo >> temp) {
            arreglo->size++;
        }
    
        if (arreglo->size == 0) {
            cout << "  [!] El archivo \"" << nombreArchivo << "\" está vacío o no contiene enteros.\n";
            arreglo->elems = nullptr;
            continue;
        }
    
        arreglo->elems = new int[arreglo->size];
    
        archivo.clear();
        archivo.seekg(0, ios::beg);
    
        for (int i = 0; i < arreglo->size; i++) {
            archivo >> arreglo->elems[i];
        }
    
        continue;
    }while(arreglo->isEmpty());
    arreglo->read = true;
    cout << "\n  Archivo leído correctamente. " << arreglo->size << " entero(s) encontrado(s):\n\n";
    return;
}
template <typename T>
T readUSR() {
    T valor;
    while (true) {
        cin >> valor;
        if (cin.fail()) {
            cin.clear(); // Limpia el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta la entrada inválida
            cout << "Entrada inválida. Por favor, ingrese un número entero\n > ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return valor;
        }
    }
}
bool tieneExtensionTxt(string nombre){
    string ext = ".txt";
    if (nombre.size() < ext.size()) return false;
 
    string sufijo = nombre.substr(nombre.size() - ext.size());
    for (char& c : sufijo) c = tolower(c);
 
    return sufijo == ext;
}
void sort(struct arreglos *arreglo){
    auto start = std::chrono::high_resolution_clock::now();
    int opcion;
    // Llamada a QuickSort
    do{
        cout << "Seleccione ordenamiento [1] Ascendente [2] Descendente\n > ";
        opcion = readUSR<int>();
        if(opcion != 1 && opcion != 2){
            cout << "Opción Inválida. Intente de Nuevo... \n";
        }
    }while(opcion != 1 && opcion != 2);
    bool ascendente = (opcion == 1);
    if (ascendente && arreglo->sortedAsc) {
        cout << "\t[!]El arreglo ya está ordenado en orden ascendente.\n";
        return;
    }
    if (!ascendente && arreglo->sortedDesc) {
        cout << "\t[!]El arreglo ya está ordenado en orden descendente.\n";
        return;
    }
    if(opcion == 1){
        arreglo->sortedAsc = true;
        arreglo->sortedDesc = false;
    }
    if(opcion == 2){
        arreglo->sortedAsc = false;
        arreglo->sortedDesc = true;
    }
    introsort(arreglo->elems, arreglo->size, ascendente);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "\tArreglo Ordenado en " << duration.count()/1000000.0 << " segundos\n\n";
}
void displayArray(struct arreglos *arreglo){
    cout << "Cuantos elementos quiere mostrar: ";
    int elementos = readUSR<int>();
    if (elementos > arreglo->size) elementos = arreglo->size;
    cout << "Contenido del arreglo: ";
    for (int i = 0; i < elementos; i++) cout << arreglo->elems[i] << " ";
    cout << endl;
    cout << "\n\n";
}
void recordFile(struct arreglos *arreglo){
    string nombre;
    do{
        cout << "Ingrese el nombre del archivo para guardar (sin extensión): ";
        cin >> ws;
        getline(cin, nombre);
        if(tieneExtensionTxt(nombre)){
            cout << "  [!] No incluyas la extensión. " << "Escribe solo el nombre: \n";
            continue;
        }
        nombre += ".txt";
        ofstream archivo(nombre);
        if(!archivo){
            cout << "No se pudo crear el archivo\n";
            return;
        }
        for(int i = 0; i < arreglo->size; i++){
            archivo << arreglo->elems[i] << " ";
        }
        archivo.close();
        arreglo->recorded = true;
        cout << "Archivo Guardado Exitosamente como: " << nombre << endl;
    }while(!arreglo->recorded);
}
bool validAction(int opt, arreglos *arreglo) {
    switch (opt) {
        case 1: return arreglo->check(arreglo->read,       readErr);
        case 2: return arreglo->check(!arreglo->read,      sortReadErr)
                    && arreglo->check(arreglo->recorded,   sortRecErr );
        case 3: if(arreglo->recorded) cout << recordWarn;
                return arreglo->check(!arreglo->read,      recordReadErr);
        case 4: return arreglo->check(!arreglo->read,      showReadErr)
                    && arreglo->check(arreglo->recorded,   showRecErr);
        case 5: return arreglo->check(!arreglo->read,      exitReadErr)
                    && arreglo->check(!arreglo->recorded,  exitRecErr);
        default: cout << "Opción Inválida. Intente de Nuevo... \n"; return false;
    }
}
void insertionSort(int* arr, int izquierda, int derecha, bool ascendente) {
    for (int i = izquierda + 1; i <= derecha; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= izquierda && ((ascendente && arr[j] > key) || (!ascendente && arr[j] < key))) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void heapify(int* arr, int n, int i, bool ascendente) {
    int extremo = i;
    int izquierda = 2 * i + 1;
    int derecha = 2 * i + 2;

    if (ascendente) {
        if (izquierda < n && arr[izquierda] > arr[extremo]) extremo = izquierda;
        if (derecha < n && arr[derecha] > arr[extremo]) extremo = derecha;
    } else {
        if (izquierda < n && arr[izquierda] < arr[extremo]) extremo = izquierda;
        if (derecha < n && arr[derecha] < arr[extremo]) extremo = derecha;
    }

    if (extremo != i) {
        swap(arr[i], arr[extremo]);
        heapify(arr, n, extremo, ascendente);
    }
}
void heapSort(int* arr, int n, bool ascendente) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, ascendente);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, ascendente);
    }
}
void introsortUtil(int* arr, int izquierda, int derecha, int profundidadMax, bool ascendente) {
    int n = derecha - izquierda + 1;
    if (n <= 16) {
        insertionSort(arr, izquierda, derecha, ascendente);
        return;
    }
    if (profundidadMax == 0) {
        heapSort(arr + izquierda, n, ascendente);
        return;
    }

    int pivote = arr[izquierda + (derecha - izquierda) / 2];
    int i = izquierda, j = derecha;

    while (i <= j) {
        if (ascendente) {
            while (arr[i] < pivote) i++;
            while (arr[j] > pivote) j--;
        } else {
            while (arr[i] > pivote) i++;
            while (arr[j] < pivote) j--;
        }
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (izquierda < j) introsortUtil(arr, izquierda, j, profundidadMax - 1, ascendente);
    if (i < derecha) introsortUtil(arr, i, derecha, profundidadMax - 1, ascendente);
}
void introsort(int* arr, int n, bool ascendente) {
    int profundidadMax = 2 * log(n);
    cout << "Comienza Introsort con profundidad máxima permitida: " << profundidadMax << "\n";
    introsortUtil(arr, 0, n - 1, profundidadMax, ascendente);
}
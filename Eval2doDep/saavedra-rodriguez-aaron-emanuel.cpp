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
#define recordErr     "\t[!]Error: Fallo al grabar. El archivo ya fue grabado\n"
#define recordReadErr "\t[!]Error: Fallo al grabar. No se han leido datos\n"
#define showReadErr   "\t[!]Error: Fallo al mostrar. No se ha leído ningún archivo\n"
#define showRecErr    "\t[!]Error: Fallo al mostrar. Archivo ya fue grabado\n"
#define exitReadErr   "\t[!]Error: Fallo al salir. No se ha leído ningún archivo\n"
#define exitRecErr    "\t[!]Error: Fallo al salir. No se ha grabado el archivo\n"
#define exitSortErr   "\t[!]Error: Fallo al salir. El archivo no está ordenado\n"
#define searchReadErr "\t[!]Error: Fallo al buscar. No se ha leído ningún archivo\n"
#define searchSortErr "\t[!]Error: Fallo al buscar. El archivo no está ordenado\n"
#define deleteReadErr "\t[!]Error: Fallo al eliminar duplicados. No se ha leído ningún archivo\n"
#define deleteSortErr "\t[!]Error: Fallo al eliminar duplicados. El archivo no está ordenado\n"

void leerArchivo        (struct arreglos *arreglo);
bool tieneExtensionTxt  (string nombre);
void sort               (struct arreglos *arreglo);
void displayArray       (struct arreglos *arreglo);
void recordFile         (struct arreglos *arreglo);
bool validAction        (int opt, struct arreglos *arreglo);
void quickSortRecursive (int arr[], int low, int high, bool asc);
int  partition          (int arr[], int low, int high, bool asc);
void swap               (int &a, int &b);
void search             (struct arreglos *arreglo);
void removeDuplicates   (struct arreglos *arreglo);

struct arreglos{
    int size            = 0, i, mid;
    int* elems          = nullptr;
    int* dupNums        = nullptr;
    int* dupCounts      = nullptr;
    int  dupSize        = 0;
    bool read           = 0, sortedAsc = 0, sortedDesc = 0, recorded = 0, sortAsc = 0;
    bool isEmpty()      {return elems == nullptr || size == 0; }
    bool check          (bool condicion, const char* mensaje);
    bool binarySearch   (int left, int right, int searchValue, bool asc);
};
bool arreglos::check(bool condicion, const char* mensaje){
    if (condicion) cout << mensaje;
    return !condicion;
}
bool arreglos::binarySearch(int left, int right, int searchValue, bool asc){
    i++;
    if(right >= left){
        mid = left + (right-left)/2;
        if(elems[mid] == searchValue) return true;
        if(asc){
            if(elems[mid] > searchValue) return binarySearch(left, mid - 1, searchValue, asc);
            else                         return binarySearch(mid + 1, right, searchValue, asc);
        } else {
            if(elems[mid] > searchValue) return binarySearch(mid + 1, right, searchValue, asc);
            else                         return binarySearch(left, mid - 1, searchValue, asc);
        }
    }
    return false;
}
int leerEntero() {
    int valor;
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
int main(){
    int opt;
    struct arreglos arreglo1;
    
    do{
        cout << "\t\t\tMenú Ordenamiento por Selección\n" <<
        "Elija una Opción (Ingresar Número): \n" << 
        "1. Leer Archivo\n" <<
        "2. Ordenar\n" << 
        "3. Búsqueda Binaria\n" <<
        "4. Mostrar Arreglo\n" << 
        "5. Eliminar Duplicados\n" <<
        "6. Grabar Archivo\n" <<
        "7. Salir\n" << 
        " > ";
        opt = leerEntero();
        if(!validAction(opt, &arreglo1)) continue;
        switch(opt){
            case 1: leerArchivo(&arreglo1); break;
            case 2: sort(&arreglo1); break;
            case 3: search(&arreglo1); break;
            case 4: displayArray(&arreglo1); break;
            case 5: removeDuplicates(&arreglo1); break;
            case 6: recordFile(&arreglo1); break;
            case 7: 
                    cout << "Saliendo del programa...\n";
                    delete[] arreglo1.elems;
                    delete[] arreglo1.dupNums;
                    delete[] arreglo1.dupCounts;
            return 0;
        }
    }while(true);
}
//Opción 1 - Leer Archivo
void leerArchivo(struct arreglos *arreglo){
 
    string nombreArchivo;

    do{
        delete[] arreglo->elems;
        arreglo->elems = nullptr;

        cout << "Ingrese el nombre del archivo con extensión: ";
        cin  >> nombreArchivo;

        if(!tieneExtensionTxt(nombreArchivo)){
            cout << "  [!] Incluye la extensión. " << "Escribe el nombre con la extensión: \n";
            arreglo->elems = nullptr;
            continue;
        }

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
bool tieneExtensionTxt(string nombre){
    string ext = ".txt";
    if (nombre.size() < ext.size()) return false;
 
    string sufijo = nombre.substr(nombre.size() - ext.size());
    for (char& c : sufijo) c = tolower(c);
 
    return sufijo == ext;
}
//Opción 2 - Ordenar Archivo
void sort(struct arreglos *arreglo){
    int opt;
    cout << "Desea ordenar de forma ascendente o descendente [1] Ascendente, [2] Descendente";
    do{
        cout << "\n > ";
        opt = leerEntero();
        if(opt != 1 && opt != 2){
            cout << "Opción Inválida. Intente de Nuevo... \n";
        }
    }while(opt != 1 && opt != 2);
    bool asc = (opt == 1);
    if (asc && arreglo->sortedAsc) {
        cout << "\t[!]El arreglo ya está ordenado en orden ascendente.\n";
        return;
    }
    if (!asc && arreglo->sortedDesc) {
        cout << "\t[!]El arreglo ya está ordenado en orden descendente.\n";
        return;
    }
    if(opt == 1){
        arreglo->sortedAsc = true;
    }
    if(opt == 2){
        arreglo->sortedDesc = true;
    }
    auto start = chrono::high_resolution_clock::now();
    quickSortRecursive(arreglo->elems, 0, arreglo->size - 1, asc);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Tiempo de ordenamiento: " << duration.count() << " microsegundos\n";
}
void quickSortRecursive(int arr[], int low, int high, bool asc) {
    if (low < high) {
        int pi = partition(arr, low, high, asc);

        quickSortRecursive(arr, low, pi - 1, asc);
        quickSortRecursive(arr, pi + 1, high, asc);
    }
}
int partition(int arr[], int low, int high, bool asc){
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (asc ? arr[j] < pivot : arr[j] > pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}
//Opción 3 - Busqueda Binaria
void search(struct arreglos *arreglo){
    int searchValue;
    cout << "Ingresa un Número a Buscar: \n > ";
    searchValue = leerEntero();
    arreglo->i = 0;
    bool asc = arreglo->sortedAsc;
    auto start = chrono::high_resolution_clock::now();
    if(arreglo->binarySearch(0, arreglo->size - 1, searchValue, asc)){
        cout << "Se encontró el " << searchValue << ". Iteraciones: " << arreglo->i<< ". Posición: " << arreglo->mid + 1<< "\n\n";
    }
    else{
        cout << "No se encontró el " << searchValue << ". Iteraciones: " << arreglo->i << "\n\n";
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Tiempo de búsqueda: " << duration.count() << " microsegundos\n";
}
//Opción 5 - Eliminar Duplicados
void removeDuplicates(struct arreglos *arreglo){
    int* temp = new int[arreglo->size];
    int j = 0;

    arreglo->dupNums   = new int[arreglo->size];
    arreglo->dupCounts = new int[arreglo->size];

    auto start = chrono::high_resolution_clock::now();
    int i = 0;
    while (i < arreglo->size) {
        int count = 1;
        while (i + count < arreglo->size && arreglo->elems[i + count] == arreglo->elems[i])
            count++;
        if (count == 1) {
            temp[j++] = arreglo->elems[i];
        } else {
            arreglo->dupNums[arreglo->dupSize]   = arreglo->elems[i];
            arreglo->dupCounts[arreglo->dupSize] = count;
            arreglo->dupSize++;
        }
        i += count;
    }

    delete[] arreglo->elems;
    arreglo->elems = temp;
    arreglo->size  = j;
    for(int i = 0; i < arreglo->dupSize; i++){
        cout << "Número: " << arreglo->dupNums[i] << " - Veces Repetido: " << arreglo->dupCounts[i] << "\n";
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Tiempo de eliminación de duplicados: " << duration.count() << " microsegundos\n";
}
//Opción 6 - Grabar Archivo
void recordFile(struct arreglos *arreglo){
    string nombre;
    do{
        cout << "Ingrese el nombre del archivo para guardar (con extensión): ";
        cin >> ws;
        getline(cin, nombre);
        if(!tieneExtensionTxt(nombre)){
            cout << "  [!] Incluye la extensión. " << "Escribe el nombre con la extensión: \n";
            continue;
        }
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
//Opción 4 - Mostrar Arreglo
void displayArray(struct arreglos *arreglo){
    cout << "Cuantos elementos quiere mostrar\n > ";
    int elementos = leerEntero();
    if (elementos > arreglo->size) elementos = arreglo->size;
    cout << "Contenido del arreglo: ";
    for (int i = 0; i < elementos; i++) {
        cout << arreglo->elems[i] << " ";
    }
    cout << endl;

    if(arreglo->dupSize > 0){
        bool foundAny = false;
        for(int i = 0; i < arreglo->dupSize; i++){
            bool inRange = false;
            for(int j = 0; j < elementos; j++){
                if(arreglo->elems[j] == arreglo->dupNums[i]){
                    inRange = true;
                    break;
                }
            }
            if(inRange){
                if(!foundAny){
                    cout << "Números Duplicados:\n";
                    foundAny = true;
                }
                cout << "Número: " << arreglo->dupNums[i] << " - Veces Repetido: " << arreglo->dupCounts[i] << "\n";
            }
        }
    }
    cout << "\n\n";
}
//Validar que la opción del usuario sea posible de ejecutar
bool validAction(int opt, arreglos *arreglo) {
    switch (opt) {
        case 1: return arreglo->check(arreglo->read,       readErr);
        case 2: return arreglo->check(!arreglo->read,      sortReadErr)
                    && arreglo->check(arreglo->recorded,   sortRecErr );
        case 3: return arreglo->check(!arreglo->read,      searchReadErr)
                    && arreglo->check(!arreglo->sortedAsc && !arreglo->sortedDesc, searchSortErr);
        case 4: return arreglo->check(!arreglo->read,      showReadErr)
                    && arreglo->check(arreglo->recorded,   showRecErr);
        case 5: return arreglo->check(!arreglo->read,      deleteReadErr)
                    && arreglo->check(!arreglo->sortedAsc && !arreglo->sortedDesc, deleteSortErr);
        case 6: return arreglo->check(!arreglo->read,      recordReadErr)
                    && arreglo->check(arreglo->recorded,   recordErr);
        case 7: return arreglo->check(!arreglo->read,      exitReadErr)
                    && arreglo->check(!arreglo->sortedAsc && !arreglo->sortedDesc, exitSortErr)   
                    && arreglo->check(!arreglo->recorded,  exitRecErr);
        default: cout << "Opción Inválida. Intente de Nuevo... \n"; return false;
    }
}
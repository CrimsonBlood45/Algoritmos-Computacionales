#include <iostream>
#include <fstream>
#include <string>
#include <locale>

using namespace std;

#define readErr     "   [!] No se puede leer. Ya se leyó un archivo\n"
#define showReadErr "   [!] No se pudo Mostrar. No se ha leído ningún archivo\n"
#define sortReadErr "   [!] No se pudo Ordenar. No se ha leído ningún archivo\n"

void leerArchivo(struct arreglos *arreglo);
bool tieneExtensionTxt(string nombre);
void search(struct arreglos *arreglo);
void displayArray(struct arreglos *arreglo);
bool validAction(int opt, struct arreglos *arreglo);
void close(struct arreglos *arreglo);

struct arreglos{
    int size = 0, i, mid;
    int* elems = nullptr;
    bool read = 0, sorted = 0, recorded = 0;
    bool isEmpty();
    bool check(bool condicion, const char* mensaje);
    bool binarySearch(int left, int right, int searchValue);
};

bool arreglos::isEmpty(){return elems == nullptr || size == 0;}
bool arreglos::check(bool condicion, const char* mensaje){
    if (!condicion) cout << mensaje;
    return condicion;
}
bool arreglos::binarySearch(int left, int right, int searchValue){
    i++;
    if(right >= left){
        mid = left + (right-left)/2;
        if(elems[mid] == searchValue) return true;
        else if(elems[mid] > searchValue) return binarySearch(left, mid - 1, searchValue);
        else return binarySearch(mid + 1, right, searchValue);
    }
    if(elems[mid] == searchValue){
        cout << "";
    }
    return false;
}

int main(){
    setlocale(LC_ALL,"");
    int opt;
    struct arreglos arreglo1;
    
    do{
        cout << "\t\t\tMenú Ordenamiento por Selección\n" <<
        "Elija una Opción (Ingresar Número): \n" << 
        "1. Leer Archivo\n" <<
        "2. Mostrar Arreglo\n" << 
        "3. Búsqueda Binaria\n" << 
        "4. Salir\n" << 
        " > ";
        cin >> opt;
        if(!validAction(opt, &arreglo1)) continue;
        switch(opt){
            case 1: leerArchivo(&arreglo1); break;
            case 2: displayArray(&arreglo1); break;
            case 3: search(&arreglo1); break;
            case 4: close(&arreglo1); return 0;
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
            if(arreglo->elems[i] < arreglo->elems[i-1]){
                cout << "   [!] El archivo no está ordenado\n";
                arreglo->elems = nullptr;
                break;
            }
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
void search(struct arreglos *arreglo){
    int searchValue;
    cout << "Ingresa un Número a Buscar: \n > ";
    cin >> searchValue;
    arreglo->i = 0;
    if(arreglo->binarySearch(0, arreglo->size, searchValue)){
        cout << "Se encontró el " << searchValue << ". Iteraciones: " << arreglo->i<< ". Posición: " << arreglo->mid + 1<< "\n\n";
    }
    else{
        cout << "No se encontró el " << searchValue << ". Iteraciones: " << arreglo->i << "\n\n";
    }
}
void displayArray(struct arreglos *arreglo){
    cout << "Arreglo Actual: ";
    if(arreglo->isEmpty()){cout << "(Empty)\n\n";}
    for(int i = 0; i < arreglo->size; i++){
        cout << arreglo->elems[i] << " ";
    }
    cout << "\n\n";
}
void close(struct arreglos *arreglo){
    delete arreglo->elems;
    cout << "Cerrando Programa... \n";
}
bool validAction(int opt, arreglos *arreglo) {
    switch (opt) {
        case 1: return arreglo->check(!arreglo->read,      readErr);
        case 2: return arreglo->check(arreglo->read,      showReadErr);
        case 3: return arreglo->check(arreglo->read,      sortReadErr);
        case 4: return true;
        default: cout << "Opción Inválida. Intente de Nuevo... \n"; return false;
    }
}
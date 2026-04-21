#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <chrono>

#define readErr         "   [!] No se puede leer. Ya se leyó un archivo\n"
#define sortErr         "   [!] No se puede ordenar. El arreglo ya fue ordenado\n"
#define sortReadErr     "   [!] No se puede ordenar. No se han leído datos\n"
#define sortRecErr      "   [!] No se puede ordenar. El archivo ya fue grabado\n"
#define recordErr       "   [!] No se puede grabar. El archivo ya fue grabado\n"
#define recordReadErr   "   [!] No se puede grabar. No se han leído datos\n"
#define showReadErr     "   [!] No se puede mostrar. No se ha leído ningún dato\n"
#define showRecErr      "   [!] No se puede mostrar. El archivo ya fue grabado\n"
#define exitReadErr     "   [!] No se puede salir. No se ha leído ningún archivo\n"
#define exitRecErr      "   [!] No se puede salir. No se ha grabado el archivo\n"

using namespace std;

void leerArchivo(struct arreglos *arreglo);
bool tieneExtensionTxt(string nombre);
void insertSort(struct arreglos *arreglo);
void recordFile(struct arreglos *arreglo);
void displayArray(struct arreglos *arreglo);
bool validAction(int opt, arreglos *arreglo);
void close(struct arreglos *arreglo);

struct arreglos{
    int size = 0, i, mid;
    int* elems = nullptr;
    bool read = 0, sorted = 0, recorded = 0;
    bool check(bool condicion, const char* mensaje);
    bool isEmpty();
};
bool arreglos::check(bool condicion, const char* mensaje){
    if (!condicion) cout << mensaje;
        return condicion;
}
bool arreglos::isEmpty(){return elems == nullptr || size == 0;}

int main(){
    setlocale(LC_ALL,"");
    int opt;
    struct arreglos arreglo1;
    
    do{
        cout << "\t\t\tMenú Ordenamiento por Selección\n" <<
        "Elija una Opción (Ingresar Número): \n" << 
        "1. Leer Archivo\n" <<
        "2. Ordenar (Inserción)\n" << 
        "3. Grabar Archivo\n" << 
        "4. Mostrar Arreglo\n" <<
        "5. Salir\n" << 
        " > ";
        cin >> opt;
        if(!validAction(opt, &arreglo1)) continue;
        switch(opt){
            case 1: leerArchivo(&arreglo1); break;
            case 2: insertSort(&arreglo1); break;
            case 3: recordFile(&arreglo1); break;
            case 4: displayArray(&arreglo1); break;
            case 5: close(&arreglo1); return 0;
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
bool tieneExtensionTxt(string nombre){
    string ext = ".txt";
    if (nombre.size() < ext.size()) return false;
 
    string sufijo = nombre.substr(nombre.size() - ext.size());
    for (char& c : sufijo) c = tolower(c);
 
    return sufijo == ext;
}
void insertSort(struct arreglos *arreglo){
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < arreglo->size; ++i) {
        int key = arreglo->elems[i];
        int j = i - 1;

        while (j >= 0 && arreglo->elems[j] > key) {
            arreglo->elems[j + 1] = arreglo->elems[j];
            j = j - 1;
        }
        arreglo->elems[j + 1] = key;
    }
    arreglo->sorted = true;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "\tArreglo Ordenado en " << duration.count()/1000000.0 << " segundos\n\n";
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
void displayArray(struct arreglos *arreglo){
    cout << "Arreglo Actual: ";
    if(arreglo->isEmpty()){cout << "(Empty)\n\n";}
    for(int i = 0; i < arreglo->size; i++){
        cout << arreglo->elems[i] << " ";
    }
    cout << "\n\n";
}
bool validAction(int opt, arreglos *arreglo){
    //Evitar Relectura
    //Ordenamiento si ya leyó archivo y no ha grabado. Evitar reordenar.
    //Deben haber datos antes de grabar. Impedir volver a grabar
    //Solo puede mostrar si se ha leido y no ha grabado
    //Salir solo si ya se leyó y grabó
    switch (opt) {
        case 1: return arreglo->check(!arreglo->read,     readErr);
        case 2: return arreglo->check(arreglo->read,      sortReadErr)
                    && arreglo->check(!arreglo->recorded, sortRecErr)
                    && arreglo->check(!arreglo->sorted,   sortErr);
        case 3: return arreglo->check(arreglo->read,      recordReadErr)
                    && arreglo->check(!arreglo->recorded, recordErr);
        case 4: return arreglo->check(arreglo->read,      showReadErr)
                    && arreglo->check(!arreglo->recorded, showRecErr);
        case 5: return arreglo->check(arreglo->read,      exitReadErr)
                    && arreglo->check(arreglo->recorded,  exitRecErr);
        default: cout << "Opción Inválida. Intente de Nuevo... \n"; return false;
    }
}
void close(struct arreglos *arreglo){
    delete arreglo->elems;
    cout << "Cerrando Programa... \n";
}
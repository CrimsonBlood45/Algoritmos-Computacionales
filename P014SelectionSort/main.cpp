#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

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

void leerArchivo(struct arreglos *arreglo);
bool tieneExtensionTxt(string nombre);
void selectSort(struct arreglos *arreglo);
void displayArray(struct arreglos *arreglo);
void recordFile(struct arreglos *arreglo);
bool validAction(int opt, arreglos *arreglo);

struct arreglos{
    int size = 0;
    int* elems = nullptr;
    bool read = 0, sorted = 0, recorded = 0;
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
        cin >> opt;
        if(!validAction(opt, &arreglo1)) continue;
        switch(opt){
            case 1: leerArchivo(&arreglo1); break;
            case 2: selectSort(&arreglo1); break;
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
bool tieneExtensionTxt(string nombre){
    string ext = ".txt";
    if (nombre.size() < ext.size()) return false;
 
    string sufijo = nombre.substr(nombre.size() - ext.size());
    for (char& c : sufijo) c = tolower(c);
 
    return sufijo == ext;
}
void selectSort(struct arreglos *arreglo){
    auto start = std::chrono::high_resolution_clock::now();
    int minIndex, temp;
    for(int i = 0; i < arreglo->size; i++){
        minIndex = i;
        for(int j = i + 1; j < arreglo->size; j++){
            if(arreglo->elems[minIndex] > arreglo->elems[j]){
                temp = arreglo->elems[minIndex];
                arreglo->elems[minIndex] = arreglo->elems[j];
                arreglo->elems[j] = temp;
            }
        }
    }
    arreglo->sorted = true;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "\tArreglo Ordenado en " << duration.count()/1000000.0 << " segundos\n\n";
}
void displayArray(struct arreglos *arreglo){
    cout << "Arreglo Actual: ";
    if(arreglo->isEmpty()){cout << "(Empty)\n\n";}
    for(int i = 0; i < arreglo->size; i++){
        cout << arreglo->elems[i] << " ";
    }
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
                    && arreglo->check(arreglo->sorted,     sortErr)
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
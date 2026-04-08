#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int* leerArchivo(const string& nombreArchivo, int& tam);
void mostrarArreglo(int* arr, int tam);
void ordenar(int* arr, int tam);
void buscar(int* arr, int tam, int searchValue);
void eliminar(int* arr, int tam, int searchValue, int pos);
void grabar(int* arr, int tam);

bool readFile = 0;
bool archivoGrabado = 0;
bool arregloOrdenado = 0;
bool deleted = 0;

int main(){
    string nombreArchivo;
    int tam = 0;
    int* arreglo = nullptr;
    int opcion, valor, pos;
    do{
        cout << "Menú de opciones\n\t1. Leer archivo\n\t2. Ordenar\n\t3. Buscar\n\t4. Eliminar\n\t5. Mostrar Arreglo\n\t6. Grabar\n\t7. Salir\n" << endl;
        cin >> opcion;
        string nombreArchivo;
        switch(opcion){
            case 1:
                if(readFile){
                    cout << "ERROR: El archivo ya fue leido\n";
                    break;
                }
                if(archivoGrabado){
                    cout << "ERROR: El arreglo ya fue grabado\n";
                    break;
                }
                do{
                    cout << "Ingrese el nombre del archivo sin extension: ";
                    cin >> nombreArchivo;
                    nombreArchivo+=".txt";
                }while(!readFile);
                if(arreglo){
                    delete[] arreglo;
                }
                arreglo = leerArchivo(nombreArchivo, tam);
                mostrarArreglo(arreglo, tam);
                break;
            case 2:
                if(!readFile){
                    cout << "ERROR: No se ha leido archivo\n";
                    break;
                }
                if(arregloOrdenado){
                    cout << "ERROR: El arreglo ya se ordenó\n";
                    break;
                }
                if(archivoGrabado){
                    cout << "ERROR: El arreglo ya fue grabado\n";
                    break;
                }
                ordenar(arreglo, tam);
                mostrarArreglo(arreglo, tam);
                break;
            case 3:
                if(!readFile){
                    cout << "ERROR: No se ha leido archivo\n";
                    break;
                }
                if(!arregloOrdenado){
                    cout << "ERROR: No se ha ordenado el arreglo\n";
                    break;
                }
                if(archivoGrabado){
                    cout << "ERROR: El arreglo ya fue grabado\n";
                    break;
                }
                cout << "Ingrese un valor a buscar: ";
                cin >> valor;
                buscar(arreglo, tam, valor);
                break;
            case 4:
                if(!readFile){
                    cout << "ERROR: No se ha leido archivo\n";
                    break;
                }
                if(!arregloOrdenado){
                    cout << "ERROR: No se ha ordenado el arreglo\n";
                    break;
                }
                if(archivoGrabado){
                    cout << "ERROR: El arreglo ya fue grabado\n";
                    break;
                }
                if(!readFile){
                    cout << "ERROR: No se ha leido archivo\n";
                    break;
                }
                cout << "Ingrese un valor a buscar: ";
                cin >> valor;
                cout << "Ingrese posicion del valor a buscar: ";
                cin >> pos;
                eliminar(arreglo, tam, valor, pos);
                break;
            case 5:
                if(!readFile){
                    cout << "ERROR: No se ha leido archivo\n";
                    break;
                }
                if(archivoGrabado){
                    cout << "ERROR: El arreglo ya fue grabado\n";
                    break;
                }
                if(!readFile){
                    cout << "ERROR: No se ha leido archivo\n";
                    break;
                }
                mostrarArreglo(arreglo, tam);
                break;
            case 6:
                if(!readFile){
                    cout << "ERROR: No se ha leido archivo\n";
                    break;
                }
                if(!deleted){
                    cout << "ERROR: No se ha borrado ningun valor\n";
                    break;
                }
                grabar(arreglo, tam);
        }
    }while(opcion != 7 || !readFile || !archivoGrabado);
}

int* leerArchivo(const string& nombreArchivo, int& tam){
    ifstream archivo(nombreArchivo);
    if(!archivo){
        cout << "Error al abrir el archivo " << nombreArchivo << endl;
        readFile = 0;
        exit(1);
    }
    tam = 0;
    int temp;
    while(archivo >> temp){
        tam++;
    }
    if(tam == 0){
        cout << "El archivo está vacío.\n";
        archivo.close();
        return nullptr;
    }
    archivo.clear();
    archivo.seekg(0, ios::beg);

    int* arreglo = new int[tam];
    for(int i = 0; i < tam; i++){
        archivo >> arreglo[i];
    }
    archivo.close();
    readFile = 1;
    return arreglo;
}
void ordenar(int* arr, int tam){
    int temp;
    if(arr == nullptr || tam == 0){
        cout << "El arreglo está vacío." << endl;
        return;
    }
    for(int i = 0; i < tam - 1; i++){
        for(int j = i; j < tam; j++){
            if(arr[i] > arr[j]){
                temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
    arregloOrdenado = 1;
    return;
}
void buscar(int* arr, int tam, int searchValue){
    int count = 0, i = 0; 
    if(arr == nullptr || tam == 0){
        cout << "El arreglo está vacío." << endl;
        return;
    }
    for(i = 0; i < tam; i++){
        if(arr[i] == searchValue){
            count++;
        }
    }
    count > 0 ? cout << "Se encontró el " << searchValue << " " << count << " veces en " << i << " iteraciones" << endl: cout << "No se encontró el valor en el arreglo\n";
}
void eliminar(int* arr, int tam, int searchValue, int pos){
    int i;
    if(arr == nullptr || tam == 0){
        cout << "El arreglo está vacío." << endl;
        return;
    }
    for(i = 0; i < tam; i++){
        if(arr[i] == searchValue && i == pos){
            break;
        }
    }
    if(arr[i] == searchValue && i == pos){
        for(i = 0; i < tam; i++){
            arr[i] = 0;
        }
        deleted = 1;
    }
}
void mostrarArreglo(int* arr, int tam){
    if(arr == nullptr || tam == 0){
        cout << "El arreglo está vacío." << endl;
        return;
    }
    cout << "Arreglo actual: ";
    for(int i = 0; i < tam; i++){
        cout << arr[i] << " ";
    }
    cout << endl;
}
void grabar(int* arr, int tam){
    if(!readFile){
        cout << "Primero debe leer un archivo\n";
        return;
    }
    if(archivoGrabado){
        cout << "Ya se grabó el archivo\n";
        return;
    }
    string nombre;
    cout << "Ingrese el nombre del archivo para guardar (sin extensión): ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt";
    ofstream archivo(nombre);
    if(!archivo){
        cout << "No se pudo crear el archivo\n";
        return;
    }
    for(int i = 0;i < tam; i++){
        archivo << arr[i] << " ";
    }
    archivoGrabado = 1;
    archivo.close();
    cout << "Archivo Guardado Exitosamente como: " << nombre << endl;
}
// Autores: Edna Martha Miranda Chávez
// Sergio Fuenlabrada Velázquez
// Programa P041

#include <iostream>
#include <time.h>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <limits>

using namespace std;

// Definición de la estructura para leer del archivo de entrada
FILE *archivo1;
struct Registro {
    int num;
} entrada;

// Definición de la estructura para escribir en el archivo de salida con tiempos de ejecución
FILE *archivo2;

// Variable auxiliar para referenciar archivo de salida dentro del recorrido
FILE *archivoSalidaOrdenado;

struct Registro1 {
    int num1; // es el nodo que se buscó
    char coma1;
    double secs1; // tiempo de búsqueda
    char coma2;
    int bus; // resultado de la búsqueda 1= encontrado, -1= no encontrado
} entrada1[10000];

int salvan = 0;
clock_t t_ini, t_fin;
double secs;

class AVL;

// Clase Nodo de Árbol AVL:
class Nodo {
public:
    // Constructor:
    Nodo(const int dat, Nodo *pad = NULL, Nodo *izq = NULL, Nodo *der = NULL) 
        : dato(dat), padre(pad), izquierdo(izq), derecho(der), FE(0) {}

    // Miembros:
    int dato;
    int FE;
    Nodo *izquierdo;
    Nodo *derecho;
    Nodo *padre;

    friend class AVL;
};

class AVL {
private:
    enum { IZQUIERDO, DERECHO };
    Nodo *raiz;
    Nodo *actual;
    int contador;
    int altura;

public:
    AVL() : raiz(NULL), actual(NULL) {}
    ~AVL() { Podar(raiz); }

    void Insertar(const int dat);
    int Borrar(const int dat);
    int Buscar(const int dat);
    bool Vacio(Nodo *r) { return r == NULL; }
    bool EsHoja(Nodo *r) { return !r->derecho && !r->izquierdo; }
    const int NumeroNodos();
    const int AlturaArbol();
    int Altura(const int dat);
    int &ValorActual() { return actual->dato; }
    void Raiz() { actual = raiz; }
    void InOrden(void (*func)(int&, int), Nodo *nodo = NULL, bool r = true);
    void PreOrden(void (*func)(int&, int), Nodo *nodo = NULL, bool r = true);
    void PostOrden(void (*func)(int&, int), Nodo *nodo = NULL, bool r = true);
    int Obtencion();
    int Otronodo();
    int Borrarnodo();
    int CreaSalida();

private:
    void Equilibrar(Nodo *nodo, int, bool);
    void RSI(Nodo* nodo);
    void RSD(Nodo* nodo);
    void RDI(Nodo* nodo);
    void RDD(Nodo* nodo);
    void Podar(Nodo* &);
    void auxContador(Nodo*);
    void auxAltura(Nodo*, int);
} ArbolInt;

// Borrar un nodo
int AVL::Borrarnodo() {
    int nodo2;
    int si = 1;
    int enc = -1;
    do {
        cout << "Indica nodo a borrar: ";
        cin >> nodo2;
        enc = ArbolInt.Borrar(nodo2);
        if (enc == 0) {
            cout << "El nodo " << nodo2 << " fue borrado" << endl;
        } else {
            cout << "El nodo " << nodo2 << " no existe" << endl;
        }
        cout << "¿Desea borrar otro (si=1, no=0)? ";
        cin >> si;
    } while (si == 1);
    return 0;
}

// Agrega otro nodo
int AVL::Otronodo() {
    int numnod;
    int nodo1;
    int k;
    do {
        cout << "¿Cuántos nodos deseas agregar? ";
        cin >> numnod;
    } while (numnod < 0);
    for (k = 1; k <= numnod; k++) {
        cout << "Teclea número de nodo: ";
        cin >> nodo1;
        ArbolInt.Insertar(nodo1);
    }
    return 0;
}

// Obtención: Obtiene árbol desde archivo
int AVL::Obtencion() {
    int j;
    int n = 0;
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo sin extensión: ";
    cin >> nombreArchivo;
    nombreArchivo += ".txt";
    archivo1 = fopen(nombreArchivo.c_str(), "r"); // apertura de archivo de entrada

    if (archivo1 == NULL) {
        cout << "Error - No se puede abrir el archivo " << nombreArchivo << endl;
        return 1;
    } else {
        cout << "Se abrió el archivo correctamente " << nombreArchivo << endl;
    }
    n = 0;
    for (int j = 0; !feof(archivo1); j++) {
        fscanf(archivo1, "%i", &entrada.num);
        ArbolInt.Insertar(entrada.num);
        entrada1[n].num1 = entrada.num;
        n++;
    }
    salvan = n;
    fclose(archivo1);
    return 0;
}

// Función que escribe en archivo los nodos en orden InOrden
void GuardarInOrden(int &dato, int FE) {
    fprintf(archivoSalidaOrdenado, "%d\n", dato);
}

// Crear archivo de salida
int AVL::CreaSalida() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo sin extensión para guardar (en orden InOrden): ";
    cin >> nombreArchivo;
    nombreArchivo += ".txt";
    archivoSalidaOrdenado = fopen(nombreArchivo.c_str(), "w");
    if (archivoSalidaOrdenado == NULL) {
        cout << "No se puede abrir el archivo " << nombreArchivo << " para escritura." << endl;
        return 1;
    }
    cout << ">> Guardando árbol en orden InOrden en el archivo " << nombreArchivo << "..." << endl;
    // Recorre el árbol y guarda los datos en orden
    InOrden(GuardarInOrden);
    fclose(archivoSalidaOrdenado);
    cout << ">> Archivo guardado correctamente\n";
    return 0;
}

// Poda: borrar todos los nodos a partir de uno, incluido
void AVL::Podar(Nodo* &nodo) {
    if (nodo) {
        Podar(nodo->izquierdo); // Podar izquierdo
        Podar(nodo->derecho);   // Podar derecho
        delete nodo;            // Eliminar nodo
        nodo = NULL;
    }
}

// Insertar un dato en el árbol AVL
void AVL::Insertar(const int dat) {
    Nodo *padre = NULL;
    cout << "Insertar: " << dat << endl;
    actual = raiz;
    // Buscar posición de inserción o detectar duplicado
    while (!Vacio(actual)) {
        if (dat == actual->dato) {
            cout << ">> El nodo " << dat << " ya existe. No se insertará duplicado\n";
            return; // salir sin insertar
        }
        padre = actual;
        if (dat > actual->dato) actual = actual->derecho;
        else actual = actual->izquierdo;
    }
    // Insertar nuevo nodo
    if (Vacio(padre)) {
        raiz = new Nodo(dat);
    } else if (dat < padre->dato) {
        padre->izquierdo = new Nodo(dat, padre);
        Equilibrar(padre, IZQUIERDO, true);
    } else {
        padre->derecho = new Nodo(dat, padre);
        Equilibrar(padre, DERECHO, true);
    }
}

// Equilibrar árbol AVL partiendo del nodo nuevo
void AVL::Equilibrar(Nodo *nodo, int rama, bool nuevo) {
    bool salir = false;

    while (nodo && !salir) {
        if (nuevo) {
            if (rama == IZQUIERDO) nodo->FE--;
            else nodo->FE++;
        } else {
            if (rama == IZQUIERDO) nodo->FE++;
            else nodo->FE--;
        }
        
        if (nodo->FE == 0) salir = true;
        else if (nodo->FE == -2) {
            if (nodo->izquierdo->FE == 1) RDD(nodo);
            else RSD(nodo);
            salir = true;
        } else if (nodo->FE == 2) {
            if (nodo->derecho->FE == -1) RDI(nodo);
            else RSI(nodo);
            salir = true;
        }
        
        if (nodo->padre) {
            if (nodo->padre->derecho == nodo) rama = DERECHO; 
            else rama = IZQUIERDO;
        }
        nodo = nodo->padre;
    }
}

// Rotación doble a derechas
void AVL::RDD(Nodo* nodo) {
    cout << "RDD" << endl;
    Nodo *Padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->izquierdo;
    Nodo *R = Q->derecho;
    Nodo *B = R->izquierdo;
    Nodo *C = R->derecho;
    
    if (Padre) {
        if (Padre->derecho == nodo) Padre->derecho = R;
        else Padre->izquierdo = R;
    } else {
        raiz = R;
    }
    
    Q->derecho = B;
    P->izquierdo = C;
    R->izquierdo = Q;
    R->derecho = P;
    R->padre = Padre;
    P->padre = Q->padre = R;

    if (B) B->padre = Q;
    if (C) C->padre = P;
    
    switch (R->FE) {
        case -1: Q->FE = 0; P->FE = 1; break;
        case 0:  Q->FE = 0; P->FE = 0; break;
        case 1:  Q->FE = -1; P->FE = 0; break;
    }
    R->FE = 0;
}

// Rotación doble a izquierdas
void AVL::RDI(Nodo* nodo) {
    cout << "RDI" << endl;
    Nodo *Padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->derecho;
    Nodo *R = Q->izquierdo;
    Nodo *B = R->izquierdo;
    Nodo *C = R->derecho;
    
    if (Padre) {
        if (Padre->derecho == nodo) Padre->derecho = R;
        else Padre->izquierdo = R;
    } else {
        raiz = R;
    }
    
    P->derecho = B;
    Q->izquierdo = C;
    R->izquierdo = P;
    R->derecho = Q;
    R->padre = Padre;
    P->padre = Q->padre = R;
    
    if (B) B->padre = P;
    if (C) C->padre = Q;
    
    switch (R->FE) {
        case -1: P->FE = 0; Q->FE = 1; break;
        case 0:  P->FE = 0; Q->FE = 0; break;
        case 1:  P->FE = -1; Q->FE = 0; break;
    }
    R->FE = 0;
}

// Rotación simple a derechas
void AVL::RSD(Nodo* nodo) {
    cout << "RSD" << endl;
    Nodo *Padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->izquierdo;
    Nodo *B = Q->derecho;
    
    if (Padre) {
        if (Padre->derecho == P) Padre->derecho = Q;
        else Padre->izquierdo = Q;
    } else {
        raiz = Q;
    }
    
    P->izquierdo = B;
    Q->derecho = P;
    P->padre = Q;
    if (B) B->padre = P;
    Q->padre = Padre;
    P->FE = 0;
    Q->FE = 0;
}

// Rotación simple a izquierdas
void AVL::RSI(Nodo* nodo) {
    cout << "RSI" << endl;
    Nodo *Padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->derecho;
    Nodo *B = Q->izquierdo;
    
    if (Padre) {
        if (Padre->derecho == P) Padre->derecho = Q;
        else Padre->izquierdo = Q;
    } else {
        raiz = Q;
    }
    
    P->derecho = B;
    Q->izquierdo = P;
    P->padre = Q;
    if (B) B->padre = P;
    Q->padre = Padre;
    P->FE = 0;
    Q->FE = 0;
}

// Eliminar un elemento de un árbol AVL
int AVL::Borrar(const int dat) {
    Nodo *padre = NULL;
    Nodo *nodo;
    int aux;
    actual = raiz;
    
    while (!Vacio(actual)) {
        if (dat == actual->dato) {
            if (EsHoja(actual)) {
                if (padre) {
                    if (padre->derecho == actual) padre->derecho = NULL;
                    else if (padre->izquierdo == actual) padre->izquierdo = NULL;
                }
                delete actual;
                actual = NULL;
                
                if (padre && ((padre->derecho == actual && padre->FE == 1) ||
                    (padre->izquierdo == actual && padre->FE == -1))) {
                    padre->FE = 0;
                    actual = padre;
                    padre = actual->padre;
                }
                if (padre) {
                    if (padre->derecho == actual) Equilibrar(padre, DERECHO, false);
                    else Equilibrar(padre, IZQUIERDO, false);
                }
                return 0;
            } else {
                padre = actual;
                if (actual->derecho) {
                    nodo = actual->derecho;
                    while (nodo->izquierdo) {
                        padre = nodo;
                        nodo = nodo->izquierdo;
                    }
                } else {
                    nodo = actual->izquierdo;
                    while (nodo->derecho) {
                        padre = nodo;
                        nodo = nodo->derecho;
                    }
                }
                aux = actual->dato;
                actual->dato = nodo->dato;
                nodo->dato = aux;
                actual = nodo;
            }
        } else {
            padre = actual;
            if (dat > actual->dato) actual = actual->derecho;
            else if (dat < actual->dato) actual = actual->izquierdo;
        }
    }
    return 1;
}

void AVL::InOrden(void (*func)(int&, int), Nodo *nodo, bool r) {
    if (r) nodo = raiz;
    if (nodo->izquierdo) InOrden(func, nodo->izquierdo, false);
    func(nodo->dato, nodo->FE);
    if (nodo->derecho) InOrden(func, nodo->derecho, false);
}

void AVL::PreOrden(void (*func)(int&, int), Nodo *nodo, bool r) {
    if (r) nodo = raiz;
    func(nodo->dato, nodo->FE);
    if (nodo->izquierdo) PreOrden(func, nodo->izquierdo, false);
    if (nodo->derecho) PreOrden(func, nodo->derecho, false);
}

void AVL::PostOrden(void (*func)(int&, int), Nodo *nodo, bool r) {
    if (r) nodo = raiz;
    if (nodo->izquierdo) PostOrden(func, nodo->izquierdo, false);
    if (nodo->derecho) PostOrden(func, nodo->derecho, false);
    func(nodo->dato, nodo->FE);
}

int AVL::Buscar(const int dat) {
    actual = raiz;
    while (!Vacio(actual)) {
        if (dat == actual->dato) return 1;
        else if (dat > actual->dato) actual = actual->derecho;
        else if (dat < actual->dato) actual = actual->izquierdo;
    }
    return -1;
}

int AVL::Altura(const int dat) {
    int altura = 0;
    actual = raiz;
    while (!Vacio(actual)) {
        if (dat == actual->dato) return altura;
        else {
            altura++;
            if (dat > actual->dato) actual = actual->derecho;
            else if (dat < actual->dato) actual = actual->izquierdo;
        }
    }
    return -1;
}

const int AVL::NumeroNodos() {
    contador = 0;
    auxContador(raiz);
    return contador;
}

void AVL::auxContador(Nodo *nodo) {
    contador++;
    if (nodo->izquierdo) auxContador(nodo->izquierdo);
    if (nodo->derecho) auxContador(nodo->derecho);
}

const int AVL::AlturaArbol() {
    altura = 0;
    auxAltura(raiz, 0);
    return altura;
}

void AVL::auxAltura(Nodo *nodo, int a) {
    if (nodo->izquierdo) auxAltura(nodo->izquierdo, a + 1);
    if (nodo->derecho) auxAltura(nodo->derecho, a + 1);
    if (EsHoja(nodo) && a > altura) altura = a;
}

void Mostrar(int &d, int FE) {
    cout << d << "(" << FE << "),";
}

// Submenú para mostrar recorridos del árbol AVL
void SubmenuRecorridos() {
    int opcion = 0;
    do {
        cout << "\n>> Submenú: Mostrar recorridos del árbol <<\n";
        cout << "1. Recorrido InOrden\n";
        cout << "2. Recorrido PostOrden\n";
        cout << "3. Recorrido PreOrden\n";
        cout << "4. Regresar al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                cout << "Recorrido InOrden: ";
                ArbolInt.InOrden(Mostrar);
                cout << endl;
                break;
            case 2:
                cout << "Recorrido PostOrden: ";
                ArbolInt.PostOrden(Mostrar);
                cout << endl;
                break;
            case 3:
                cout << "Recorrido PreOrden: ";
                ArbolInt.PreOrden(Mostrar);
                cout << endl;
                break;
            case 4:
                cout << "Regresando al menú principal\n";
                break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
        }
    } while (opcion != 4);
}

int main() {
    setlocale(LC_ALL, ""); // Establece el locale para aceptar caracteres con acentos y ñ
    int opc = 0;
    int nn;
    int an;
    int bandera = 0; // para validar que no se ha creado el árbol
    bool archivoGuardado = false; // Bandera para verificar si el archivo ha sido guardado
    int b1 = -1;
    
    do {
        cout << " Árbol AVL\n";
        cout << " Menú\n\n";
        cout << "1. Creación del árbol\n";
        cout << "2. Insertar nodo\n";
        cout << "3. Borrar nodo\n";
        cout << "4. Buscar nodo\n";
        cout << "5. Mostrar AVL\n";
        cout << "6. Altura del arbol\n";
        cout << "7. Altura de un nodo determinado\n";
        cout << "8. Número de nodos\n";
        cout << "9. Archivo de salida\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        
        while (!(cin >> opc)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Por favor ingrese un número válido: ";
        }
        
        if (opc != 1 && bandera == 0) {
            cout << "Error árbol no creado, usar opción 1" << endl;
            system("pause");
            opc = 99;
        }
        if (archivoGuardado && opc != 0) { // Solo permite salir si el archivo ha sido guardado
            cout << "El árbol ha sido guardado. Solo puede salir del programa (opción 0)." << endl;
            system("pause");
            continue;
        }
        
        switch (opc) {
            case 1:
                ArbolInt.Obtencion();
                bandera = 1;
                break;
            case 2:
                ArbolInt.Otronodo();
                break;
            case 3:
                ArbolInt.Borrarnodo();
                break;
            case 4:
                cout << "Indica el nodo a buscar: ";
                cin >> nn;
                cout << endl;
                b1 = ArbolInt.Buscar(nn);
                if (b1 == 1) {
                    cout << "Encontré el nodo " << nn << endl;
                } else {
                    cout << "No encontre el nodo " << nn << endl;
                }
                break;
            case 5:
                SubmenuRecorridos();
                break;
            case 6:
                cout << "Altura de árbol " << ArbolInt.AlturaArbol() << endl;
                break;
            case 7:
                cout << "Indica el nodo a buscar: ";
                cin >> nn;
                an = ArbolInt.Altura(nn);
                if (an != -1) {
                    cout << "Altura del nodo " << nn << " es " << an << endl;
                } else {
                    cout << "El nodo " << nn << " no existe" << endl;
                }
                break;
            case 8:
                cout << "Número de nodos: " << ArbolInt.NumeroNodos() << endl;
                break;
            case 9:
                ArbolInt.CreaSalida();
                archivoGuardado = true;
                break;
            case 0:
                cout << "Termina proceso" << endl;
                break;
            default:
                cout << "Opción incorrecta" << endl;
        }
    } while (opc != 0);
    
    return 0;
}
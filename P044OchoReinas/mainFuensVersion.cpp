// Autores: Edna Martha Miranda Chávez
// Sergio Fuenlabrada Velázquez
// P044- Problema de las 8 reinas

#include <chrono>
#include <clocale>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <locale>

using namespace std;

constexpr int N_MINIMO = 4;
constexpr int N_MAXIMO = 14;

enum class ModoVisualizacion {
    TODAS = 1,
    CANTIDAD_ESPECIFICA = 2,
    SOLO_ESTADISTICAS = 3
};

struct Configuracion {
    int n = 8;
    ModoVisualizacion modo = ModoVisualizacion::TODAS;
    unsigned long long limiteMostrar = 0;
};

struct Estadisticas {
    unsigned long long solucionesEncontradas = 0;
    unsigned long long solucionesMostradas = 0;
    unsigned long long posicionesEvaluadas = 0;
    unsigned long long posicionesDescartadas = 0;
    unsigned long long retrocesos = 0;
};

int leerEnteroEnRango(const string& mensaje, int minimo, int maximo) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= minimo && valor <= maximo) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        cout << "Entrada inválida. Capture un número entero entre "
             << minimo << " y " << maximo << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

unsigned long long leerEnteroPositivo(const string& mensaje) {
    unsigned long long valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        cout << "Entrada inválida. Capture un número entero mayor que cero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Configuracion solicitarConfiguracion() {
    Configuracion configuracion;
    cout << "========================================\n";
    cout << " PROBLEMA DE LAS N REINAS\n";
    cout << "========================================\n";
    cout << "El programa coloca una reina por fila sin repetir columnas\n";
    cout << "ni permitir conflictos en diagonales.\n\n";

    configuracion.n = leerEnteroEnRango(
        "Ingrese el tamaño del tablero (4 a 14): ",
        N_MINIMO,
        N_MAXIMO
    );
    
    cout << "\nSeleccione el modo de salida:\n";
    cout << "1. Mostrar todas las soluciones\n";
    cout << "2. Mostrar una cantidad específica\n";
    cout << "3. No mostrar tableros; presentar solo estadísticas\n";
    
    const int opcion = leerEnteroEnRango("Opción: ", 1, 3);
    configuracion.modo = static_cast<ModoVisualizacion>(opcion);
    
    if (configuracion.modo == ModoVisualizacion::CANTIDAD_ESPECIFICA) {
        configuracion.limiteMostrar = leerEnteroPositivo(
            "Número máximo de soluciones que desea mostrar: "
        );
    }
    return configuracion;
}

bool debeMostrarSolucion(const Configuracion& configuracion, const Estadisticas& estadisticas) {
    switch (configuracion.modo) {
        case ModoVisualizacion::TODAS:
            return true;
        case ModoVisualizacion::CANTIDAD_ESPECIFICA:
            return estadisticas.solucionesMostradas < configuracion.limiteMostrar;
        case ModoVisualizacion::SOLO_ESTADISTICAS:
            return false;
    }
    return false;
}

void imprimirTablero(const vector<int>& columnaPorFila, unsigned long long numeroSolucion) {
    const int n = static_cast<int>(columnaPorFila.size());
    const int ancho = static_cast<int>(to_string(n - 1).size()) + 1;

    cout << "\nSolución #" << numeroSolucion << ":\n\n";
    cout << setw(ancho + 2) << " ";
    for (int columna = 0; columna < n; ++columna) {
        cout << setw(ancho) << columna;
    }
    cout << "\n" << setw(ancho + 2) << " " << "+";
    for (int columna = 0; columna < n; ++columna) {
        cout << string(ancho, '-');
    }
    cout << "+\n";
    
    for (int fila = 0; fila < n; ++fila) {
        cout << setw(ancho) << fila << " |";
        for (int columna = 0; columna < n; ++columna) {
            cout << setw(ancho) << (columnaPorFila[fila] == columna ? "Q" : ".");
        }
        cout << "|\n";
    }
    
    cout << setw(ancho + 2) << " " << "+";
    for (int columna = 0; columna < n; ++columna) {
        cout << string(ancho, '-');
    }
    cout << "+\n";
}

void colocarReinas(int fila,
                    vector<int>& columnaPorFila,
                    vector<bool>& columnsOcupadas,
                    vector<bool>& diagonalesPrincipalesOcupadas,
                    vector<bool>& diagonalesSecundariasOcupadas,
                    const Configuracion& configuracion,
                    Estadisticas& estadisticas) {
    const int n = configuracion.n;
    if (fila == n) {
        ++estadisticas.solucionesEncontradas;

        if (debeMostrarSolucion(configuracion, estadisticas)) {
            imprimirTablero(columnaPorFila, estadisticas.solucionesEncontradas);
            ++estadisticas.solucionesMostradas;
        }
        return;
    }
    
    bool seColocoReinaEnLaFila = false;
    for (int columna = 0; columna < n; ++columna) {
        ++estadisticas.posicionesEvaluadas;
        const int diagonalPrincipal = fila - columna + n - 1;
        const int diagonalSecundaria = fila + columna;
        
        const bool hayConflicto =
            columnsOcupadas[columna] ||
            diagonalesPrincipalesOcupadas[diagonalPrincipal] ||
            diagonalesSecundariasOcupadas[diagonalSecundaria];
            
        if (hayConflicto) {
            ++estadisticas.posicionesDescartadas;
            continue;
        }
        
        seColocoReinaEnLaFila = true;
        columnaPorFila[fila] = columna;
        columnsOcupadas[columna] = true;
        diagonalesPrincipalesOcupadas[diagonalPrincipal] = true;
        diagonalesSecundariasOcupadas[diagonalSecundaria] = true;
        
        colocarReinas(
            fila + 1,
            columnaPorFila,
            columnsOcupadas,
            diagonalesPrincipalesOcupadas,
            diagonalesSecundariasOcupadas,
            configuracion,
            estadisticas
        );
        
        columnaPorFila[fila] = -1;
        columnsOcupadas[columna] = false;
        diagonalesPrincipalesOcupadas[diagonalPrincipal] = false;
        diagonalesSecundariasOcupadas[diagonalSecundaria] = false;
        ++estadisticas.retrocesos;
    }
    if (!seColocoReinaEnLaFila) {
        ++estadisticas.retrocesos;
    }
}

void mostrarEstadisticas(const Configuracion& configuracion,
                         const Estadisticas& estadisticas,
                         double tiempoMilisegundos) {
    cout << "\n========================================\n";
    cout << " ESTADÍSTICAS DEL PROCESO\n";
    cout << "========================================\n";
    cout << "Tamaño del tablero: " << configuracion.n << " x " << configuracion.n << '\n';
    cout << "Soluciones encontradas: " << estadisticas.solucionesEncontradas << '\n';
    cout << "Soluciones mostradas: " << estadisticas.solucionesMostradas << '\n';
    cout << "Posiciones evaluadas: " << estadisticas.posicionesEvaluadas << '\n';
    cout << "Posiciones descartadas: " << estadisticas.posicionesDescartadas << '\n';
    cout << "Operaciones de retroceso: " << estadisticas.retrocesos << '\n';
    cout << fixed << setprecision(3);
    cout << "Tiempo de ejecución: " << tiempoMilisegundos << " ms\n";
    cout << "Complejidad temporal teórica: O(n!)\n";
    cout << "Complejidad espacial: O(n)\n";
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "es_MX.UTF-8");
    
    const Configuracion configuracion = solicitarConfiguracion();
    Estadisticas estadisticas;
    
    vector<int> columnaPorFila(configuracion.n, -1);
    vector<bool> columnsOcupadas(configuracion.n, false);
    vector<bool> diagonalesPrincipalesOcupadas(2 * configuracion.n - 1, false);
    vector<bool> diagonalesSecundariasOcupadas(2 * configuracion.n - 1, false);
    
    cout << "\nIniciando búsqueda mediante backtracking optimizado...\n";
    const auto inicio = chrono::steady_clock::now();
    
    colocarReinas(
        0,
        columnaPorFila,
        columnsOcupadas,
        diagonalesPrincipalesOcupadas,
        diagonalesSecundariasOcupadas,
        configuracion,
        estadisticas
    );
    
    const auto fin = chrono::steady_clock::now();
    const double tiempoMilisegundos = chrono::duration<double, milli>(fin - inicio).count();
    
    mostrarEstadisticas(configuracion, estadisticas, tiempoMilisegundos);
    return 0;
}
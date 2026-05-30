#include "AcademicNetwork.h"
#include "Benchmark.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;

/*
    En CLion/Windows a veces system("cls") no redibuja bien el menu
    y deja la pantalla cortada. Por eso aqui NO usamos cls.
    Solo separamos visualmente las pantallas para que siempre se vea
    el menu completo en la consola de CLion.
*/
static void limpiarPantalla() {
    cout << "\n\n\n";
}

static void esperarTecla() {
    cout << "\nPresione una tecla para volver al menu...";

#ifdef _WIN32
    _getch();
    cout << "\n";
#else
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

static void mostrarEncabezado() {
    cout << "\n";
    cout << "***********************************************************************\n";
    cout << "*              RONY ALEXANDER BARRIOS  9941 99 127                    *\n";
    cout << "*              RED SOCIAL ACADEMICA UNIVERSITARIA - C++               *\n";
    cout << "*                                                                     *\n";
    cout << "***********************************************************************\n";
    cout << "*  Proyecto Final: Estructuras de Datos Manuales y Benchmark Comparativo  *\n";
    cout << "***********************************************************************\n";
}

static void mostrarMenuPrincipal() {
    limpiarPantalla();

    cout << "\n";
    cout << "                           MENU PRINCIPAL DEL SISTEMA\n";
    cout << "\n";
    cout << "      +---------------------------------+   +---------------------------------+\n";
    cout << "      |        MODULO ESTUDIANTES       |   |         MODULO RED / AVL        |\n";
    cout << "      +---------------------------------+   +---------------------------------+\n";
    cout << "      | [ 1] Cargar estudiantes CSV     |   | [ 7] Conectar estudiantes       |\n";
    cout << "      | [ 2] Registrar estudiante       |   | [ 8] Verificar conexion         |\n";
    cout << "      | [ 3] Buscar estudiante por ID   |   | [ 9] Mostrar conexiones         |\n";
    cout << "      | [ 4] Eliminar estudiante        |   | [10] Recorrer red con BFS       |\n";
    cout << "      | [ 5] Agregar proyecto           |   | [11] Recorrer red con DFS       |\n";
    cout << "      | [ 6] Mostrar proyectos          |   | [12] Opciones del AVL           |\n";
    cout << "      +---------------------------------+   +---------------------------------+\n";
    cout << "\n";
    cout << "      +---------------------------------+   +---------------------------------+\n";
    cout << "      |          ESTADISTICAS           |   |          HERRAMIENTAS           |\n";
    cout << "      +---------------------------------+   +---------------------------------+\n";
    cout << "      | [13] Estadisticas estructuras   |   | [14] Generar benchmark C++      |\n";
    cout << "      |                                 |   | [16] Ruta mas corta [MEJORA]    |\n";
    cout << "      +---------------------------------+   +---------------------------------+\n";
    cout << "\n";
    cout << "                          +-------------------------+\n";
    cout << "                          | [15] Salir del sistema  |\n";
    cout << "                          +-------------------------+\n";
    cout << "\n";
    cout << "  -----------------------------------------------------------------------------\n";
    cout << "  Nota: la opcion 16 es una mejora avanzada adicional.\n";
    cout << "  Seleccione una opcion del menu [1-16]: ";
}

static bool leerEntero(int& opcion) {
    cin >> opcion;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        opcion = 0;
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

int main() {
    AcademicNetwork redAcademica;
    int opcion = 0;

    while (true) {
        mostrarMenuPrincipal();

        if (!leerEntero(opcion)) {
            limpiarPantalla();
            mostrarEncabezado();
            cout << "\nOpcion invalida. Debe ingresar un numero del 1 al 16.\n";
            esperarTecla();
            continue;
        }

        if (opcion == 15) {
            limpiarPantalla();
            mostrarEncabezado();
            cout << "\nSaliendo del sistema.\n";
            break;
        }

        if (opcion == 12) {
            limpiarPantalla();
            redAcademica.manageAvlOptions();

            /*
                Al salir del modulo AVL con opcion 4,
                se regresa al inicio del while y se vuelve a dibujar
                el menu principal completo.
            */
            continue;
        }

        limpiarPantalla();
        mostrarEncabezado();
        cout << "\n";

        switch (opcion) {
            case 1: {
                string rutaArchivo;

                cout << "Ruta del CSV [ENTER para usar ./dataset/estudiantes.csv]: ";
                getline(cin, rutaArchivo);

                if (rutaArchivo.empty()) {
                    rutaArchivo = "./dataset/estudiantes.csv";
                }

                redAcademica.loadStudentsFromCSV(rutaArchivo);
                esperarTecla();
                break;
            }

            case 2:
                redAcademica.registerStudentManual();
                esperarTecla();
                break;

            case 3:
                redAcademica.searchStudentById();
                esperarTecla();
                break;

            case 4:
                redAcademica.removeStudentById();
                esperarTecla();
                break;

            case 5:
                redAcademica.addProjectToStudent();
                esperarTecla();
                break;

            case 6:
                redAcademica.manageProjectsOfStudent();
                esperarTecla();
                break;

            case 7:
                redAcademica.connectStudents();
                esperarTecla();
                break;

            case 8:
                redAcademica.verifyConnection();
                esperarTecla();
                break;

            case 9:
                redAcademica.showConnections();
                esperarTecla();
                break;

            case 10:
                redAcademica.bfs();
                esperarTecla();
                break;

            case 11:
                redAcademica.dfs();
                esperarTecla();
                break;

            case 13:
                redAcademica.showStats();
                esperarTecla();
                break;

            case 14:
                generateCppBenchmark("./evidencias/resultados/cpp_results.csv", 10000);
                esperarTecla();
                break;

            case 16:
                redAcademica.findShortestPath();
                esperarTecla();
                break;

            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
                esperarTecla();
                break;
        }
    }

    return 0;
}
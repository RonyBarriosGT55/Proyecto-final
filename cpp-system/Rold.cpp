#include "AcademicNetwork.h"
#include "Benchmark.h"
#include <iostream>
#include <string>
using namespace std;

static void showMenu() {
    cout << "\n====================================================\n";
    cout << " RED SOCIAL ACADEMICA UNIVERSITARIA - C++\n";
    cout << "====================================================\n";
    cout << "1. Cargar estudiantes desde CSV\n";
    cout << "2. Registrar estudiante manualmente\n";
    cout << "3. Buscar estudiante por ID\n";
    cout << "4. Eliminar estudiante\n";
    cout << "5. Agregar proyecto a estudiante\n";
    cout << "6. Mostrar proyectos de estudiante\n";
    cout << "7. Conectar estudiantes\n";
    cout << "8. Verificar si dos estudiantes estan conectados\n";
    cout << "9. Mostrar conexiones de estudiante\n";
    cout << "10. Recorrer red con BFS\n";
    cout << "11. Recorrer red con DFS\n";
    cout << "12. Mostrar ranking academico\n";
    cout << "13. Mostrar estadisticas de estructuras\n";
    cout << "14. Generar benchmark C++\n";
    cout << "15. [MEJORA] Ruta mas corta entre estudiantes\n";
    cout << "16. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    AcademicNetwork redAcademica;
    int opcion = 0;

    do {
        showMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string rutaArchivo;
                cout << "Ruta del CSV [ENTER para usar ../dataset/estudiantes.csv]: ";
                cin.ignore();
                getline(cin, rutaArchivo);
                if (rutaArchivo.empty()) rutaArchivo = "../dataset/estudiantes.csv";
                redAcademica.loadStudentsFromCSV(rutaArchivo);
                break;
            }
            case 2:
                redAcademica.registerStudentManual();
                break;
            case 3:
                redAcademica.searchStudentById();
                break;
            case 4:
                redAcademica.removeStudentById();
                break;
            case 5:
                redAcademica.addProjectToStudent();
                break;
            case 6:
                redAcademica.manageProjectsOfStudent();
                break;
            case 7:
                redAcademica.connectStudents();
                break;
            case 8:
                redAcademica.verifyConnection();
                break;
            case 9:
                redAcademica.showConnections();
                break;
            case 10:
                redAcademica.bfs();
                break;
            case 11:
                redAcademica.dfs();
                break;
            case 12:
                redAcademica.showRanking();
                break;
            case 13:
                redAcademica.showStats();
                break;
            case 14:
                generateCppBenchmark("../evidencias/resultados/cpp_results.csv", 10000);
                break;
            case 15:
                redAcademica.findShortestPath();
                break;
            case 16:
                cout << "Saliendo del sistema.\n";
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
                break;
        }
    } while (opcion != 16);

    return 0;
}
#include "AcademicNetwork.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;
using namespace std::chrono;

static void pausarModuloAVL() {
    cout << "\nPresione una tecla para continuar en el modulo AVL...";

#ifdef _WIN32
    _getch();
    cout << "\n";
#else
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

AcademicNetwork::AcademicNetwork()
    : indicePorId(20011),
      grafoConexiones(20011),
      totalProyectosRed(0),
      tiempoCargaDatasetMs(0.0) {}

AcademicNetwork::~AcademicNetwork() {
    clearAll();
}

void AcademicNetwork::deleteProjects(Student* student) {
    if (student == nullptr) return;

    ProjectNode* current = student->listaProyectos;

    while (current != nullptr) {
        ProjectNode* temp = current;
        current = current->siguienteProyecto;
        delete temp;
    }

    student->listaProyectos = nullptr;
    student->cantidadProyectos = 0;
}

void AcademicNetwork::clearAll() {
    arbolDeRanking.clearNodesOnly();
    grafoConexiones.clear();
    indicePorId.clearAndDeleteStudents();
    pilaHistorial.clear();

    totalProyectosRed = 0;
    tiempoCargaDatasetMs = 0.0;
}

bool AcademicNetwork::parseStudentLine(const string& line, Student*& student) const {
    if (line.empty()) return false;

    size_t p1 = line.find(',');
    if (p1 == string::npos) return false;

    size_t p2 = line.find(',', p1 + 1);
    if (p2 == string::npos) return false;

    size_t p3 = line.find(',', p2 + 1);
    if (p3 == string::npos) return false;

    size_t p4 = line.find(',', p3 + 1);
    if (p4 == string::npos) return false;

    size_t p5 = line.find(',', p4 + 1);
    if (p5 == string::npos) return false;

    string idText = line.substr(0, p1);
    string name = line.substr(p1 + 1, p2 - p1 - 1);
    string career = line.substr(p2 + 1, p3 - p2 - 1);
    string semesterText = line.substr(p3 + 1, p4 - p3 - 1);
    string gpaText = line.substr(p4 + 1, p5 - p4 - 1);
    string scoreText = line.substr(p5 + 1);

    int idEstudiante = atoi(idText.c_str());
    int semestreActual = atoi(semesterText.c_str());
    float promedioAcumulado = static_cast<float>(atof(gpaText.c_str()));
    int puntajeHabilidad = atoi(scoreText.c_str());

    if (idEstudiante <= 0 || name.empty() || career.empty() || semestreActual <= 0) {
        return false;
    }

    student = new Student(
        idEstudiante,
        name,
        career,
        semestreActual,
        promedioAcumulado,
        puntajeHabilidad
    );

    return true;
}

bool AcademicNetwork::addStudentInternal(Student* student, bool registerAction) {
    if (student == nullptr) return false;

    if (indicePorId.search(student->idEstudiante) != nullptr) {
        return false;
    }

    bool insertedHash = indicePorId.insert(student);

    if (!insertedHash) {
        return false;
    }

    arbolDeRanking.insert(student);
    grafoConexiones.addStudent(student->idEstudiante);

    if (registerAction) {
        pilaHistorial.push(
            "Estudiante registrado: ID " + to_string(student->idEstudiante)
        );
    }

    return true;
}

bool AcademicNetwork::loadStudentsFromCSV(const string& path) {
    ifstream file(path.c_str());
    string finalPath = path;

    if (!file.is_open()) {
        finalPath = "dataset/estudiantes.csv";
        file.open(finalPath.c_str());
    }

    if (!file.is_open()) {
        finalPath = "../dataset/estudiantes.csv";
        file.open(finalPath.c_str());
    }

    if (!file.is_open()) {
        finalPath = "../../dataset/estudiantes.csv";
        file.open(finalPath.c_str());
    }

    if (!file.is_open()) {
        cout << "No se pudo abrir el CSV. Revise la ruta indicada.\n";
        return false;
    }

    clearAll();

    auto tiempoInicio = high_resolution_clock::now();

    string line;
    int loaded = 0;
    int rejected = 0;

    getline(file, line);

    while (getline(file, line)) {
        Student* student = nullptr;

        if (parseStudentLine(line, student)) {
            if (addStudentInternal(student, false)) {
                loaded++;
            } else {
                deleteProjects(student);
                delete student;
                rejected++;
            }
        } else {
            rejected++;
        }
    }

    auto tiempoFin = high_resolution_clock::now();

    tiempoCargaDatasetMs = duration<double, milli>(tiempoFin - tiempoInicio).count();

    file.close();

    pilaHistorial.push(
        "Carga masiva CSV: " + to_string(loaded) + " estudiantes desde " + finalPath
    );

    cout << "Carga finalizada. Estudiantes cargados: " << loaded
         << " | Rechazados: " << rejected
         << " | Tiempo: " << fixed << setprecision(4)
         << tiempoCargaDatasetMs << " ms\n";

    return loaded > 0;
}

bool AcademicNetwork::registerStudentManual() {
    int idEstudiante;
    int semestreActual;
    int puntajeHabilidad;
    float promedioAcumulado;

    string nombreCompleto;
    string carreraInscrita;

    cout << "ID del estudiante: ";
    cin >> idEstudiante;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (indicePorId.search(idEstudiante) != nullptr) {
        cout << "Error: ya existe un estudiante con ese ID.\n";
        return false;
    }

    cout << "Nombre completo: ";
    getline(cin, nombreCompleto);

    cout << "Carrera: ";
    getline(cin, carreraInscrita);

    cout << "Semestre: ";
    cin >> semestreActual;

    cout << "GPA/promedio: ";
    cin >> promedioAcumulado;

    cout << "Skill score: ";
    cin >> puntajeHabilidad;

    if (idEstudiante <= 0 ||
        nombreCompleto.empty() ||
        carreraInscrita.empty() ||
        semestreActual <= 0) {
        cout << "Datos invalidos. No se registro el estudiante.\n";
        return false;
    }

    Student* student = new Student(
        idEstudiante,
        nombreCompleto,
        carreraInscrita,
        semestreActual,
        promedioAcumulado,
        puntajeHabilidad
    );

    if (!addStudentInternal(student, true)) {
        delete student;
        cout << "No se pudo registrar el estudiante.\n";
        return false;
    }

    cout << "Estudiante registrado correctamente.\n";
    return true;
}

void AcademicNetwork::searchStudentById() const {
    int idEstudiante;

    cout << "ID a buscar: ";
    cin >> idEstudiante;

    Student* student = indicePorId.search(idEstudiante);

    if (student == nullptr) {
        cout << "Estudiante no encontrado.\n";
        return;
    }

    cout << "ID: " << student->idEstudiante << endl;
    cout << "Nombre: " << student->nombreCompleto << endl;
    cout << "Carrera: " << student->carreraInscrita << endl;
    cout << "Semestre: " << student->semestreActual << endl;
    cout << "GPA: " << student->promedioAcumulado << endl;
    cout << "Skill score: " << student->puntajeHabilidad << endl;
    cout << "Proyectos registrados: " << student->cantidadProyectos << endl;
}

bool AcademicNetwork::removeStudentById() {
    int idEstudiante;

    cout << "ID a eliminar: ";
    cin >> idEstudiante;

    Student* student = indicePorId.search(idEstudiante);

    if (student == nullptr) {
        cout << "No existe ese estudiante.\n";
        return false;
    }

    totalProyectosRed -= student->cantidadProyectos;

    arbolDeRanking.remove(student);
    grafoConexiones.removeStudent(idEstudiante);

    Student* removed = indicePorId.remove(idEstudiante);

    if (removed != nullptr) {
        deleteProjects(removed);
        delete removed;
    }

    pilaHistorial.push(
        "Estudiante eliminado: ID " + to_string(idEstudiante)
    );

    cout << "Estudiante eliminado correctamente en Hash, AVL, Grafo y proyectos asociados.\n";

    return true;
}

ProjectNode* AcademicNetwork::findProjectNode(Student* student, int idProyecto) const {
    if (student == nullptr) return nullptr;

    ProjectNode* current = student->listaProyectos;

    while (current != nullptr) {
        if (current->idProyecto == idProyecto) {
            return current;
        }

        current = current->siguienteProyecto;
    }

    return nullptr;
}

bool AcademicNetwork::addProjectToStudent() {
    int idEstudiante;
    int idProyecto;
    int anioFinalizacion;

    string tituloProyecto;
    string description;

    cout << "ID del estudiante: ";
    cin >> idEstudiante;

    Student* student = indicePorId.search(idEstudiante);

    if (student == nullptr) {
        cout << "Estudiante no encontrado.\n";
        return false;
    }

    cout << "ID del proyecto: ";
    cin >> idProyecto;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (findProjectNode(student, idProyecto) != nullptr) {
        cout << "Error: ese project_id ya existe para este estudiante.\n";
        return false;
    }

    cout << "Titulo: ";
    getline(cin, tituloProyecto);

    cout << "Descripcion: ";
    getline(cin, description);

    cout << "Anio de finalizacion: ";
    cin >> anioFinalizacion;

    ProjectNode* project = new ProjectNode(
        idProyecto,
        tituloProyecto,
        description,
        anioFinalizacion
    );

    project->siguienteProyecto = student->listaProyectos;
    student->listaProyectos = project;
    student->cantidadProyectos++;
    totalProyectosRed++;

    pilaHistorial.push(
        "Proyecto agregado: estudiante ID " + to_string(idEstudiante) +
        ", proyecto ID " + to_string(idProyecto)
    );

    cout << "Proyecto agregado correctamente.\n";

    return true;
}

void AcademicNetwork::manageProjectsOfStudent() {
    int idEstudiante;

    cout << "ID del estudiante: ";
    cin >> idEstudiante;

    Student* student = indicePorId.search(idEstudiante);

    if (student == nullptr) {
        cout << "Estudiante no encontrado.\n";
        return;
    }

    int opcion;

    cout << "\n========== MODULO DE PROYECTOS ==========\n";
    cout << "1. Mostrar todos los proyectos\n";
    cout << "2. Buscar proyecto por ID\n";
    cout << "3. Eliminar proyecto por ID\n";
    cout << "4. Regresar al menu principal\n";
    cout << "Seleccione opcion: ";
    cin >> opcion;

    if (opcion == 1) {
        if (student->listaProyectos == nullptr) {
            cout << "El estudiante no tiene proyectos.\n";
            return;
        }

        ProjectNode* current = student->listaProyectos;

        while (current != nullptr) {
            cout << "Proyecto ID: " << current->idProyecto << endl;
            cout << "Titulo: " << current->tituloProyecto << endl;
            cout << "Descripcion: " << current->description << endl;
            cout << "Anio: " << current->anioFinalizacion << endl;
            cout << "-----------------------------\n";

            current = current->siguienteProyecto;
        }

    } else if (opcion == 2) {
        int idProyecto;

        cout << "ID del proyecto: ";
        cin >> idProyecto;

        ProjectNode* project = findProjectNode(student, idProyecto);

        if (project == nullptr) {
            cout << "Proyecto no encontrado.\n";
            return;
        }

        cout << "Proyecto ID: " << project->idProyecto << endl;
        cout << "Titulo: " << project->tituloProyecto << endl;
        cout << "Descripcion: " << project->description << endl;
        cout << "Anio: " << project->anioFinalizacion << endl;

    } else if (opcion == 3) {
        int idProyecto;

        cout << "ID del proyecto a eliminar: ";
        cin >> idProyecto;

        ProjectNode* current = student->listaProyectos;
        ProjectNode* previous = nullptr;

        while (current != nullptr) {
            if (current->idProyecto == idProyecto) {
                if (previous == nullptr) {
                    student->listaProyectos = current->siguienteProyecto;
                } else {
                    previous->siguienteProyecto = current->siguienteProyecto;
                }

                delete current;
                student->cantidadProyectos--;
                totalProyectosRed--;

                pilaHistorial.push(
                    "Proyecto eliminado: estudiante ID " + to_string(idEstudiante) +
                    ", proyecto ID " + to_string(idProyecto)
                );

                cout << "Proyecto eliminado correctamente.\n";
                return;
            }

            previous = current;
            current = current->siguienteProyecto;
        }

        cout << "Proyecto no encontrado.\n";

    } else if (opcion == 4) {
        cout << "Regresando al menu principal.\n";

    } else {
        cout << "Opcion invalida.\n";
    }
}

bool AcademicNetwork::connectStudents() {
    int id1;
    int id2;

    cout << "ID del primer estudiante: ";
    cin >> id1;

    cout << "ID del segundo estudiante: ";
    cin >> id2;

    if (id1 == id2) {
        cout << "No se permite conectar un estudiante consigo mismo.\n";
        return false;
    }

    Student* estudiante1 = indicePorId.search(id1);
    Student* estudiante2 = indicePorId.search(id2);

    if (estudiante1 == nullptr && estudiante2 == nullptr) {
        cout << "No existen los estudiantes con ID " << id1 << " y " << id2 << ".\n";
        cout << "Primero debe cargarlos desde el CSV o registrarlos manualmente.\n";
        return false;
    }

    if (estudiante1 == nullptr) {
        cout << "No existe el primer estudiante con ID " << id1 << ".\n";
        cout << "Primero debe cargarlo desde el CSV o registrarlo manualmente.\n";
        return false;
    }

    if (estudiante2 == nullptr) {
        cout << "No existe el segundo estudiante con ID " << id2 << ".\n";
        cout << "Primero debe cargarlo desde el CSV o registrarlo manualmente.\n";
        return false;
    }

    if (!grafoConexiones.connectStudents(id1, id2)) {
        cout << "No se pudo conectar. Puede ser una conexion duplicada.\n";
        return false;
    }

    pilaHistorial.push(
        "Conexion creada: " + to_string(id1) + " <-> " + to_string(id2)
    );

    cout << "Conexion creada correctamente.\n";
    cout << "Estudiante " << id1 << " conectado con estudiante " << id2 << ".\n";

    return true;
}

void AcademicNetwork::verifyConnection() const {
    int id1;
    int id2;

    cout << "ID del primer estudiante: ";
    cin >> id1;

    cout << "ID del segundo estudiante: ";
    cin >> id2;

    if (grafoConexiones.areConnected(id1, id2)) {
        cout << "Si estan conectados directamente.\n";
    } else {
        cout << "No estan conectados directamente.\n";
    }
}

void AcademicNetwork::showConnections() const {
    int idEstudiante;

    cout << "ID del estudiante: ";
    cin >> idEstudiante;

    grafoConexiones.showConnections(idEstudiante);
}

void AcademicNetwork::bfs() {
    int idEstudiante;

    cout << "ID inicial para BFS: ";
    cin >> idEstudiante;

    grafoConexiones.bfs(idEstudiante);
}

void AcademicNetwork::dfs() {
    int idEstudiante;

    cout << "ID inicial para DFS: ";
    cin >> idEstudiante;

    grafoConexiones.dfs(idEstudiante);
}

void AcademicNetwork::showRanking() const {
    arbolDeRanking.showRanking();
}

void AcademicNetwork::manageAvlOptions() const {
    int opcionAVL = 0;

    do {
        cout << "\n";
        cout << "=========================================\n";
        cout << "              MODULO AVL                 \n";
        cout << "=========================================\n";
        cout << "1. Mostrar ranking academico por skill_score\n";
        cout << "2. Buscar estudiantes por skill_score\n";
        cout << "3. Mostrar recorrido InOrden del AVL\n";
        cout << "4. Regresar al menu principal\n";
        cout << "=========================================\n";
        cout << "Seleccione opcion: ";

        cin >> opcionAVL;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nOpcion invalida. Ingrese un numero del 1 al 4.\n";
            pausarModuloAVL();
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcionAVL) {
            case 1:
                cout << "\n--- RANKING ACADEMICO AVL ---\n";
                arbolDeRanking.showRanking();
                pausarModuloAVL();
                break;

            case 2: {
                int score;

                cout << "\nIngrese el skill_score a buscar: ";
                cin >> score;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nScore invalido.\n";
                    pausarModuloAVL();
                    break;
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\n--- BUSQUEDA POR SKILL_SCORE ---\n";
                arbolDeRanking.searchByScore(score);
                pausarModuloAVL();
                break;
            }

            case 3:
                cout << "\n--- RECORRIDO INORDEN DEL AVL ---\n";
                arbolDeRanking.showInOrder();
                pausarModuloAVL();
                break;

            case 4:
                cout << "\nRegresando al menu principal.\n";
                break;

            default:
                cout << "\nOpcion invalida del modulo AVL. Ingrese un numero del 1 al 4.\n";
                pausarModuloAVL();
                break;
        }

    } while (opcionAVL != 4);
}

void AcademicNetwork::showStats() const {
    cout << fixed << setprecision(4);

    cout << "\n========== ESTADISTICAS ==========" << endl;
    cout << "Total de estudiantes cargados: " << indicePorId.size() << endl;
    cout << "Total de conexiones creadas: " << grafoConexiones.getEdgeCount() << endl;
    cout << "Total de proyectos registrados: " << totalProyectosRed << endl;
    cout << "Total de colisiones hash: " << indicePorId.getCollisions() << endl;
    cout << "Factor de carga hash: " << indicePorId.loadFactor() << endl;
    cout << "Altura actual del AVL: " << arbolDeRanking.getHeight() << endl;
    cout << "Tiempo total de carga del dataset: " << tiempoCargaDatasetMs << " ms" << endl;
    cout << "Acciones en historial/pila: " << pilaHistorial.size() << endl;
    cout << "==================================\n";
}

void AcademicNetwork::showHistory() const {
    pilaHistorial.show();
}

void AcademicNetwork::findShortestPath() {
    int id1;
    int id2;

    cout << "\n--- RUTA MAS CORTA (GRADOS DE SEPARACION) ---" << endl;

    cout << "ID del estudiante origen: ";
    cin >> id1;

    cout << "ID del estudiante destino: ";
    cin >> id2;

    grafoConexiones.shortestPath(id1, id2);
}
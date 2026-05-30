#include "Benchmark.h"
#include "HashTable.h"
#include "AVL.h"
#include "Graph.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;
using namespace std::chrono;

static Student* createBenchmarkStudent(int i) {
    int idEstudiante = 100000 + i;
    string nombreCompleto = "Benchmark Student " + to_string(i);
    string carreraInscrita = "Ingenieria en Sistemas";
    int semestreActual = (i % 10) + 1;
    float promedioAcumulado = 60.0f + (float)(i % 4000) / 100.0f;
    int puntajeHabilidad = (i * 37) % 100000;
    return new Student(idEstudiante, nombreCompleto, carreraInscrita, semestreActual, promedioAcumulado, puntajeHabilidad);
}

static double elapsedMs(high_resolution_clock::time_point tInicio, high_resolution_clock::time_point tFin) {
    return duration<double, milli>(tFin - tInicio).count();
}

void generateCppBenchmark(const string& outputPath, int numeroRegistros) {
    Student** arrayEst = new Student*[numeroRegistros];
    for (int i = 0; i < numeroRegistros; i++) {
        arrayEst[i] = createBenchmarkStudent(i + 1);
    }

    string finalOutputPath = outputPath;
    ofstream file(finalOutputPath.c_str());
    if (!file.is_open()) {
        finalOutputPath = "../../evidencias/resultados/cpp_results.csv";
        file.open(finalOutputPath.c_str());
    }
    if (!file.is_open()) {
        finalOutputPath = "../evidencias/resultados/cpp_results.csv";
        file.open(finalOutputPath.c_str());
    }
    if (!file.is_open()) {
        finalOutputPath = "evidencias/resultados/cpp_results.csv";
        file.open(finalOutputPath.c_str());
    }
    if (!file.is_open()) {
        finalOutputPath = "cpp_results.csv";
        file.open(finalOutputPath.c_str());
    }
    if (!file.is_open()) {
        cout << "No se pudo crear el archivo de benchmark.\n";
        for (int i = 0; i < numeroRegistros; i++) delete arrayEst[i];
        delete[] arrayEst;
        return;
    }

    file << "language,operation,structure,records,time_ms\n";
    file << fixed << setprecision(4);

    // HashTable insert/search sobre carga limpia.
    HashTable indicePorId(numeroRegistros * 2 + 101);
    auto tiempoInicio = high_resolution_clock::now();
    for (int i = 0; i < numeroRegistros; i++) {
        indicePorId.insert(arrayEst[i]);
    }
    auto tiempoFin = high_resolution_clock::now();
    file << "C++,insert,HashTable," << numeroRegistros << "," << elapsedMs(tiempoInicio, tiempoFin) << "\n";

    tiempoInicio = high_resolution_clock::now();
    int found = 0;
    for (int i = 0; i < numeroRegistros; i++) {
        if (indicePorId.search(arrayEst[i]->idEstudiante) != nullptr) found++;
    }
    tiempoFin = high_resolution_clock::now();
    file << "C++,search,HashTable," << numeroRegistros << "," << elapsedMs(tiempoInicio, tiempoFin) << "\n";

    indicePorId.clearNodesOnly();

    // AVL insert/traversal sobre carga limpia.
    AVL arbolDeRanking;
    tiempoInicio = high_resolution_clock::now();
    for (int i = 0; i < numeroRegistros; i++) {
        arbolDeRanking.insert(arrayEst[i]);
    }
    tiempoFin = high_resolution_clock::now();
    file << "C++,insert,AVL," << numeroRegistros << "," << elapsedMs(tiempoInicio, tiempoFin) << "\n";

    tiempoInicio = high_resolution_clock::now();
    int visitedAvl = arbolDeRanking.inOrderCount();
    tiempoFin = high_resolution_clock::now();
    file << "C++,traversal,AVL," << numeroRegistros << "," << elapsedMs(tiempoInicio, tiempoFin) << "\n";
    arbolDeRanking.clearNodesOnly();

    // Graph BFS/DFS sobre carga limpia.
    Graph grafoConexiones(numeroRegistros * 2 + 101);
    for (int i = 0; i < numeroRegistros; i++) {
        grafoConexiones.addStudent(arrayEst[i]->idEstudiante);
    }
    for (int i = 0; i < numeroRegistros - 1; i++) {
        grafoConexiones.connectStudents(arrayEst[i]->idEstudiante, arrayEst[i + 1]->idEstudiante);
    }

    tiempoInicio = high_resolution_clock::now();
    int bfsVisited = grafoConexiones.bfsCount(arrayEst[0]->idEstudiante);
    tiempoFin = high_resolution_clock::now();
    file << "C++,bfs,Graph," << numeroRegistros << "," << elapsedMs(tiempoInicio, tiempoFin) << "\n";

    tiempoInicio = high_resolution_clock::now();
    int dfsVisited = grafoConexiones.dfsCount(arrayEst[0]->idEstudiante);
    tiempoFin = high_resolution_clock::now();
    file << "C++,dfs,Graph," << numeroRegistros << "," << elapsedMs(tiempoInicio, tiempoFin) << "\n";

    file.close();

    grafoConexiones.clear();

    for (int i = 0; i < numeroRegistros; i++) {
        delete arrayEst[i];
    }
    delete[] arrayEst;

    cout << "Benchmark C++ generado en: " << finalOutputPath << endl;
    cout << "Control interno: Hash encontrados=" << found
         << ", AVL visitados=" << visitedAvl
         << ", BFS visitados=" << bfsVisited
         << ", DFS visitados=" << dfsVisited << endl;
}
#ifndef ACADEMICNETWORK_H
#define ACADEMICNETWORK_H

#include "ActionStack.h"
#include "AVL.h"
#include "Graph.h"
#include "HashTable.h"

#include <string>

using namespace std;

class AcademicNetwork {
private:
    HashTable indicePorId;
    AVL arbolDeRanking;
    Graph grafoConexiones;
    ActionStack pilaHistorial;
    int totalProyectosRed;
    double tiempoCargaDatasetMs;

    bool parseStudentLine(const string& line, Student*& student) const;
    void deleteProjects(Student* student);
    bool addStudentInternal(Student* student, bool registerAction);
    ProjectNode* findProjectNode(Student* student, int idProyecto) const;

public:
    AcademicNetwork();
    ~AcademicNetwork();

    void clearAll();

    bool loadStudentsFromCSV(const string& path);
    bool registerStudentManual();
    void searchStudentById() const;
    bool removeStudentById();

    bool addProjectToStudent();
    void manageProjectsOfStudent();

    bool connectStudents();
    void verifyConnection() const;
    void showConnections() const;

    void bfs();
    void dfs();

    void showRanking() const;
    void manageAvlOptions() const;

    void showStats() const;
    void showHistory() const;

    void findShortestPath();
};

#endif
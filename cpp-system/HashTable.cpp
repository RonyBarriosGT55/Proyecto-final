#include "HashTable.h"
#include <iostream>
using namespace std;

static void deleteProjectListForStudent(Student* student) {
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

HashTable::HashTable(int initialCapacity) {
    capacidadTabla = initialCapacity;
    totalEstudiantesGuardados = 0;
    contadorColisiones = 0;
    arregloDeListas = new HashNode*[capacidadTabla];
    for (int i = 0; i < capacidadTabla; i++) {
        arregloDeListas[i] = nullptr;
    }
}

HashTable::~HashTable() {
    clearNodesOnly();
    delete[] arregloDeListas;
}

int HashTable::hashFunction(int idEstudiante) const {
    int value = idEstudiante % capacidadTabla;
    if (value < 0) value += capacidadTabla;
    return value;
}

bool HashTable::insert(Student* student) {
    if (student == nullptr) return false;

    int index = hashFunction(student->idEstudiante);
    HashNode* current = arregloDeListas[index];

    while (current != nullptr) {
        if (current->student->idEstudiante == student->idEstudiante) {
            return false;
        }
        current = current->siguienteNodoHash;
    }

    if (arregloDeListas[index] != nullptr) {
        contadorColisiones++;
    }

    HashNode* newNode = new HashNode(student);
    newNode->siguienteNodoHash = arregloDeListas[index];
    arregloDeListas[index] = newNode;
    totalEstudiantesGuardados++;
    return true;
}

Student* HashTable::search(int idEstudiante) const {
    int index = hashFunction(idEstudiante);
    HashNode* current = arregloDeListas[index];

    while (current != nullptr) {
        if (current->student->idEstudiante == idEstudiante) {
            return current->student;
        }
        current = current->siguienteNodoHash;
    }
    return nullptr;
}

Student* HashTable::remove(int idEstudiante) {
    int index = hashFunction(idEstudiante);
    HashNode* current = arregloDeListas[index];
    HashNode* previous = nullptr;

    while (current != nullptr) {
        if (current->student->idEstudiante == idEstudiante) {
            if (previous == nullptr) {
                arregloDeListas[index] = current->siguienteNodoHash;
            } else {
                previous->siguienteNodoHash = current->siguienteNodoHash;
            }
            Student* removed = current->student;
            delete current;
            totalEstudiantesGuardados--;
            return removed;
        }
        previous = current;
        current = current->siguienteNodoHash;
    }
    return nullptr;
}

void HashTable::clearNodesOnly() {
    for (int i = 0; i < capacidadTabla; i++) {
        HashNode* current = arregloDeListas[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->siguienteNodoHash;
            delete temp;
        }
        arregloDeListas[i] = nullptr;
    }
    totalEstudiantesGuardados = 0;
    contadorColisiones = 0;
}

void HashTable::clearAndDeleteStudents() {
    for (int i = 0; i < capacidadTabla; i++) {
        HashNode* current = arregloDeListas[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->siguienteNodoHash;
            deleteProjectListForStudent(temp->student);
            delete temp->student;
            delete temp;
        }
        arregloDeListas[i] = nullptr;
    }
    totalEstudiantesGuardados = 0;
    contadorColisiones = 0;
}

int HashTable::size() const { return totalEstudiantesGuardados; }
int HashTable::getCapacity() const { return capacidadTabla; }
long HashTable::getCollisions() const { return contadorColisiones; }
double HashTable::loadFactor() const {
    if (capacidadTabla == 0) return 0.0;
    return (double)totalEstudiantesGuardados / (double)capacidadTabla;
}
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Student.h"

struct HashNode {
    Student* student;
    HashNode* siguienteNodoHash;

    HashNode(Student* s) : student(s), siguienteNodoHash(nullptr) {}
};

class HashTable {
private:
    HashNode** arregloDeListas;
    int capacidadTabla;
    int totalEstudiantesGuardados;
    long contadorColisiones;

    int hashFunction(int idEstudiante) const;

public:
    explicit HashTable(int initialCapacity = 20011);
    ~HashTable();

    bool insert(Student* student);
    Student* search(int idEstudiante) const;
    Student* remove(int idEstudiante);
    void clearNodesOnly();
    void clearAndDeleteStudents();

    int size() const;
    int getCapacity() const;
    long getCollisions() const;
    double loadFactor() const;
};

#endif
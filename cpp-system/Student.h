#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

struct ProjectNode {
    int idProyecto;
    string tituloProyecto;
    string description;
    int anioFinalizacion;
    ProjectNode* siguienteProyecto;

    ProjectNode(int id, const string& t, const string& d, int year)
        : idProyecto(id), tituloProyecto(t), description(d), anioFinalizacion(year), siguienteProyecto(nullptr) {}
};

struct Student {
    int idEstudiante;
    string nombreCompleto;
    string carreraInscrita;
    int semestreActual;
    float promedioAcumulado;
    int puntajeHabilidad;
    ProjectNode* listaProyectos;
    int cantidadProyectos;

    Student()
        : idEstudiante(0), semestreActual(0), promedioAcumulado(0.0f), puntajeHabilidad(0), listaProyectos(nullptr), cantidadProyectos(0) {}

    Student(int id, const string& name, const string& car, int sem, float avg, int score)
        : idEstudiante(id), nombreCompleto(name), carreraInscrita(car), semestreActual(sem), promedioAcumulado(avg),
          puntajeHabilidad(score), listaProyectos(nullptr), cantidadProyectos(0) {}
};

#endif
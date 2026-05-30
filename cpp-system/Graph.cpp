#include "Graph.h"
#include <iostream>
using namespace std;

struct QueueNode {
    int value;
    QueueNode* next;
    QueueNode(int v) : value(v), next(nullptr) {}
};

class IntQueue {
private:
    QueueNode* nodoFrente;
    QueueNode* nodoFinal;
public:
    IntQueue() : nodoFrente(nullptr), nodoFinal(nullptr) {}
    ~IntQueue() { while (!isEmpty()) dequeue(); }
    bool isEmpty() const { return nodoFrente == nullptr; }
    void enqueue(int value) {
        QueueNode* node = new QueueNode(value);
        if (nodoFinal == nullptr) {
            nodoFrente = nodoFinal = node;
        } else {
            nodoFinal->next = node;
            nodoFinal = node;
        }
    }
    int dequeue() {
        if (nodoFrente == nullptr) return -1;
        QueueNode* temp = nodoFrente;
        int value = temp->value;
        nodoFrente = nodoFrente->next;
        if (nodoFrente == nullptr) nodoFinal = nullptr;
        delete temp;
        return value;
    }
};

struct StackNodeInt {
    int value;
    StackNodeInt* next;
    StackNodeInt(int v) : value(v), next(nullptr) {}
};

class IntStackLocal {
private:
    StackNodeInt* nodoCima;
public:
    IntStackLocal() : nodoCima(nullptr) {}
    ~IntStackLocal() { while (!isEmpty()) pop(); }
    bool isEmpty() const { return nodoCima == nullptr; }
    void push(int value) {
        StackNodeInt* node = new StackNodeInt(value);
        node->next = nodoCima;
        nodoCima = node;
    }
    int pop() {
        if (nodoCima == nullptr) return -1;
        StackNodeInt* temp = nodoCima;
        int value = temp->value;
        nodoCima = nodoCima->next;
        delete temp;
        return value;
    }
};

Graph::Graph(int initialCapacity) {
    capacidadTabla = initialCapacity;
    cantidadVertices = 0;
    cantidadAristas = 0;
    verticesDelGrafo = new GraphVertex*[capacidadTabla];
    for (int i = 0; i < capacidadTabla; i++) verticesDelGrafo[i] = nullptr;
}

Graph::~Graph() { clear(); delete[] verticesDelGrafo; }

int Graph::hashFunction(int idEstudiante) const {
    int value = idEstudiante % capacidadTabla;
    if (value < 0) value += capacidadTabla;
    return value;
}

GraphVertex* Graph::findVertex(int idEstudiante) const {
    int index = hashFunction(idEstudiante);
    GraphVertex* current = verticesDelGrafo[index];
    while (current != nullptr) {
        if (current->idEstudiante == idEstudiante) return current;
        current = current->siguienteVerticeHash;
    }
    return nullptr;
}

bool Graph::adjacencyExists(GraphVertex* vertex, int target_id) const {
    if (vertex == nullptr) return false;
    AdjNode* current = vertex->listaAdyacencia;
    while (current != nullptr) {
        if (current->idEstudiante == target_id) return true;
        current = current->siguienteAdyacencia;
    }
    return false;
}

void Graph::addAdjacency(GraphVertex* vertex, int target_id) {
    AdjNode* node = new AdjNode(target_id);
    node->siguienteAdyacencia = vertex->listaAdyacencia;
    vertex->listaAdyacencia = node;
}

bool Graph::removeAdjacency(GraphVertex* vertex, int target_id) {
    if (vertex == nullptr) return false;
    AdjNode* current = vertex->listaAdyacencia;
    AdjNode* previous = nullptr;
    while (current != nullptr) {
        if (current->idEstudiante == target_id) {
            if (previous == nullptr) vertex->listaAdyacencia = current->siguienteAdyacencia;
            else previous->siguienteAdyacencia = current->siguienteAdyacencia;
            delete current;
            return true;
        }
        previous = current;
        current = current->siguienteAdyacencia;
    }
    return false;
}

int Graph::degree(GraphVertex* vertex) const {
    int total = 0;
    AdjNode* current = vertex == nullptr ? nullptr : vertex->listaAdyacencia;
    while (current != nullptr) { total++; current = current->siguienteAdyacencia; }
    return total;
}

void Graph::clearSearchData() {
    for (int i = 0; i < capacidadTabla; i++) {
        GraphVertex* current = verticesDelGrafo[i];
        while (current != nullptr) {
            current->fueVisitado = false;
            current->idPadreRecorrido = -1;
            current = current->siguienteVerticeHash;
        }
    }
}

bool Graph::addStudent(int idEstudiante) {
    if (findVertex(idEstudiante) != nullptr) return false;
    int index = hashFunction(idEstudiante);
    GraphVertex* vertex = new GraphVertex(idEstudiante);
    vertex->siguienteVerticeHash = verticesDelGrafo[index];
    verticesDelGrafo[index] = vertex;
    cantidadVertices++;
    return true;
}

bool Graph::removeStudent(int idEstudiante) {
    int index = hashFunction(idEstudiante);
    GraphVertex* current = verticesDelGrafo[index];
    GraphVertex* previous = nullptr;
    while (current != nullptr && current->idEstudiante != idEstudiante) {
        previous = current;
        current = current->siguienteVerticeHash;
    }
    if (current == nullptr) return false;

    int removedEdges = degree(current);
    for (int i = 0; i < capacidadTabla; i++) {
        GraphVertex* v = verticesDelGrafo[i];
        while (v != nullptr) {
            if (v->idEstudiante != idEstudiante) removeAdjacency(v, idEstudiante);
            v = v->siguienteVerticeHash;
        }
    }

    AdjNode* adj = current->listaAdyacencia;
    while (adj != nullptr) {
        AdjNode* temp = adj;
        adj = adj->siguienteAdyacencia;
        delete temp;
    }

    if (previous == nullptr) verticesDelGrafo[index] = current->siguienteVerticeHash;
    else previous->siguienteVerticeHash = current->siguienteVerticeHash;

    delete current;
    cantidadVertices--;
    cantidadAristas -= removedEdges;
    if (cantidadAristas < 0) cantidadAristas = 0;
    return true;
}

bool Graph::connectStudents(int id1, int id2) {
    if (id1 == id2) return false;
    GraphVertex* v1 = findVertex(id1);
    GraphVertex* v2 = findVertex(id2);
    if (v1 == nullptr || v2 == nullptr) return false;
    if (adjacencyExists(v1, id2)) return false;

    addAdjacency(v1, id2);
    addAdjacency(v2, id1);
    cantidadAristas++;
    return true;
}

bool Graph::areConnected(int id1, int id2) const {
    GraphVertex* v1 = findVertex(id1);
    if (v1 == nullptr) return false;
    return adjacencyExists(v1, id2);
}

void Graph::showConnections(int idEstudiante) const {
    GraphVertex* vertex = findVertex(idEstudiante);
    if (vertex == nullptr) { cout << "El estudiante no existe en el grafo.\n"; return; }
    cout << "Conexiones de " << idEstudiante << ": ";
    if (vertex->listaAdyacencia == nullptr) { cout << "sin conexiones"; }
    else {
        AdjNode* current = vertex->listaAdyacencia;
        while (current != nullptr) {
            cout << current->idEstudiante;
            if (current->siguienteAdyacencia != nullptr) cout << " -> ";
            current = current->siguienteAdyacencia;
        }
    }
    cout << endl;
}

void Graph::bfs(int start_id) {
    GraphVertex* start = findVertex(start_id);
    if (start == nullptr) { cout << "No existe el estudiante inicial.\n"; return; }
    clearSearchData();
    IntQueue q;
    start->fueVisitado = true;
    q.enqueue(start_id);
    cout << "BFS desde " << start_id << ": ";
    while (!q.isEmpty()) {
        int currentId = q.dequeue();
        cout << currentId << " ";
        GraphVertex* currentVertex = findVertex(currentId);
        AdjNode* adj = currentVertex == nullptr ? nullptr : currentVertex->listaAdyacencia;
        while (adj != nullptr) {
            GraphVertex* neighbor = findVertex(adj->idEstudiante);
            if (neighbor != nullptr && !neighbor->fueVisitado) {
                neighbor->fueVisitado = true;
                q.enqueue(neighbor->idEstudiante);
            }
            adj = adj->siguienteAdyacencia;
        }
    }
    cout << endl;
}

void Graph::dfs(int start_id) {
    GraphVertex* start = findVertex(start_id);
    if (start == nullptr) { cout << "No existe el estudiante inicial.\n"; return; }
    clearSearchData();
    IntStackLocal stack;
    stack.push(start_id);
    cout << "DFS desde " << start_id << ": ";
    while (!stack.isEmpty()) {
        int currentId = stack.pop();
        GraphVertex* currentVertex = findVertex(currentId);
        if (currentVertex == nullptr || currentVertex->fueVisitado) continue;

        currentVertex->fueVisitado = true;
        cout << currentId << " ";
        AdjNode* adj = currentVertex->listaAdyacencia;
        while (adj != nullptr) {
            GraphVertex* neighbor = findVertex(adj->idEstudiante);
            if (neighbor != nullptr && !neighbor->fueVisitado) stack.push(neighbor->idEstudiante);
            adj = adj->siguienteAdyacencia;
        }
    }
    cout << endl;
}

int Graph::bfsCount(int start_id) {
    GraphVertex* start = findVertex(start_id);
    if (start == nullptr) return 0;
    clearSearchData();
    IntQueue q;
    int visitedCount = 0;
    start->fueVisitado = true;
    q.enqueue(start_id);
    while (!q.isEmpty()) {
        int currentId = q.dequeue();
        visitedCount++;
        GraphVertex* currentVertex = findVertex(currentId);
        AdjNode* adj = currentVertex == nullptr ? nullptr : currentVertex->listaAdyacencia;
        while (adj != nullptr) {
            GraphVertex* neighbor = findVertex(adj->idEstudiante);
            if (neighbor != nullptr && !neighbor->fueVisitado) {
                neighbor->fueVisitado = true;
                q.enqueue(neighbor->idEstudiante);
            }
            adj = adj->siguienteAdyacencia;
        }
    }
    return visitedCount;
}

int Graph::dfsCount(int start_id) {
    GraphVertex* start = findVertex(start_id);
    if (start == nullptr) return 0;
    clearSearchData();
    IntStackLocal stack;
    int visitedCount = 0;
    stack.push(start_id);
    while (!stack.isEmpty()) {
        int currentId = stack.pop();
        GraphVertex* currentVertex = findVertex(currentId);
        if (currentVertex == nullptr || currentVertex->fueVisitado) continue;
        currentVertex->fueVisitado = true;
        visitedCount++;
        AdjNode* adj = currentVertex->listaAdyacencia;
        while (adj != nullptr) {
            GraphVertex* neighbor = findVertex(adj->idEstudiante);
            if (neighbor != nullptr && !neighbor->fueVisitado) stack.push(neighbor->idEstudiante);
            adj = adj->siguienteAdyacencia;
        }
    }
    return visitedCount;
}

void Graph::shortestPath(int start_id, int end_id) {
    GraphVertex* start = findVertex(start_id);
    GraphVertex* endNode = findVertex(end_id);
    if (start == nullptr || endNode == nullptr) {
        cout << "Uno o ambos estudiantes no existen en el grafo.\n"; return;
    }
    if (start_id == end_id) {
        cout << "El estudiante origen y destino son el mismo.\n"; return;
    }

    clearSearchData();
    IntQueue q;
    start->fueVisitado = true;
    q.enqueue(start_id);
    bool found = false;

    while (!q.isEmpty()) {
        int currentId = q.dequeue();
        if (currentId == end_id) { found = true; break; }
        GraphVertex* currentVertex = findVertex(currentId);
        AdjNode* adj = currentVertex == nullptr ? nullptr : currentVertex->listaAdyacencia;
        while (adj != nullptr) {
            GraphVertex* neighbor = findVertex(adj->idEstudiante);
            if (neighbor != nullptr && !neighbor->fueVisitado) {
                neighbor->fueVisitado = true;
                neighbor->idPadreRecorrido = currentId;
                q.enqueue(neighbor->idEstudiante);
            }
            adj = adj->siguienteAdyacencia;
        }
    }

    if (!found) { cout << "No existe una ruta de conexion.\n"; return; }
    IntStackLocal pathStack;
    int curr = end_id;
    int steps = -1;
    while (curr != -1) {
        pathStack.push(curr);
        steps++;
        if (curr == start_id) break;
        GraphVertex* v = findVertex(curr);
        curr = v->idPadreRecorrido;
    }
    cout << "Ruta mas corta (" << steps << " grados de separacion): ";
    while (!pathStack.isEmpty()) {
        cout << pathStack.pop();
        if (!pathStack.isEmpty()) cout << " -> ";
    }
    cout << endl;
}

void Graph::clear() {
    for (int i = 0; i < capacidadTabla; i++) {
        GraphVertex* vertex = verticesDelGrafo[i];
        while (vertex != nullptr) {
            AdjNode* adj = vertex->listaAdyacencia;
            while (adj != nullptr) {
                AdjNode* tempAdj = adj;
                adj = adj->siguienteAdyacencia;
                delete tempAdj;
            }
            GraphVertex* tempVertex = vertex;
            vertex = vertex->siguienteVerticeHash;
            delete tempVertex;
        }
        verticesDelGrafo[i] = nullptr;
    }
    cantidadVertices = 0;
    cantidadAristas = 0;
}

int Graph::getVertexCount() const { return cantidadVertices; }
int Graph::getEdgeCount() const { return cantidadAristas; }
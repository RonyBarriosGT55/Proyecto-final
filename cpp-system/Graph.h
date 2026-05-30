#ifndef GRAPH_H
#define GRAPH_H

struct AdjNode {
    int idEstudiante;
    AdjNode* siguienteAdyacencia;
    AdjNode(int id) : idEstudiante(id), siguienteAdyacencia(nullptr) {}
};

struct GraphVertex {
    int idEstudiante;
    AdjNode* listaAdyacencia;
    GraphVertex* siguienteVerticeHash;
    bool fueVisitado;
    int idPadreRecorrido;

    GraphVertex(int id) : idEstudiante(id), listaAdyacencia(nullptr), siguienteVerticeHash(nullptr), fueVisitado(false), idPadreRecorrido(-1) {}
};

class Graph {
private:
    GraphVertex** verticesDelGrafo;
    int capacidadTabla;
    int cantidadVertices;
    int cantidadAristas;

    int hashFunction(int idEstudiante) const;
    bool adjacencyExists(GraphVertex* vertex, int target_id) const;
    void addAdjacency(GraphVertex* vertex, int target_id);
    bool removeAdjacency(GraphVertex* vertex, int target_id);
    int degree(GraphVertex* vertex) const;
    void clearSearchData();

public:
    explicit Graph(int initialCapacity = 20011);
    ~Graph();

    bool addStudent(int idEstudiante);
    bool removeStudent(int idEstudiante);
    bool connectStudents(int id1, int id2);
    bool areConnected(int id1, int id2) const;
    void showConnections(int idEstudiante) const;
    void bfs(int start_id);
    void dfs(int start_id);
    int bfsCount(int start_id);
    int dfsCount(int start_id);
    void shortestPath(int start_id, int end_id);
    GraphVertex* findVertex(int idEstudiante) const;
    void clear();

    int getVertexCount() const;
    int getEdgeCount() const;
};

#endif
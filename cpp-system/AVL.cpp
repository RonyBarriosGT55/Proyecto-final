#include "AVL.h"
#include <iostream>
using namespace std;

AVL::AVL() : nodoRaiz(nullptr), cantidadNodosAVL(0) {}
AVL::~AVL() { clearNodesOnly(); }

int AVL::height(AVLNode* node) const { return node == nullptr ? 0 : node->height; }
int AVL::maxValue(int a, int b) const { return a > b ? a : b; }
int AVL::balanceFactor(AVLNode* node) const { return node == nullptr ? 0 : height(node->hijoIzquierdo) - height(node->hijoDerecho); }

bool AVL::isLess(Student* a, Student* b) const {
    if (a->puntajeHabilidad != b->puntajeHabilidad) {
        return a->puntajeHabilidad < b->puntajeHabilidad;
    }
    return a->idEstudiante < b->idEstudiante;
}

AVLNode* AVL::rotateRight(AVLNode* y) {
    AVLNode* x = y->hijoIzquierdo;
    AVLNode* t2 = x->hijoDerecho;
    x->hijoDerecho = y;
    y->hijoIzquierdo = t2;
    y->height = maxValue(height(y->hijoIzquierdo), height(y->hijoDerecho)) + 1;
    x->height = maxValue(height(x->hijoIzquierdo), height(x->hijoDerecho)) + 1;
    return x;
}

AVLNode* AVL::rotateLeft(AVLNode* x) {
    AVLNode* y = x->hijoDerecho;
    AVLNode* t2 = y->hijoIzquierdo;
    y->hijoIzquierdo = x;
    x->hijoDerecho = t2;
    x->height = maxValue(height(x->hijoIzquierdo), height(x->hijoDerecho)) + 1;
    y->height = maxValue(height(y->hijoIzquierdo), height(y->hijoDerecho)) + 1;
    return y;
}

AVLNode* AVL::insertRecursive(AVLNode* node, Student* student, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new AVLNode(student);
    }
    if (isLess(student, node->student)) {
        node->hijoIzquierdo = insertRecursive(node->hijoIzquierdo, student, inserted);
    } else if (isLess(node->student, student)) {
        node->hijoDerecho = insertRecursive(node->hijoDerecho, student, inserted);
    } else {
        return node;
    }

    node->height = 1 + maxValue(height(node->hijoIzquierdo), height(node->hijoDerecho));
    int balance = balanceFactor(node);

    if (balance > 1 && isLess(student, node->hijoIzquierdo->student)) return rotateRight(node);
    if (balance < -1 && isLess(node->hijoDerecho->student, student)) return rotateLeft(node);
    if (balance > 1 && isLess(node->hijoIzquierdo->student, student)) {
        node->hijoIzquierdo = rotateLeft(node->hijoIzquierdo);
        return rotateRight(node);
    }
    if (balance < -1 && isLess(student, node->hijoDerecho->student)) {
        node->hijoDerecho = rotateRight(node->hijoDerecho);
        return rotateLeft(node);
    }
    return node;
}

AVLNode* AVL::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current != nullptr && current->hijoIzquierdo != nullptr) current = current->hijoIzquierdo;
    return current;
}

AVLNode* AVL::removeRecursive(AVLNode* node, Student* student, bool& removed) {
    if (node == nullptr) return nullptr;

    if (isLess(student, node->student)) {
        node->hijoIzquierdo = removeRecursive(node->hijoIzquierdo, student, removed);
    } else if (isLess(node->student, student)) {
        node->hijoDerecho = removeRecursive(node->hijoDerecho, student, removed);
    } else {
        removed = true;
        if (node->hijoIzquierdo == nullptr || node->hijoDerecho == nullptr) {
            AVLNode* temp = node->hijoIzquierdo ? node->hijoIzquierdo : node->hijoDerecho;
            if (temp == nullptr) {
                delete node;
                return nullptr;
            } else {
                AVLNode* oldNode = node;
                node = temp;
                delete oldNode;
            }
        } else {
            AVLNode* temp = minValueNode(node->hijoDerecho);
            node->student = temp->student;
            bool dummy = false;
            node->hijoDerecho = removeRecursive(node->hijoDerecho, temp->student, dummy);
        }
    }

    if (node == nullptr) return node;

    node->height = 1 + maxValue(height(node->hijoIzquierdo), height(node->hijoDerecho));
    int balance = balanceFactor(node);

    if (balance > 1 && balanceFactor(node->hijoIzquierdo) >= 0) return rotateRight(node);
    if (balance > 1 && balanceFactor(node->hijoIzquierdo) < 0) {
        node->hijoIzquierdo = rotateLeft(node->hijoIzquierdo);
        return rotateRight(node);
    }
    if (balance < -1 && balanceFactor(node->hijoDerecho) <= 0) return rotateLeft(node);
    if (balance < -1 && balanceFactor(node->hijoDerecho) > 0) {
        node->hijoDerecho = rotateRight(node->hijoDerecho);
        return rotateLeft(node);
    }
    return node;
}

void AVL::clearRecursive(AVLNode* node) {
    if (node == nullptr) return;
    clearRecursive(node->hijoIzquierdo);
    clearRecursive(node->hijoDerecho);
    delete node;
}

void AVL::showRankingRecursive(AVLNode* node, int& position) const {
    if (node == nullptr) return;
    showRankingRecursive(node->hijoDerecho, position);
    cout << position << ". ID: " << node->student->idEstudiante
         << " | " << node->student->nombreCompleto
         << " | Score: " << node->student->puntajeHabilidad
         << " | GPA: " << node->student->promedioAcumulado << endl;
    position++;
    showRankingRecursive(node->hijoIzquierdo, position);
}

void AVL::showInOrderRecursive(AVLNode* node) const {
    if (node == nullptr) return;
    showInOrderRecursive(node->hijoIzquierdo);
    cout << "ID: " << node->student->idEstudiante << " | Score: " << node->student->puntajeHabilidad << " | " << node->student->nombreCompleto << endl;
    showInOrderRecursive(node->hijoDerecho);
}

void AVL::searchScoreRecursive(AVLNode* node, int score, bool& found) const {
    if (node == nullptr) return;
    if (score < node->student->puntajeHabilidad) {
        searchScoreRecursive(node->hijoIzquierdo, score, found);
    } else if (score > node->student->puntajeHabilidad) {
        searchScoreRecursive(node->hijoDerecho, score, found);
    } else {
        searchScoreRecursive(node->hijoIzquierdo, score, found);
        cout << "ID: " << node->student->idEstudiante << " | " << node->student->nombreCompleto << " | Score: " << node->student->puntajeHabilidad << endl;
        found = true;
        searchScoreRecursive(node->hijoDerecho, score, found);
    }
}

int AVL::inOrderCountRecursive(AVLNode* node) const {
    if (node == nullptr) return 0;
    return inOrderCountRecursive(node->hijoIzquierdo) + 1 + inOrderCountRecursive(node->hijoDerecho);
}

bool AVL::insert(Student* student) {
    bool inserted = false;
    nodoRaiz = insertRecursive(nodoRaiz, student, inserted);
    if (inserted) cantidadNodosAVL++;
    return inserted;
}

bool AVL::remove(Student* student) {
    if (student == nullptr) return false;
    bool removed = false;
    nodoRaiz = removeRecursive(nodoRaiz, student, removed);
    if (removed) cantidadNodosAVL--;
    return removed;
}

void AVL::clearNodesOnly() {
    clearRecursive(nodoRaiz);
    nodoRaiz = nullptr;
    cantidadNodosAVL = 0;
}

void AVL::showRanking() const {
    if (nodoRaiz == nullptr) { cout << "No hay estudiantes en el ranking.\n"; return; }
    int position = 1;
    showRankingRecursive(nodoRaiz, position);
}

void AVL::showInOrder() const {
    if (nodoRaiz == nullptr) { cout << "No hay datos en el AVL.\n"; return; }
    showInOrderRecursive(nodoRaiz);
}

void AVL::searchByScore(int score) const {
    bool found = false;
    searchScoreRecursive(nodoRaiz, score, found);
    if (!found) cout << "No se encontraron estudiantes con score " << score << ".\n";
}

int AVL::getHeight() const { return height(nodoRaiz); }
int AVL::size() const { return cantidadNodosAVL; }
int AVL::inOrderCount() const { return inOrderCountRecursive(nodoRaiz); }
#ifndef AVL_H
#define AVL_H

#include "Student.h"

struct AVLNode {
    Student* student;
    AVLNode* hijoIzquierdo;
    AVLNode* hijoDerecho;
    int height;

    AVLNode(Student* s) : student(s), hijoIzquierdo(nullptr), hijoDerecho(nullptr), height(1) {}
};

class AVL {
private:
    AVLNode* nodoRaiz;
    int cantidadNodosAVL;

    int height(AVLNode* node) const;
    int maxValue(int a, int b) const;
    int balanceFactor(AVLNode* node) const;
    bool isLess(Student* a, Student* b) const;

    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insertRecursive(AVLNode* node, Student* student, bool& inserted);
    AVLNode* removeRecursive(AVLNode* node, Student* student, bool& removed);
    AVLNode* minValueNode(AVLNode* node);
    void clearRecursive(AVLNode* node);
    void showRankingRecursive(AVLNode* node, int& position) const;
    void showInOrderRecursive(AVLNode* node) const;
    void searchScoreRecursive(AVLNode* node, int score, bool& found) const;
    int inOrderCountRecursive(AVLNode* node) const;

public:
    AVL();
    ~AVL();

    bool insert(Student* student);
    bool remove(Student* student);
    void clearNodesOnly();
    void showRanking() const;
    void showInOrder() const;
    void searchByScore(int score) const;
    int getHeight() const;
    int size() const;
    int inOrderCount() const;
};

#endif
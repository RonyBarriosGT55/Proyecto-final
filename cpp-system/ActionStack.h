#ifndef ACTIONSTACK_H
#define ACTIONSTACK_H
#include <string>
using namespace std;

struct ActionNode {
    string description;
    ActionNode* next;
    ActionNode(const string& text) : description(text), next(nullptr) {}
};

class ActionStack {
private:
    ActionNode* nodoCima;
    int cantidadAcciones;
public:
    ActionStack();
    ~ActionStack();
    void push(const string& description);
    bool pop(string& description);
    void show() const;
    void clear();
    int size() const;
};
#endif
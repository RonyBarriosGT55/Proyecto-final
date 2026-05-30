#include "ActionStack.h"
#include <iostream>
using namespace std;

ActionStack::ActionStack() : nodoCima(nullptr), cantidadAcciones(0) {}
ActionStack::~ActionStack() { clear(); }

void ActionStack::push(const string& description) {
    ActionNode* node = new ActionNode(description);
    node->next = nodoCima;
    nodoCima = node;
    cantidadAcciones++;
}

bool ActionStack::pop(string& description) {
    if (nodoCima == nullptr) return false;
    ActionNode* temp = nodoCima;
    description = temp->description;
    nodoCima = nodoCima->next;
    delete temp;
    cantidadAcciones--;
    return true;
}

void ActionStack::show() const {
    if (nodoCima == nullptr) { cout << "No hay acciones registradas.\n"; return; }
    cout << "Historial de acciones (pila):\n";
    ActionNode* current = nodoCima;
    int position = 1;
    while (current != nullptr) {
        cout << position << ". " << current->description << endl;
        current = current->next;
        position++;
    }
}

void ActionStack::clear() {
    while (nodoCima != nullptr) {
        ActionNode* temp = nodoCima;
        nodoCima = nodoCima->next;
        delete temp;
    }
    cantidadAcciones = 0;
}

int ActionStack::size() const { return cantidadAcciones; }
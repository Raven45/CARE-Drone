#include "Object.h"

Object::Object() {
    Updatable = true;
    Initialized = true;
}
Object::~Object(){}

Object::Object(bool Updatable) {
    
    this->Updatable =  Updatable;
    this->Initialized = true;
}

bool Object::DisableUpdate() {
    
    Updatable = false;
    return true;
}

bool Object::EnableUpdate() {
    
    Updatable = true;
    return true;
}

bool Object::IsUpdatable() {
    return Updatable;
}

bool Object::IsInitialized() {
    return Initialized;
}
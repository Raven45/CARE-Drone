#include "Object.h"

Object::Object()    { 
    State = ObjectStates::Initialized;
}

Object::~Object()   { 
    State = ObjectStates::Uninitialized; 
}

short unsigned int Object::GetState() {
    return State;
}
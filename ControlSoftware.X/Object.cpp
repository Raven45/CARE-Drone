/*******************************************************************************
  * Copyright 2016  Aaron Burns,
 *                  Joshua Donaway,
 *                  Matthew Love,
 *                  Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*******************************************************************************/

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

void Object::Delay(UnsignedInteger32 Time) {
    
    for (UnsignedInteger32 i = Time; i > 0; i--) { }
}
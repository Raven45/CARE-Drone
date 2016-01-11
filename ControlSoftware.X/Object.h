/*******************************************************************************
 * Copyright 2016 Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


 * Created on January 11, 2016, 9:42 AM
*******************************************************************************/

#ifndef OBJECT_H
#define	OBJECT_H

#include <map>
#include <exception>

/*******************************************************************************
 * Fundamental bitwise operations.
 * The following macros define behavior that allows for basic, rudimentary 
 * bitwise operations. These are not already implemented by a C/C++ compiler, 
 * as the smallest addressable unit is memory is a byte (8 bits). The definition
 * of these macros allows the control software to manipulate individual bits.
*******************************************************************************/
#define CheckBit(var, bit) (var >> bit) & 1
#define SetBit(var, bit) var |= 1 << bit
#define ClearBit(var, bit) var &= ~(1 << bit)
#define ToggleBit(var, bit) var ^= 1 << bit

#define UnsignedInteger32 unsigned int
#define SignedInteger32 signed int
#define UnsignedInteger16 unsigned short int
#define SignedInteger16 signed short int

/*******************************************************************************
 * Object class
 * The object class is a superclass of which all other classes in the flight 
 * computer?s software is inherited from. The purpose of having a superclass of 
 * this nature is that all declared objects could be self-contained in an object
 * list and references in an object list. This also allows for a way of 
 * providing global constants to all objects that are instantiated; all objects 
 * implicitly inherit the global constant. In addition, having a root superclass
 * brings a common blueprint to all classes used in the flight computer 
 * software. 
*******************************************************************************/
class Object {
    
public:
    
    Object();
    Object(bool Updatable);
    virtual ~Object() = 0;
    
    virtual bool Initialize() = 0;
    virtual bool Update() = 0;

    bool DisableUpdate();
    bool EnableUpdate();
    bool IsUpdatable();
    bool IsInitialized();
    
protected:
    bool Updatable;
    bool Initialized;
    
};

#endif	/* OBJECT_H */


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

 * Created on January 11, 2016, 11:24 AM
*******************************************************************************/

#ifndef SYSTEM_H
#define	SYSTEM_H

#include "Object.h"

#define foreach(var, array, size) for (unsigned int i = 0; i < size; i++)
#define Address unsigned char



/*******************************************************************************
 * System class
 * The System object is a singleton object (meaning that only one can exist in 
 * memory) that contains most of the actual working program for the flight 
 * computer. It contains lists of references to every HAL object, register, and 
 * input needed for the operation of the flight computer. 
*******************************************************************************/
class System {
    
    enum States {
        Standby,
        Run,
        Debug
    };
    
public:
    System();
    ~System();
    
    bool InitializeSystem();
    bool UpdateSystem();
    void Main();
    bool ClearToProceed();
    bool IsUSBAttached();
    
private:
    std::map<Address, int> Devices;
    std::map<Address, int> Inputs;
    UnsignedInteger16 State;
    
    void StandbyMain();
    void RunMain();
    void DebugMain();
};

#endif	/* SYSTEM_H */


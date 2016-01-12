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

#include "SPIDevice.h"
#include "Register.h"
#include "Map.h"

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
    ~System();
    
    bool InitializeSystem();
    bool UpdateSystem();
    void Main();
    bool ClearToProceed();
    bool IsUSBAttached();
    
    static System* GetInstance();
    
private:
    //Default constructor
    System();
    
    
    /***************************************************************************
     * Private Properties.
    ***************************************************************************/
    //Reference to the singleton object
    static System* Instance;
    
    //Array to all Devices.
    HAL::SPIDevice * Devices;
    UnsignedInteger8 NumDevices;
    
    //Array to all inputs
    //HAL::Input * Inputs;
    UnsignedInteger8 NumInputs;
    
    //Map to all registered registers.
    Map<std::string, HAL::Register> RegisterList;
    
    //Integer showing the state of the system.
    UnsignedInteger8 State;
    
    //Motor engagement safety
    bool Safety;
    
    //Flag for enabling debugging. This is not the same
    //as the DebugMode state.
    bool SystemDebugging;
    
    
    /***************************************************************************
     * Private methods
    ***************************************************************************/
    void StandbyMain();
    void RunMain();
    void DebugMain();
    
    void GoToState(UnsignedInteger16 State);
    
    bool CreateDevice(ADDRESS, short int Type);
    
    UnsignedInteger16 ReceiveCommand();
    bool ExecuteCommand(UnsignedInteger16 Command);
};

#endif	/* SYSTEM_H */


#include "System.h"

System* System::Instance = 0;

System::System() {
    
}

System::~System() {
    
}

bool System::InitializeSystem() {
    
    
    //Initialize devices
    for (unsigned int i = 0; i < NumDevices; i++) {
        Devices[i].Initialize();
    }
}

bool System::UpdateSystem() {
    
    //Update devices
    for (unsigned int i = 0; i < NumDevices; i++) {
        Devices[i].Update();
    }
    
    
}

void System::Main() {
    
    if (State == States::Debug) {
        DebugMain();
    }
    
    else if (State == States::Run) {
        RunMain();
    }
    
    else if (State == States::Standby) {
        StandbyMain();
    }
}

bool System::ClearToProceed() {
    return true;
}

bool System::IsUSBAttached() {
    return false;
}

System* System::GetInstance() {
    
    if (!Instance) {
        Instance = new System;
    }
    return Instance;
}

void System::StandbyMain() {
    
    UpdateSystem();
    
    if (IsUSBAttached()) {
        //GoToState(States::Debug);
    }
    else {
        
        if (Safety /*&& Throttle == 0*/) {
            Safety = false;
        }
        else if (!Safety /*&& Throttle > 0*/) {
            //GoToState(States::Standby);
        }
    }
}

void System::RunMain() {
    
    UpdateSystem();
}

void System::DebugMain() {
    
    UpdateSystem();
    UnsignedInteger16 Command = 0;
    //Command = ReceiveCommand();
    //ExecuteCommand(Command);
}
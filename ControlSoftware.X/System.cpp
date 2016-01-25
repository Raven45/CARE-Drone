#include "System.h"
#include "Altimeter.h"

#ifdef SYSTEM_IS_SINGLETON
System* System::Instance = 0;
#endif

System::System() {
    
}

System::~System() {
    
}

bool System::InitializeSystem() {
    
    //InitializeSystemClock();
    
    //Create device objects
    Devices.push_back(new HAL::Altimeter(ADDRESS_BAROMETER, &DeviceManager));
    Devices.push_back(new HAL::PWMC(ADDRESS_MOTOR_1, &DeviceManager));
    Devices.push_back(new HAL::PWMC(ADDRESS_MOTOR_2, &DeviceManager));
    Devices.push_back(new HAL::PWMC(ADDRESS_MOTOR_3, &DeviceManager));
    Devices.push_back(new HAL::PWMC(ADDRESS_MOTOR_4, &DeviceManager));
    Devices.push_back(new HAL::PWMC(ADDRESS_MOTOR_5, &DeviceManager));
    Devices.push_back(new HAL::PWMC(ADDRESS_MOTOR_6, &DeviceManager));
    
    //Initialize the usb bus.
    usb_init();
    
    //Initialize devices
    for (unsigned int i = 0; i < NumDevices; i++) {
        Devices[i]->Initialize();
    }
    
    //usb_init();
    return true;
}

bool System::UpdateSystem() {
    
    //Update devices
    for (unsigned int i = 0; i < NumDevices; i++) {
        Devices[i]->Update();
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

#ifdef SYSTEM_IS_SINGLETON
System* System::GetInstance() {
    
    if (!Instance) {
        
        try{
            Instance = new System();
        }
        catch (std::bad_alloc) {
            return 0;
        }
    }
    return Instance;
}
#endif

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
    const unsigned char * Command = ReceiveCommand();
    ExecuteCommand(Command);
}


void System::GoToState(UnsignedInteger16 State) {
    this->State = State;
}
    
bool System::CreateDevice(ADDRESS, short int Type) {
    
}
    
const unsigned char * System::ReceiveCommand() {
    
    //Receive data from host
    if (usb_is_configured() &&
        !usb_out_endpoint_halted(2) &&
        usb_out_endpoint_has_data(2)) {
        
        const unsigned char * Incoming;
        
        if (usb_get_out_buffer(2, &Incoming) <= 0) {
            //Empty response condition
            usb_arm_out_endpoint(2);
            return 0;
        }
        else {
            return Incoming;
        }
    }
    
    else {
        return 0;
    }
}

bool System::ExecuteCommand(const unsigned char * Command) {
    
    
    std::string strCommand((const char*)Command);
    
    if (strCommand.rfind("ReadGyroscope") != std::string::npos) { 
        Command_ReadGyroscope(strCommand);
    }
    else if (strCommand.rfind("ReadAccelerometer") != std::string::npos) { 
        Command_ReadAccelerometer(strCommand);
    }
    else if (strCommand.rfind("GetGravity") != std::string::npos) { 
        Command_GetGravity();
    }
    else if (strCommand.rfind("GetPressure") != std::string::npos) { 
        Command_GetPressure();
    }
    else if (strCommand.rfind("GetTemperature") != std::string::npos) { 
        Command_GetTemperature();
    }
    else if (strCommand.rfind("GetAltitude") != std::string::npos) { 
        Command_GetAltitude();
    }
    else if (strCommand.rfind("SetThrottle") != std::string::npos) { 
        Command_SetThrottle(strCommand);
    }
    else if (strCommand.rfind("GetThrottle") != std::string::npos) { 
        Command_GetThrottle(strCommand);
    }
    else if (strCommand.rfind("StopAllMotors") != std::string::npos) { 
        Command_StopAllMotors();
    }
    else if (strCommand.rfind("SetAllMotors") != std::string::npos) { 
        Command_SetAllMotors(strCommand);
    }
    else if (strCommand.rfind("ReleaseCargo") != std::string::npos) { 
        Command_StopAllMotors();
    }
    else if (strCommand.rfind("HoldCargo") != std::string::npos) { 
        Command_StopAllMotors();
    }
    else if (strCommand.rfind("ReturnToStandby") != std::string::npos) { 
        Command_ReturnToStandby();
    }
}

bool System::SendUSBData(std::string Message) {
    
    if (usb_is_configured() &&
        !usb_in_endpoint_halted(2) &&
        !usb_in_endpoint_busy(2)) {
        
        unsigned char * cMessage = (unsigned char *)Message.c_str();
        cMessage = usb_get_in_buffer(2);
        
        usb_send_in_buffer(2, Message.length());
        return true;
    }
    else {
        return false;
    }
}

bool System::Command_ReadGyroscope(std::string Command) {
    
    if (Command.rfind("/X") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Y") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Z") != std::string::npos) {
            //Send X axis gyro reading
    }
}

bool System::Command_ReadAccelerometer(std::string Command) {
    
    if (Command.rfind("/X") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Y") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Z") != std::string::npos) {
            //Send X axis gyro reading
    }
}

bool System::Command_GetGravity() {
    
}

bool System::Command_GetPressure() {
    
}

bool System::Command_GetTemperature() {
    
}

bool System::Command_GetAltitude() {
    
}

bool System::Command_SetThrottle(std::string Command) {
    
    if (Command.rfind("/C:") != std::string::npos) {
            //Get channel.
    }
    if (Command.rfind("/D:") != std::string::npos) {
            //Get throttle and then set the throttle.
    }
}

bool System::Command_GetThrottle(std::string Command) {
    
    if (Command.rfind("/C:") != std::string::npos) {
            //Get channel.
    }
    if (Command.rfind("/D:") != std::string::npos) {
            //Get throttle and then set the throttle.
    }
}

bool System::Command_StopAllMotors() {
    
}

bool System::Command_SetAllMotors(std::string Command) {
    
    if (Command.rfind("/D:") != std::string::npos) {
            //Get throttle and then set the throttle.
    }
}

bool System::Command_ReleaseCargo() {
    
    PORTSetBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
    
    //delay()
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
}

bool System::Command_HoldCargo() {
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTSetBits(IOPORT_C, BIT_5);
    
    //delay()
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
}

bool System::Command_ReturnToStandby() {
    GoToState(States::Standby);
}
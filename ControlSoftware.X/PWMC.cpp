#include "PWMC.h"

HAL::PWMC::PWMC(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
}

HAL::PWMC::~PWMC() {
    
}
    
bool HAL::PWMC::Initialize() {
    //MotorIsStarted = false;
}

bool HAL::PWMC::Update() {
         
    //Set throttle
    UnsignedInteger16 ThrottleCommand = ((SET_SPEED << 8) | Throttle);
    UnsignedInteger16 Incoming = SendAndReceive(ThrottleCommand);     
}
    
UnsignedInteger8 HAL::PWMC::GetThrottle() {
    return Throttle;
}

void HAL::PWMC::SetThrottle(UnsignedInteger8 Throttle) {
    this->Throttle = Throttle;
}
    
void HAL::PWMC::StartMotor() {
    SendAndReceive(START_MOTOR << 8);
}

void HAL::PWMC::StopMotor() {
    SendAndReceive(STOP_MOTOR << 8);
}
    
UnsignedInteger8 HAL::PWMC::GetKp() {
    Kp = SendAndReceive(GET_KP << 8);
}

UnsignedInteger8 HAL::PWMC::GetKi() {
    Ki = SendAndReceive(GET_KI << 8);
}

UnsignedInteger8 HAL::PWMC::GetKd() {
    Kd = SendAndReceive(GET_KD << 8);
}

void HAL::PWMC::SetKp(UnsignedInteger8 Kp) {
    SendAndReceive((SET_KP << 8) | Kp );
}

void HAL::PWMC::SetKi(UnsignedInteger8 Ki) {
    SendAndReceive((SET_KI << 8) | Ki );
}

void HAL::PWMC::SetKd(UnsignedInteger8 Kd) {
    SendAndReceive((SET_KD << 8) | Kd );
}

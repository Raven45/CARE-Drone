#include "LCD.h"

HAL::LCD::LCD(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
}

HAL::LCD::~LCD() {
    
}
    
bool HAL::LCD::Initialize() {
    
}

bool HAL::LCD::Update() {
    
}
    
bool HAL::LCD::PrintString(std::string Message) {
    
    UnsignedInteger16 * Incoming = SendAndReceiveBurst(0, Message.length());
    if (Incoming = 0) {
        return false;
    }
    else {
        return true;
    }
}

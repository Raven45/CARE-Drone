#include "Transceiver.h"

HAL::Transceiver::Transceiver(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
}

HAL::Transceiver::~Transceiver() {
    
}

bool HAL::Transceiver::Initialize() {
    
}

bool HAL::Transceiver::Update() {
    
}

bool HAL::Transceiver::LoadBuffer(  Math::Quaternion Orientation,
                                    SignedInteger32 Altitude,
                                    UnsignedInteger16 Motor1Speed,
                                    UnsignedInteger16 Motor2Speed,
                                    UnsignedInteger16 Motor3Speed,
                                    UnsignedInteger16 Motor4Speed,
                                    UnsignedInteger16 Motor5Speed,
                                    UnsignedInteger16 Motor6Speed) {
    
    
}

bool HAL::Transceiver::Tansmit() {
    
}

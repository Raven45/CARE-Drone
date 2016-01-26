#include "Gyroscope.h"

HAL::Gyroscope::Gyroscope(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
    this->Parity = ParityTypes::OddParity;
}

HAL::Gyroscope::~Gyroscope(){
    
    
}
    
bool HAL::Gyroscope::Initialize() {
    
}

bool HAL::Gyroscope::Update() {
    
    UnsignedInteger16 Commands[6] = {
        (GYRO_RATE_X << 8),
        ((GYRO_RATE_X +1) << 8),
        (GYRO_RATE_Y << 8),
        ((GYRO_RATE_Y +1) << 8),
        (GYRO_RATE_Z << 8),
        ((GYRO_RATE_Z +1) << 8),
    };
    UnsignedInteger16 * Incoming = SendAndReceiveBurst(Commands, 6);
    
    RateX = (Incoming[1] << 8) | Incoming[0];
    RateY = (Incoming[3] << 8) | Incoming[2];
    RateZ = (Incoming[5] << 8) | Incoming[4];
}
    
UnsignedInteger16 HAL::Gyroscope::GetRateX() {
    return RateX;
}

UnsignedInteger16 HAL::Gyroscope::GetRateY() {
    return RateY;
}

UnsignedInteger16 HAL::Gyroscope::GetRateZ() {
    return RateZ;
}

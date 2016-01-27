#include "Accelerometer.h"

HAL::Accelerometer::Accelerometer(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
}

HAL::Accelerometer::~Accelerometer() {
    
}
    
bool HAL::Accelerometer::Initialize() {
    
}

bool HAL::Accelerometer::Update() {
    
    UnsignedInteger16 Commands[6] = {
        (ACCD_X_LSB << 8),
        (ACCD_X_MSB << 8),
        (ACCD_Y_LSB << 8),
        (ACCD_Y_MSB << 8),
        (ACCD_Z_LSB << 8),
        (ACCD_Z_MSB << 8)
    };
    SignedInteger16 * Incoming = (SignedInteger16*)SendAndReceiveBurst(Commands, 6);
    
    AccelX = (Incoming[1] << 8) | Incoming[0];
    AccelY = (Incoming[3] << 8) | Incoming[2];
    AccelZ = (Incoming[5] << 8) | Incoming[4];
}
    
SignedInteger16 HAL::Accelerometer::GetAccelX() {
    return AccelX;
}

SignedInteger16 HAL::Accelerometer::GetAccelY() {
    return AccelY;
}

SignedInteger16 HAL::Accelerometer::GetAccelZ() {
    return AccelZ;
}

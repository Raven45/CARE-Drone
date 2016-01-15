#include "IMU.h"

HAL::IMU::IMU(ADDRESS Address, SPIBus* DeviceManager) {
    
}

HAL::IMU::~IMU() {
    
}
    
bool HAL::IMU::Initialize() {
    
}

bool HAL::IMU::Update() {
    
    SignedInteger16 * AccelDataCommands[6];
    for (int i = 0; i < 6; i++) {
        //Set Register addresses
        AccelDataCommands[i] = ACC_ACCD_X + i;
        //Set read command bit
        AccelDataCommands[i] |= 0x80;
    }
    SignedInteger16 * AccelData = SendAndReceiveBurst(AccelDataCommands, 6);
}
    
Math::Quaternion HAL::IMU::GetOrientation() {
    return Orientation;
}

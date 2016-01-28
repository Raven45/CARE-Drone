#include "Gyroscope.h"

HAL::Gyroscope::Gyroscope(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
    this->Parity = ParityTypes::OddParity;
}

HAL::Gyroscope::~Gyroscope(){
    
    
}
    
bool HAL::Gyroscope::Initialize() {
    
    //Set range to 125 degrees/s.
    SendAndReceive(GYRO_RANGE);
    
    //Set ODR bandwidth to 200 Hz and filter bandwidth to 23 Hz.
    SendAndReceive(GYRO_BW);
    
    //Set the resolution.
    RateResolution = 124.87f/32768.0f;
}

bool HAL::Gyroscope::Update() {
    
    SignedInteger16 RateX = 0;
    SignedInteger16 RateY = 0;
    SignedInteger16 RateZ = 0;
    
    UnsignedInteger16 Commands[6] = {
        GYRO_RATE_X,
        GYRO_RATE_X + 0x100,
        GYRO_RATE_Y,
        GYRO_RATE_Y + 0x100,
        GYRO_RATE_Z,
        GYRO_RATE_Z + 0x100
    };
    UnsignedInteger16 * Incoming = SendAndReceiveBurst(Commands, 6);
    
    RateX = (Incoming[1] << 8) | Incoming[0];
    RateY = (Incoming[3] << 8) | Incoming[2];
    RateZ = (Incoming[5] << 8) | Incoming[4];
    
    this->RateX = (float)RateX*RateResolution;
    this->RateY = (float)RateY*RateResolution;
    this->RateZ = (float)RateZ*RateResolution;
}
    
float HAL::Gyroscope::GetRateX() {
    return RateX;
}

float HAL::Gyroscope::GetRateY() {
    return RateY;
}

float HAL::Gyroscope::GetRateZ() {
    return RateZ;
}

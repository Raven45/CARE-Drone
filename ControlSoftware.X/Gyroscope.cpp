/*******************************************************************************
  * Copyright 2016  Aaron Burns,
 *                  Joshua Donaway,
 *                  Matthew Love,
 *                  Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*******************************************************************************/

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

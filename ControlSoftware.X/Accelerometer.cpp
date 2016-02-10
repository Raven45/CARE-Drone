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

#include "Accelerometer.h"

HAL::Accelerometer::Accelerometer(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
    this->Parity = ParityTypes::OddParity;
}

HAL::Accelerometer::~Accelerometer() {
    
}
    
bool HAL::Accelerometer::Initialize() {
    
    AccelX = 0.0f;
    AccelY = 0.0f;
    AccelZ = 0.0f;
    AccelResolution = 2.0f/2048.0f;
    
    //Set accelerometer range to plus or minus 2g.
    SendAndReceive(ACCD_RANGE);
    
    //Set filter bandwidth to 16 Hz.
    SendAndReceive(ACCD_BW);
    
    //Set sensor to use filtered data.
    SendAndReceive(ACCD_HBW);
}

bool HAL::Accelerometer::Update() {
    
    SignedInteger16 AccelX = 0;
    SignedInteger16 AccelY = 0;
    SignedInteger16 AccelZ = 0;
    
    UnsignedInteger16 Commands[6] = {
        ACCD_X_LSB,
        ACCD_X_MSB,
        ACCD_Y_LSB,
        ACCD_Y_MSB,
        ACCD_Z_LSB,
        ACCD_Z_MSB
    };
    UnsignedInteger16 * Incoming = SendAndReceiveBurst(Commands, 6);
    
    //If new data is available
    if ((Incoming[0] & 0x01) && (Incoming[0] & 0x01) && (Incoming[0] & 0x01)) {
        AccelX = (SignedInteger16)(((SignedInteger16)Incoming[1] << 8) | Incoming[0]) >> 4;
        AccelY = (SignedInteger16)(((SignedInteger16)Incoming[3] << 8) | Incoming[2]) >> 4;
        AccelZ = (SignedInteger16)(((SignedInteger16)Incoming[5] << 8) | Incoming[4]) >> 4;
    }
    
    //Typecast to a float and scale according to set resolution.
    this->AccelX = (float)AccelX*AccelResolution;
    this->AccelY = (float)AccelY*AccelResolution;
    this->AccelZ = (float)AccelZ*AccelResolution;
}
    
float HAL::Accelerometer::GetAccelX() {
    return AccelX;
}

float HAL::Accelerometer::GetAccelY() {
    return AccelY;
}

float HAL::Accelerometer::GetAccelZ() {
    return AccelZ;
}

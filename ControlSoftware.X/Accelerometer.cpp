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
    
    this->Parity = ParityTypes::NoParity;
}

HAL::Accelerometer::~Accelerometer() {
    
}
    
bool HAL::Accelerometer::Initialize() {

  ChipID = SendAndReceive(0x8F00) & 0x00FF;
  if (ChipID != 0x0049) {
    return false;
  }

  SendAndReceive(ACCEL_ENABLE);      
  SendAndReceive(ACCEL_SET_SCALE);  
}

bool HAL::Accelerometer::Update() {
    
    UnsignedInteger16 Status = SendAndReceive(0xA700) & 0x0008;
    
    if (Status == 0x0008) {
        
        UnsignedInteger16 * Data;
        Data = SendAndReceiveBurst(ACCEL_READ, 4);

        UnsignedInteger16 X = (Data[1] & 0xFF00) | (Data[0] & 0x00FF);
        UnsignedInteger16 Y = (Data[2] & 0xFF00) | (Data[1] & 0x00FF);
        UnsignedInteger16 Z = (Data[3] & 0xFF00) | (Data[2] & 0x00FF);

//        AccelX = (SignedInteger16)X / 2048.0f;
//        AccelY = (SignedInteger16)Y / 2048.0f;
//        AccelZ = (SignedInteger16)Z / 2048.0f;
        AccelX = ((SignedInteger16)X * 0.122F)/1000.0f;
        AccelY = ((SignedInteger16)Y * 0.122F)/1000.0f;
        AccelZ = ((SignedInteger16)Z * 0.122F)/1000.0f;
    }
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

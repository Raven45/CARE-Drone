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
    
    this->Parity = ParityTypes::NoParity;
    this->Scale = 245;
}

HAL::Gyroscope::~Gyroscope(){ }
    
bool HAL::Gyroscope::Initialize() {

  ChipID = SendAndReceive(GYRO_READ_CHIPID) & 0x00FF;
  if (ChipID != 0x00D4) {
    return false;
  }
    
  SendAndReceive(GYRO_ENABLE);        //Enable the gyro along all three axis.
  SendAndReceive(GYRO_SET_FILTER);   
  SendAndReceive(GYRO_SET_INT);

  if (Scale == 245) { 
    SendAndReceive(GYRO_SET_DATA_245);
  }

  else if (Scale == 500) {
    SendAndReceive(GYRO_SET_DATA_500);
  }

  else if (Scale == 2000) {
    SendAndReceive(GYRO_SET_DATA_2000);
  }
  
  SendAndReceive(GYRO_BOOT);          //Boot the device.
}

bool HAL::Gyroscope::Update() {
    
    UnsignedInteger16 Status = SendAndReceive(0xA700) & 0x0008;
    
    if (Status == 0x0008) {

        UnsignedInteger16 * Data;
        Data = SendAndReceiveBurst(0xE800, 4);

        UnsignedInteger16 X = (Data[1] & 0xFF00) | (Data[0] & 0x00FF);
        UnsignedInteger16 Y = (Data[2] & 0xFF00) | (Data[1] & 0x00FF);
        UnsignedInteger16 Z = (Data[3] & 0xFF00) | (Data[2] & 0x00FF);

        if (Scale == 245) { 
          RateX = (SignedInteger16)X * 0.00748f;
          RateY = (SignedInteger16)Y * 0.00748f;
          RateZ = (SignedInteger16)Z * 0.00748f;
        }

        else if (Scale == 500) {
          RateX = (SignedInteger16)X * 0.01526f;
          RateY = (SignedInteger16)Y * 0.01526f;
          RateZ = (SignedInteger16)Z * 0.01526f;
        }

        else if (Scale == 2000) {
          RateX = (SignedInteger16)X * 0.06104f;
          RateY = (SignedInteger16)Y * 0.06104f;
          RateZ = (SignedInteger16)Z * 0.06104f;
        }
    }

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

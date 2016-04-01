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

#include "Magnetometer.h"

HAL::Magnetometer::Magnetometer(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager){
    
    this->Parity = ParityTypes::NoParity;
}

HAL::Magnetometer::~Magnetometer() {}

bool HAL::Magnetometer::Initialize() { 

  SendAndReceive(MAG_SET_DATA);      
  SendAndReceive(MAG_SET_SCALE); 
  SendAndReceive(MAG_SET_MODE);      
  //SendAndReceive(MAG_BOOT); 
}

bool HAL::Magnetometer::Update() {
    
  UnsignedInteger16 * Data;
  Data = SendAndReceiveBurst(MAG_READ, 4);

  UnsignedInteger16 X = (Data[1] & 0xFF00) | (Data[0] & 0x00FF);
  UnsignedInteger16 Y = (Data[2] & 0xFF00) | (Data[1] & 0x00FF);
  UnsignedInteger16 Z = (Data[3] & 0xFF00) | (Data[2] & 0x00FF);

  MagX = (SignedInteger16)X * 0.48f;
  MagX /= 1000;
  MagY = (SignedInteger16)Y * 0.48f;
  MagY /= 1000;
  MagZ = (SignedInteger16)Z * 0.48f;
  MagZ /= 1000;
}

float HAL::Magnetometer::GetMagX() {
    return MagX;
}

float HAL::Magnetometer::GetMagY() {
    return MagY;
}

float HAL::Magnetometer::GetMagZ() {
    return MagZ;
}

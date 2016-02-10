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

#include "LCD.h"

HAL::LCD::LCD(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
}

HAL::LCD::~LCD() {
    
}
    
bool HAL::LCD::Initialize() {
    
}

bool HAL::LCD::Update() {
    
}
    
bool HAL::LCD::PrintString(std::string Message) {
    
    UnsignedInteger16 * Incoming = SendAndReceiveBurst(0, Message.length());
    if (Incoming = 0) {
        return false;
    }
    else {
        return true;
    }
}

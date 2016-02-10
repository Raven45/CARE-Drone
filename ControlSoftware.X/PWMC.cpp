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

#include "PWMC.h"

HAL::PWMC::PWMC(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
}

HAL::PWMC::~PWMC() {
    
}
    
bool HAL::PWMC::Initialize() {
    //MotorIsStarted = false;
}

bool HAL::PWMC::Update() {
         
    //Set throttle
    UnsignedInteger16 ThrottleCommand = ((SET_SPEED << 8) | Throttle);
    UnsignedInteger16 Incoming = SendAndReceive(ThrottleCommand);     
}
    
UnsignedInteger8 HAL::PWMC::GetThrottle() {
    return Throttle;
}

void HAL::PWMC::SetThrottle(UnsignedInteger8 Throttle) {
    this->Throttle = Throttle;
}
    
void HAL::PWMC::StartMotor() {
    SendAndReceive(START_MOTOR << 8);
}

void HAL::PWMC::StopMotor() {
    SendAndReceive(STOP_MOTOR << 8);
}
    
UnsignedInteger8 HAL::PWMC::GetKp() {
    Kp = SendAndReceive(GET_KP << 8);
}

UnsignedInteger8 HAL::PWMC::GetKi() {
    Ki = SendAndReceive(GET_KI << 8);
}

UnsignedInteger8 HAL::PWMC::GetKd() {
    Kd = SendAndReceive(GET_KD << 8);
}

void HAL::PWMC::SetKp(UnsignedInteger8 Kp) {
    SendAndReceive((SET_KP << 8) | Kp );
}

void HAL::PWMC::SetKi(UnsignedInteger8 Ki) {
    SendAndReceive((SET_KI << 8) | Ki );
}

void HAL::PWMC::SetKd(UnsignedInteger8 Kd) {
    SendAndReceive((SET_KD << 8) | Kd );
}

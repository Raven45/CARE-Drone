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
    
    Kp = 0;
    Kd = 0;
    Ki = 0;
    
    //Retrieve the number of motor poles from ESC.
    SendAndReceive(SET_POLES | 22);          //Send command. Discard response.
    
    //Retrieve the current phase advance.
    SendAndReceive(GET_PHASE);          //Send command. Discard response.
    PhaseAdvance = SendAndReceive(0);   //Get value and write zeros. 
    
    //Retrieve the current switching method.
    SendAndReceive(GET_PWM);            //Send command. Discard response.
    PWMSwitchingMethod = (SendAndReceive(0) != 0);   //Get value and write zeros.
    
    
    //SendAndReceive(START_MOTOR);            //Start motor for testing.
}

bool HAL::PWMC::Update() {
         
    //Set throttle
//    UnsignedInteger16 ThrottleCommand = ((SET_SPEED << 8) | Throttle);
//    UnsignedInteger16 Incoming = SendAndReceive(ThrottleCommand);    
    
    //Retrieve the current switching method.
    UnsignedInteger16 Loopback = SendAndReceive(MOTOR_STARTED);              //Send command. Discard response.
    IsMotorStarted = (SendAndReceive(0) != 0);    //Get value and write zeros.

    //Retrieve the current motor speed
    SendAndReceive(GET_THROTTLE);         //Send command. Discard response.
    Throttle = SendAndReceive(0);         //Get value and write zeros.
    
    //Retrieve the current motor speed
    SendAndReceive(GET_SPEED);            //Send command. Discard response.
    MotorSpeed = SendAndReceive(0);       //Get value and write zeros.
}
    
UnsignedInteger8 HAL::PWMC::GetThrottle() {
    return Throttle;
}

UnsignedInteger16 HAL::PWMC::GetSpeed() {
    return MotorSpeed;
}

void HAL::PWMC::SetThrottle(UnsignedInteger8 Throttle) {
    this->Throttle = Throttle;
}
    
void HAL::PWMC::StartMotor() {
    SendAndReceive(START_MOTOR);
}

void HAL::PWMC::StopMotor() {
    SendAndReceive(STOP_MOTOR);
}
    
UnsignedInteger8 HAL::PWMC::GetKp() {
    //Kp = SendAndReceive(GET_KP);
    return Kp;
}

UnsignedInteger8 HAL::PWMC::GetKi() {
    //Ki = SendAndReceive(GET_KI);
    return Ki;
}

UnsignedInteger8 HAL::PWMC::GetKd() {
    //Kd = SendAndReceive(GET_KD);
    return Kd;
}

void HAL::PWMC::SetKp(UnsignedInteger8 Kp) {
    SendAndReceive((SET_KP) | Kp );
}

void HAL::PWMC::SetKi(UnsignedInteger8 Ki) {
    SendAndReceive((SET_KI) | Ki );
}

void HAL::PWMC::SetKd(UnsignedInteger8 Kd) {
    SendAndReceive((SET_KD) | Kd );
}

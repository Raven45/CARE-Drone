/*******************************************************************************
 * Copyright 2016 Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


 * Created on January 25, 2016, 11:26 AM
*******************************************************************************/


#ifndef PWMC_H
#define	PWMC_H
#include "SPIDevice.h"


#define START_MOTOR     0x01
#define STOP_MOTOR      0x02
#define SET_SPEED       0x03
#define GET_SPEED       0x04
#define SET_KP          0x05
#define GET_KP          0x06
#define SET_KI          0x07
#define GET_KI          0x08
#define SET_KD          0x09
#define GET_KD          0x0A
#define GET_STATUS      0x0B

#define STATUS_MOTOR_DEAD    0x00
#define STATUS_MOTOR_STOPPED 0x01
#define STATUS_MOTOR_STARTED 0x02

namespace HAL {

class PWMC: public SPIDevice {
  
public:
    
    PWMC(ADDRESS Address, SPIBus* DeviceManager);
    ~PWMC();
    
    bool Initialize();
    bool Update();
    
    UnsignedInteger8 GetThrottle();
    void SetThrottle(UnsignedInteger8 Throttle);
    
    void StartMotor();
    void StopMotor();
    
    UnsignedInteger8 GetKp();
    UnsignedInteger8 GetKi();
    UnsignedInteger8 GetKd();
    void SetKp(UnsignedInteger8 Kp);
    void SetKi(UnsignedInteger8 Ki);
    void SetKd(UnsignedInteger8 Kd);
    
private:
    //bool StartMotor;
    UnsignedInteger8 Throttle;
    UnsignedInteger8 Kp;
    UnsignedInteger8 Ki;
    UnsignedInteger8 Kd;
};
}


#endif	/* PWMC_H */


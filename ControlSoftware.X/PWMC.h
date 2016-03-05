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

//ESC Command Table
#define START_MOTOR     0x0100  //Starts the motor
#define STOP_MOTOR      0x0200  //Stops the motor
#define SET_THROTTLE    0x0300  //Sets motor throttle to specified value.
#define GET_THROTTLE    0x0400  //Gets current motor throttle.
#define SET_KP          0x0500  //Sets the PID proportional gain.
#define GET_KP          0x0600  //Gets current proportional gain.
#define SET_KI          0x0700  //Sets the PID integral gain.
#define GET_KI          0x0800  //Gets current integral gain.
#define SET_KD          0x0900  //Sets the PID derivative gain.
#define GET_KD          0x0A00  //Gets current derivative gain.
#define GET_SPEED       0x0B00  //Gets current motor speed in rpm.
#define GET_PHASE       0x0C00  //Gets current phase advance in ticks per second.
#define SET_PHASE       0x0D00  //Sets the motor phase advance.
#define GET_POLES       0x0E00  //Gets the current number of motor poles.
#define SET_POLES       0x0F00  //Sets the number of motor poles.
#define GET_PWM         0x1000  //Gets the current switching mode.
#define SET_COMP        0x1100  //Enable Complimentary PWM switching.
#define SET_IND         0x1200  //Enable Independent PWM switching.
#define MOTOR_STARTED   0x1300  //Is motor currently started?
#define ENABLE_PID      0x1400  //Enables the ESC's PID controller.
#define DISABLE_PID     0x1500  //Disables the ESC's PID controller.


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
    UnsignedInteger16 GetSpeed();
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

    UnsignedInteger16 Throttle;
    UnsignedInteger16 MotorSpeed;
    UnsignedInteger16 PhaseAdvance;
    UnsignedInteger16 MotorPoles;
    bool PWMSwitchingMethod;    //true = complimentary, false = independent
    bool IsMotorStarted;        //true=motor running, false=motor stalled.
    UnsignedInteger16 Kp;
    UnsignedInteger16 Ki;
    UnsignedInteger16 Kd;
};
}


#endif	/* PWMC_H */


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

 * Created on January 11, 2016, 11:24 AM
*******************************************************************************/

#ifndef SYSTEM_H
#define	SYSTEM_H

#define INPUT_FLOOR 1000
#define INPUT_CEILING 2000

#define SYSTEM_IS_SINGLETON
#define GetUSBClock()               SYS_CLOCK


#include <stdlib.h>
#include <stdint.h>
#include "HAL.h"
#include "Map.h"
#include "Quaternion.h"
#include "Accelerometer.h"

typedef union {
    float a;
    unsigned char b[4];
}FloatBuffer ;

/*******************************************************************************
 * System class
 * The System object is a singleton object (meaning that only one can exist in 
 * memory) that contains most of the actual working program for the flight 
 * computer. It contains lists of references to every HAL object, register, and 
 * input needed for the operation of the flight computer. 
*******************************************************************************/
class System {
    
    enum States {
        Standby,
        Run,
        Debug
    };
    
public:
    
    /***************************************************************************
     * The constructor for the system object is called very early on in the 
     * execution of the software for the flight computer. It is called by the 
     * flight computer?s initialization process and performs additional 
     * initialization steps for the flight computer. It will create all 
     * register, HAL, and input objects and store references to those objects in
     * lists. It will initialize the SPI bus and initialize all I/O pins on the 
     * PIC32. It will also set its initial state to Standby.
    ***************************************************************************/
    System();
    ~System();
    
    /***************************************************************************
     * Update system will call the Initialize function of every object that has 
     * a reference stored with the system object. 
    ***************************************************************************/
    bool InitializeSystem();
    
    /***************************************************************************
     * Update system will call the Update function of every object that has a 
     * reference stored with the system object. 
    ***************************************************************************/
    bool UpdateSystem();
    
    /***************************************************************************
     * The main function of the system object will make a call to the main 
     * function of the referenced State.
    ***************************************************************************/
    void Main();
    
    /***************************************************************************
     * The ClearToProceed function will evaluate certain conditions and 
     * determine whether or not it is safe for flight. 
    ***************************************************************************/
    bool ClearToProceed();
    
    /***************************************************************************
     * The "IsUSBAttached" function will return whether or not a USB cable
     * is plugged into the flight computer's USB port.
    ***************************************************************************/
    bool IsUSBAttached();
    
    /***************************************************************************
     * The GetInstance function will retrieve and return a reference to the 
     * system object. If the system object has not been 
    ***************************************************************************/
#ifdef SYSTEM_IS_SINGLETON
    static System* GetInstance();
#endif
    
private:
    
    
    
    /***************************************************************************
     * Private Properties.
    ***************************************************************************/
#ifdef SYSTEM_IS_SINGLETON
    //Reference to the singleton object
    static System* Instance;
#endif
    
    //Array to all Devices.
    std::vector<HAL::SPIDevice*> Devices;
    
    //Pointer to the onboard gyroscope.
    HAL::Gyroscope* _Gyroscope;
    
    //Pointer to the onboard accelerometer.
    HAL::Accelerometer* _Accelerometer;
    
    //Pointer to the onboard Magnetometer.
    HAL::Magnetometer* _Magnetometer;
    
    //Pointer to the onboard altimeter
    HAL::Altimeter* _Altimeter;
    
    //Pointers to our motors. Motors
    HAL::PWMC* Motor_1;
    HAL::PWMC* Motor_2;
    HAL::PWMC* Motor_3;
    HAL::PWMC* Motor_4;
    HAL::PWMC* Motor_5;
    HAL::PWMC* Motor_6;
    
    //Integer showing the state of the system.
    UnsignedInteger8 State;
    
    //Quaternion containing the desired orientation as indicated by
    //the ground controller.
    Math::Quaternion SetPoint;
    
    //Quaternion containing the current orientation as indicated by
    //the attached IMU and the sensor fusion algorithm. 
    Math::Quaternion CurrentOrientation;
    
    //The magical delta-time variable. Used for integration.
    UnsignedInteger32 DeltaTime;
    
    //The output for the RC output channel
    unsigned int RC_Output;
    
    //Gain for the Madgwick filters.
    float Beta;
    
    //Proportional gain for PID controller.
    float Kp;
    
    //Integral gain for PID controller.
    float Ki;
    
    //Derivative gain for PID controller.
    float Kd;
    
    //Motor engagement safety
    bool Safety;
    
    //Flag for enabling debugging. This is not the same
    //as the DebugMode state.
    bool SystemDebugging;
    
    //This object manages the SPI bus.
    HAL::SPIBus DeviceManager;
    
    
    /***************************************************************************
     * Private methods
    ***************************************************************************/
    
    /***************************************************************************
     * The main function that is called for when the Drone is in Standby mode.
     * In Standby mode, the drone's motors are disengaged and the drone is
     * awaiting input from the operator. This function will change the drone's 
     * state to Debug mode if a USB cable is attached to the drone while it is
     * in Standby mode. 
    ***************************************************************************/
    void StandbyMain();
    
    /***************************************************************************
     * The main function that is called for when the Drone is in Run mode.
     * In Run mode, the drone's motors are fully engaged and are running, even
     * when the drone is on the ground "idling." It is not safe to approach 
     * the drone while it is in Run mode. 
    ***************************************************************************/
    void RunMain();
    
    /***************************************************************************
     * The main function that is called for when the Drone is in Debug mode.
     * In Debug mode, the drone will wait for input from an attached PC. The
     * PC can start and stop motors on command depending on input from the user.
    ***************************************************************************/
    void DebugMain();
    
    /***************************************************************************
     * The GoToState function will cause a state change to occur. It can change
     * the state of the drone to Standby, Run, or Debug modes. Improper input
     * will cause this function to do nothing.
    ***************************************************************************/
    void GoToState(UnsignedInteger16 State);
    
    /***************************************************************************
     * The ReceiveCommand function will poll the USB port to retrieve commands
     * from an attached PC. It will return a string containing input from the 
     * PC.
    ***************************************************************************/
    const unsigned char * ReceiveCommand();
    
    /***************************************************************************
     * The ExecuteCommand will take input from the ReceiveCommand function,
     * parse it, and then execute the command embedded in the input string.
    ***************************************************************************/
    bool ExecuteCommand(const unsigned char * Command);
    
    /***************************************************************************
     * The SendUSBData function will send a string to an attached PC.
    ***************************************************************************/
    bool SendUSBData(std::string Message);
    
    /***************************************************************************
     * The AHRS_Update function will use the Madgwick filter algorithm to 
     * determine the current attitude of the drone based on sensor input from
     * the magnetometer, accelerometer, and gyroscope. Here AHRS stands
     * for "Attitude & Heading Reference System" and serves as part of the 
     * drone's control algorithm. It will output a unit quaternion representing
     * the drone's attitude in four dimensional space. 
    ***************************************************************************/
    Math::Quaternion AHRS_Update( );
    
    /***************************************************************************
     * The IMU_Update function will use the Madgwick filter algorithm to 
     * determine the current attitude of the drone based on sensor input from
     * the accelerometer and gyroscope. The Magnetometer is not used here. 
     * Here IMU stands for "Inertial Measurement Unit" and serves as part of the 
     * drone's control algorithm. It will output a unit quaternion representing
     * the drone's attitude in four dimensional space. 
    ***************************************************************************/
    Math::Quaternion IMU_Update( );
    
    /***************************************************************************
     * The CalculatePID function will take a calculated error of the drone's 
     * attitude (represented in quaternion form) and calculate the corrections
     * that the drone needs to take. It will output the projected action in 
     * quaternion form. 
    ***************************************************************************/
    Math::Quaternion CalculatePID(Math::Quaternion Error);
    
    bool Command_ReadGyroscope(std::string Command);
    bool Command_ReadAccelerometer(std::string Command);
    bool Command_GetOrientation();
    bool Command_GetPressure();
    bool Command_GetTemperature();
    bool Command_GetAltitude();
    bool Command_SetThrottle(std::string Command);
    bool Command_GetThrottle(std::string Command);
    bool Command_StopAllMotors();
    bool Command_SetAllMotors(std::string Command);
    bool Command_ReleaseCargo();
    bool Command_HoldCargo();
    bool Command_ReturnToStandby();
    bool Command_USBTest();
};

#endif	/* SYSTEM_H */


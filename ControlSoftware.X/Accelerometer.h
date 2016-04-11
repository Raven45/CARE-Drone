/*******************************************************************************
 * Copyright 2016   Aaron Burns,
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


 * Created on January 27, 2016, 10:38 AM
*******************************************************************************/

/*******************************************************************************
 Target Sensor platform: ST LSM9DS0
 It is recommended to use the LSM9DS0 breakout board from Adafruit, as it
 already has 5 volt level shifters.
*******************************************************************************/

#ifndef ACCELEROMETER_H
#define	ACCELEROMETER_H

#include "SPIDevice.h"

#define ACCEL_READ_CHIPID       0x8F00 
#define ACCEL_ENABLE            0x2067  //Enables all 3 axis. 
                                        //Sets data rate to 100 Hz.
                                        //Sets continuous mode.
#define ACCEL_SET_SCALE         0x2148  //Sets filter to 194 Hz.
                                        //Sets range to 16 g.
                                        //Sets sensor to normal mode.
#define ACCEL_READ              0xE800  //Command to burst read the data queue

namespace HAL {

    
/*******************************************************************************
Class:  Accelerometer							
Description:  An accelerometer object represents an interface to a physical 
              sensor that provides the neccesary data for determining
              current accleration along an x/y/z axis.
 
Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
Author:          Aaron Burns.
Change-log:
02-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
class Accelerometer: public SPIDevice {
    
public:
    
    /***********************************************************************
    Subroutine:   Default constructor                  
    Description:  The default constructor will initialize the acellerometer
                  object as an SPI device. Mainly this will pass on
                  information to the SPIDevice's constructor.

    Prerequisites:  SPI bus must be initialized.
    Input:          Address: the physical address to the accellerometer.
                    DeviceManage: Reference to the SPI bus manager object
    Output:         None.
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone FC.
    03-23-2016      Changed from Bosch BMX055 to a ST LSM9DS0
    ************************************************************************/
    Accelerometer(ADDRESS Address, SPIBus* DeviceManager);
    ~Accelerometer();
    
    /***********************************************************************
    Subroutine:   Initialize               
    Description:  The initialize object will attempt to initialize the
                  physical sensor, read the chip identification register,
                  read the sensor calibration data, and set the chip's
                  high pass filter.
     
                  This will return false if the chip identification
                  register cannot be read.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         False: if chip ID register cannot be read.
                    True: Chip ID register was read successfully.
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone FC.
    03-23-2016      Changed from Bosch BMX055 to a ST LSM9DS0
    ************************************************************************/
    bool Initialize();

    /***********************************************************************
    Subroutine:   Update               
    Description:  The update method will read the current acceleration
                  in m/s2 (meters per second squared). No sensor fusion is 
                  performed.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         None.
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone ESC.
    03-23-2016      Changed from Bosch BMX055 to a ST LSM9DS0
    ************************************************************************/
    bool Update();
    
    /***********************************************************************
    Subroutine:   GetAccelX              
    Description:  This will return the acceleration data along the x (Pitch)
                  axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along X in m/s2
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetAccelX();

    /***********************************************************************
    Subroutine:   GetRateY              
    Description:  This will return the acceleration data along the y (Roll)
                  axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along Y in m/s2
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetAccelY();

    /***********************************************************************
    Subroutine:   GetRateZ              
    Description:  This will return the acceleration data along the z (Yaw)
                  axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along Z in m/s2
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetAccelZ();
    
private:
    float AccelX;
    float AccelY;
    float AccelZ;
    float AccelResolution;
    
    UnsignedInteger16 ChipID;
};
}

#endif	/* ACCELEROMETER_H */


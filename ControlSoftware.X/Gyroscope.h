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


 * Created on January 25, 2016, 1:34 PM
*******************************************************************************/

/*******************************************************************************
 Target Sensor platform: ST LSM9DS0
 It is recommended to use the LSM9DS0 breakout board from Adafruit, as it
 already has 5 volt level shifters.
*******************************************************************************/
#ifndef GYROSCOPE_H
#define	GYROSCOPE_H
#include "SPIDevice.h"

#define GYRO_READ_CHIPID      0x8F00    
#define GYRO_ENABLE           0x200F    //Enables all 3 axis. 
                                        //Sets data rate to 95 Hz @ 12.5 Hz cutoff
#define GYRO_SET_FILTER       0x2100    //Sets high pass filter to normal mode.
                                        //Set filter cutoff frequency to 7.2 Hz
#define GYRO_SET_INT          0x2200    //Disable all interrupts and watermarks.
#define GYRO_SET_DATA_245     0x2300    //Coninuous data update
                                        //Little Endian ordering
                                        //245 dps scaling
                                        //self-test disabled
#define GYRO_SET_DATA_500     0x2310    //Coninuous data update
                                        //Little Endian ordering
                                        //500 dps scaling
                                        //self-test disabled
#define GYRO_SET_DATA_2000    0x2320    //Coninuous data update
                                        //Little Endian ordering
                                        //2000 dps scaling
                                        //self-test disabled
                                        //4-wire SPI interface.
#define GYRO_BOOT             0x2480    //Boot the device.
#define GYRO_READ             0xE800    //Command to burst read the data queue


namespace HAL {

/*******************************************************************************
Class:  Gyroscope               
Description:  A gyroscope object represents an interface to a physical 
              sensor that provides rotational data in x/y/z form in degrees per
              second. 
 
Platform:     Microchip PIC32MX270F256D/PIC32MX270F256B
              ST LSM9DS0 over 10 MHz SPI (mode 0)
Author:       Aaron Burns.
Change-log:
01-2016       Original code for CARE-Drone FC (flight computer)
03-23-2016    Changed from Bosch BMX055 to a ST LSM9DS0
*******************************************************************************/

class Gyroscope: public SPIDevice {
    
public:
    
    /***********************************************************************
    Subroutine:   Default constructor                  
    Description:  The default constructor will initialize the Gyroscope
                  object as an SPI device. Mainly this will pass on
                  information to the SPIDevice's constructor.

    Prerequisites:  SPI bus must be initialized.
    Input:          Address: the physical address to the gyroscope.
                    DeviceManage: Reference to the SPI bus manager object
    Output:         None.
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone FC.
    03-23-2016      Changed from Bosch BMX055 to a ST LSM9DS0
    ************************************************************************/
    Gyroscope(ADDRESS Address, SPIBus* DeviceManager);
    ~Gyroscope();
    
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
    Description:  The update method will read the current rotational speed
                  in dps (degrees per second). No sensor fusion is performed.

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
    Subroutine:   GetRateX              
    Description:  This will return the rotational data along the x (Pitch)
                  axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along X in dps
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetRateX();

    /***********************************************************************
    Subroutine:   GetRateY              
    Description:  This will return the rotational data along the y (Roll)
                  axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along Y in dps
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetRateY();

    /***********************************************************************
    Subroutine:   GetRateZ              
    Description:  This will return the rotational data along the z (Yaw)
                  axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along Z in dps
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetRateZ();
    
private:
    float RateX;
    float RateY;
    float RateZ;
    
    UnsignedInteger16 ChipID;

    UnsignedInteger16 Scale;
};
}


#endif	/* GYROSCOPE_H */


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


 * Created on January 27, 2016, 11:25 AM
*******************************************************************************/

/*******************************************************************************
 Target Sensor platform: ST LSM9DS0
 It is recommended to use the LSM9DS0 breakout board from Adafruit, as it
 already has 5 volt level shifters.
*******************************************************************************/

#ifndef MAGNETOMETER_H
#define	MAGNETOMETER_H

#include "SPIDevice.h"

#define MAG_SET_DATA            0x24F0  //Disable temp sensor
                                        //set high resolution mode
                                        //Set to 50 Hz
#define MAG_SET_SCALE           0x2560  //Set to 12 gauss
#define MAG_SET_MODE            0x2600  //Set to normal mode and reset reference
                                        //Filter off
                                        //Normal power mode
                                        //Continous-conversion mode.
#define MAG_BOOT                0x1F80  //Boot the device
#define MAG_READ                0xC800  //Command to burst read the data queue

namespace HAL {

/*******************************************************************************
Class:  Magnetometer                
Description:  A magnetometer object represents an interface to a physical 
              sensor that provides the neccesary data for calculating
              absolute orientation with respect to earth's magnetic north pole.
 
Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
Author:          Aaron Burns.
Change-log:
02-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
class Magnetometer: public SPIDevice {
    
public:
    
    /***********************************************************************
    Subroutine:   Default constructor                  
    Description:  The default constructor will initialize the magnetometer
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
    Magnetometer(ADDRESS Address, SPIBus* DeviceManager);
    ~Magnetometer();
    
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
                  in gaus. No sensor fusion is performed.

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
    Description:  This will return the magnetic data along the x axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along X in gaus
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetMagX();

    /***********************************************************************
    Subroutine:   GetAccelX              
    Description:  This will return the magnetic data along the y axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along Y in gaus
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetMagY();

    /***********************************************************************
    Subroutine:   GetAccelX              
    Description:  This will return the magnetic data along the z axis.

    Prerequisites:  SPI bus must be initialized.
    Input:          None.
    Output:         Rotation along Z in gaus
    Platform:       Microchip PIC32MX270F256D/PIC32MX270F256B
                    ST LSM9DS0
    Author:         Aaron Burns.
    Change-log:
    01-2016         Original code for CARE-Drone flight computer.
    ************************************************************************/
    float GetMagZ();
    
private:
    float MagX;
    float MagY;
    float MagZ;
    
    UnsignedInteger16 ChipID;
};
}



#endif	/* MAGNETOMETER_H */


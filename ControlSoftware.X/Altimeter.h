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


 * Created on January 13, 2016, 8:17 PM
*******************************************************************************/

#ifndef ALTIMETER_H
#define	ALTIMETER_H

#include "SPIDevice.h"

#define TEMP_ADDRESS_MSB    0xFA00
#define TEMP_ADDRESS_LSB    0xFB00
#define TEMP_ADDRESS_XLSB   0xFC00
#define PRES_ADDRESS_MSB    0xF700
#define PRES_ADDRESS_LSB    0xF800
#define PRES_ADDRESS_XLSB   0xF900

typedef unsigned int BMP280_U32_t;
typedef signed int BMP280_S32_t;

namespace HAL {
    
 /*******************************************************************************
 * Class:  Altimeter									
 * Description: An altimeter object represents an interface to a physical 
  *             sensor that provides the neccesary data for an altitude
  *             calculation. In this case, the altimeter class uses the SPI
  *             bus to communicate with a Bosch BME280 sensor. This class is
  *             essentially the driver for the BME280 for the flight computer.
 
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
    class Altimeter: public SPIDevice {
        
    public:
        
        /***********************************************************************
        * Subroutine:  Default constructor									
        * Description: The default constructor will initialize the Altimeter
         *              object as an SPI device. Mainly this will pass on
         *              information to the SPIDevice's constructor.

        * Prerequisites:   SPI bus must be initialized.
        * Input:           Address: the physical address to the altimeter.
         *                 DeviceManage: Reference to the SPI bus manager object
        * Output:          None.
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        Altimeter(ADDRESS Address, SPIBus* DeviceManager);
        ~Altimeter();
    
        /***********************************************************************
        * Subroutine:  Initialize								
        * Description: The initialize object will attempt to initialize the
         *              physical sensor, read the chip identification register,
         *              read the sensor calibration data, and set the chip's
         *              IIR filter.
         * 
         *              This will return false if the chip identification
         *              register cannot be read.

        * Prerequisites:   SPI bus must be initialized.
        * Input:           None.
        * Output:          False: if chip ID register cannot be read.
         *                 True: Chip ID register was read successfully.
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        bool Initialize();
        
        /***********************************************************************
        * Subroutine:  Update								
        * Description: The update method will read the current pressure and
         *              temperature and then calculate the current altitude.

        * Prerequisites:   SPI bus must be initialized.
        * Input:           None.
        * Output:          None.
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        bool Update();
        
        /***********************************************************************
        * Subroutine:  GetAltitude						
        * Description: This will return the current altitude.

        * Prerequisites:   SPI bus must be initialized.
        * Input:           None.
        * Output:          Current Altitude in feet.
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        SignedInteger32 GetAltitude();
        
        /***********************************************************************
        * Subroutine:  GetPressure					
        * Description: This will return the current pressure in pascals.

        * Prerequisites:   SPI bus must be initialized.
        * Input:           None.
        * Output:          Current pressure in pascals
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        UnsignedInteger32 GetPressure();
        
        /***********************************************************************
        * Subroutine:  GetStartinPressure					
        * Description: This will return the pressured measured during the 
         *              flight computer's initialization routine.

        * Prerequisites:   SPI bus must be initialized.
        * Input:           None.
        * Output:          Starting pressure in pascals
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        UnsignedInteger32 GetStartingPressure();
        
        /***********************************************************************
        * Subroutine:  GetTemperature				
        * Description: This will return the current temperature

        * Prerequisites:   SPI bus must be initialized.
        * Input:           None.
        * Output:          Current temperature in kelvins
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        UnsignedInteger32 GetTemperature();
        
    private:
        UnsignedInteger32 InitialPressure;  //Pressure at startup (Pa)
        UnsignedInteger32 CurrentPressure;  //Currently measured pressure (Pa)
        SignedInteger32 CurrentTemperature; //Currently measured temp (K)
        
        SignedInteger32 t_fine;             //Temperature adjusted calibration.
        
        UnsignedInteger16 BME280Status;
        UnsignedInteger16 BME280Config;
        UnsignedInteger16 BME280Ctrl;
        UnsignedInteger16 ChipID;           //Chip identification register.
        
        UnsignedInteger16 dig_T1;           //Temperature calibration data
        SignedInteger16 dig_T2;             //Temperature calibration data
        SignedInteger16 dig_T3;             //Temperature calibration data
        
        UnsignedInteger16 dig_P1;           //Pressure calibration data
        SignedInteger16 dig_P2;             //Pressure calibration data
        SignedInteger16 dig_P3;             //Pressure calibration data
        SignedInteger16 dig_P4;             //Pressure calibration data
        SignedInteger16 dig_P5;             //Pressure calibration data
        SignedInteger16 dig_P6;             //Pressure calibration data
        SignedInteger16 dig_P7;             //Pressure calibration data
        SignedInteger16 dig_P8;             //Pressure calibration data
        SignedInteger16 dig_P9;             //Pressure calibration data
        
        /***********************************************************************
        * Subroutine:  CompensatePressure					
        * Description: This will calculate the pressure based off of the 
         *              input raw value from the sensor and the calibration data

        * Prerequisites:   CompensatTemperature must be ran first in order
         *                 to have compensated temp calibration data.
        * Input:           Raw: a 20 bit value that comes directly from the
         *                 sensor's registers.
        * Output:          The pressure in pascals
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        UnsignedInteger32 CompensatePressure(UnsignedInteger32 Raw);
        
        /***********************************************************************
        * Subroutine:  CompensatePressure					
        * Description: This will calculate the temperature based off of raw
         *             data and calibration data.

        * Prerequisites:   None.
        * Input:           Raw: a 20 bit value containing sensor data directly
         *                 from the sensor's registers.
        * Output:          Temperature in kelvins.
        * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
        * Author:          Aaron Burns.
        * Change-log:
        * 01-2016       Original code for CARE-Drone ESC.
       ************************************************************************/
        SignedInteger32 CompensateTemperature(SignedInteger32 Raw);
    };
}


#endif	/* ALTIMETER_H */


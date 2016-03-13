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
 * Target Sensor platform: Bosch BMX055 IMU
 * Required reading:
 * https://ae-bst.resource.bosch.com/media/products/dokumente/bmx055/BST-BMX055-DS000-02.pdf
*******************************************************************************/

#ifndef ACCELEROMETER_H
#define	ACCELEROMETER_H

#include "SPIDevice.h"

#define ACCD_X_LSB 0x8200
#define ACCD_X_MSB 0x8300
#define ACCD_Y_LSB 0x8400
#define ACCD_Y_MSB 0x8500
#define ACCD_Z_LSB 0x8600
#define ACCD_Z_MSB 0x8700
#define ACCD_RANGE 0x0F03
#define ACCD_BW    0x1009
#define ACCD_HBW   0x1300

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
class Accelerometer: public SPIDevice {
    
public:
    
    Accelerometer(ADDRESS Address, SPIBus* DeviceManager);
    ~Accelerometer();
    
    bool Initialize();
    bool Update();
    
    float GetAccelX();
    float GetAccelY();
    float GetAccelZ();
    
private:
    float AccelX;
    float AccelY;
    float AccelZ;
    float AccelResolution;
};
}

#endif	/* ACCELEROMETER_H */


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

#define ACCD_X_LSB 0x82
#define ACCD_X_MSB 0x83
#define ACCD_Y_LSB 0x84
#define ACCD_Y_MSB 0x85
#define ACCD_Z_LSB 0x86
#define ACCD_Z_MSB 0x87

namespace HAL {

class Accelerometer: public SPIDevice {
    
public:
    
    Accelerometer(ADDRESS Address, SPIBus* DeviceManager);
    ~Accelerometer();
    
    bool Initialize();
    bool Update();
    
    SignedInteger16 GetAccelX();
    SignedInteger16 GetAccelY();
    SignedInteger16 GetAccelZ();
    
private:
    SignedInteger16 AccelX;
    SignedInteger16 AccelY;
    SignedInteger16 AccelZ;
};
}

#endif	/* ACCELEROMETER_H */


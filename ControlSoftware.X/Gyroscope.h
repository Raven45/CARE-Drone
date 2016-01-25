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


 * Created on January 25, 2016, 1:34 PM
*******************************************************************************/

/*******************************************************************************
 * Target Sensor platform: Bosch BMX055 IMU
 * Required reading:
 * https://ae-bst.resource.bosch.com/media/products/dokumente/bmx055/BST-BMX055-DS000-02.pdf
*******************************************************************************/
#ifndef GYROSCOPE_H
#define	GYROSCOPE_H
#include "SPIDevice.h"

#define GYRO_CHIPID     0x00
#define GYRO_RATE_X     0x82
#define GYRO_RATE_Y     0x84
#define GYRO_RATE_Z     0x86

namespace HAL {

class Gyroscope: public SPIDevice {
    
public:
    
    Gyroscope(ADDRESS Address, SPIBus* DeviceManager);
    ~Gyroscope();
    
    bool Initialize();
    bool Update();
    
    UnsignedInteger16 GetRateX();
    UnsignedInteger16 GetRateY();
    UnsignedInteger16 GetRateZ();
    
private:
    UnsignedInteger16 RateX;
    UnsignedInteger16 RateY;
    UnsignedInteger16 RateZ;
};
}


#endif	/* GYROSCOPE_H */


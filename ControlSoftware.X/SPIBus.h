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

 * Created on January 11, 2016, 2:11 PM
*******************************************************************************/

#ifndef SPIBUS_H
#define	SPIBUS_H

#include "Object.h"

//#define _SUPPRESS_PLIB_WARNING
//#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
//#include <plib.h>

#define ENABLE_SPI

#define BUS_CLOCK           (10000000ul)
#define GetSPIClock()       (BUS_CLOCK)

#define ADDRESS unsigned char

#define ADDRESS_GYRO        0x00
#define ADDRESS_ACCEL       0x01
#define ADDRESS_MAG         0x02
#define ADDRESS_BAROMETER   0x03
#define ADDRESS_MOTOR_1     0x04
#define ADDRESS_MOTOR_2     0x05
#define ADDRESS_MOTOR_3     0x06
#define ADDRESS_MOTOR_4     0x07
#define ADDRESS_MOTOR_5     0x08
#define ADDRESS_MOTOR_6     0x09

namespace HAL {

/*******************************************************************************
 * SPIBus class
 * The SPIBus class provides functionality common to all SPI devices. A design
 * decision was made for a separate class to manage the bus itself away from
 * the SPIDevice class. The reason for this was to avoid having the SPIDevice
 * class from initializing the SPI bus and I/O ports multiple times during
 * the execution of the control software.
*******************************************************************************/
class SPIBus: public Object {
    
public:
    SPIBus();
    ~SPIBus();
    
    bool Initialize();
    bool Update();
    
    void SelectSlave(ADDRESS Address);
    void ReleaseSlave(ADDRESS Address);
private:
};
}

#endif	/* SPIBUS_H */


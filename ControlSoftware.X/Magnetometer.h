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
 * Target Sensor platform: Bosch BMX055 IMU
 * Required reading:
 * https://ae-bst.resource.bosch.com/media/products/dokumente/bmx055/BST-BMX055-DS000-02.pdf
*******************************************************************************/

#ifndef MAGNETOMETER_H
#define	MAGNETOMETER_H

#include "SPIDevice.h"

#define EXTENDED_REGISTERS

#define MAG_X_LSB           0xC200
#define MAG_X_MSB           0xC300
#define MAG_Y_LSB           0xC400
#define MAG_Y_MSB           0xC500
#define MAG_Z_LSB           0xC600
#define MAG_Z_MSB           0xC700
#define MAG_HALL_LSB        0xC800
#define MAG_HALL_MSB        0xC900
#define MAG_SOFT_RESET      0x4B82
#define MAG_WAKE_UP         0x4201
#define MAG_NORMAL_MODE     0x4C00
#define MAG_REPS_XY         0x5104
#define MAG_REPS_Z          0x5216

/*******************************************************************************
 * These are addresses for the extended registers for the BMX055.
 * These are somewhat of a mystery. According to the datasheet, there are
 * registers at these addresses, but they are simply marked "reserved."
 * However, third party resources name and show examples for these registers.
 * The addresses are hard coded for read only mode (knock off the most 
 * significant bit, which is the read command bit, in binary for the actual 
 * address).
 ******************************************************************************/
#ifdef EXTENDED_REGISTERS
#define MAG_DIG_X1          0xDD00
#define MAG_DIG_Y1          0xDE00
#define MAG_DIG_Z4_LSB      0xE200
#define MAG_DIG_Z4_MSB      0xE300
#define MAG_DIG_X2          0xE400
#define MAG_DIG_Y2          0xE500
#define MAG_DIG_Z2_LSB      0xE800
#define MAG_DIG_Z2_MSB      0xE900
#define MAG_DIG_Z1_LSB      0xEA00
#define MAG_DIG_Z1_MSB      0xEB00
#define MAG_DIG_XYZ1_LSB    0xEC00
#define MAG_DIG_XYZ1_MSB    0xED00
#define MAG_DIG_Z3_LSB      0xEE00
#define MAG_DIG_Z3_MSB      0xEF00
#define MAG_DIG_XY2         0xF000
#define MAG_DIG_XY1         0xF100
#endif

namespace HAL {

class Magnetometer: public SPIDevice {
    
public:
    
    Magnetometer(ADDRESS Address, SPIBus* DeviceManager);
    ~Magnetometer();
    
    bool Initialize();
    bool Update();
    
    float GetMagX();
    float GetMagY();
    float GetMagZ();
    float GetHallResistance();
    
private:
    float MagX;
    float MagY;
    float MagZ;
    float HallResistance;
    float MagResolution;
    
#ifdef EXTENDED_REGISTERS 
    SignedInteger8      dig_x1, dig_y1, dig_x2, dig_y2, dig_xy2;
    SignedInteger16     dig_z2, dig_z3, dig_z4;
    UnsignedInteger16   dig_z1, dig_xyz1;
    UnsignedInteger8    dig_xy1;
#endif
};
}



#endif	/* MAGNETOMETER_H */


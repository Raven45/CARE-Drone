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

#define DIGT1_ADDRESS 0x8800
#define DIGT2_ADDRESS 0x8A00
#define DIGT3_ADDRESS 0x8C00

#define DIGP1_ADDRESS 0x8E00
#define DIGP2_ADDRESS 0x9000
#define DIGP3_ADDRESS 0x9200
#define DIGP4_ADDRESS 0x9400
#define DIGP5_ADDRESS 0x9600
#define DIGP6_ADDRESS 0x9800
#define DIGP7_ADDRESS 0x9A00
#define DIGP8_ADDRESS 0x9C00
#define DIGP9_ADDRESS 0x9E00

#define TEMP_ADDRESS 0xFA00
#define PRES_ADDRESS 0xF700

typedef unsigned int BMP280_U32_t;
typedef signed int BMP280_S32_t;

namespace HAL {
    
    class Altimeter: public SPIDevice {
        
    public:
        Altimeter(ADDRESS Address, SPIBus* DeviceManager);
        ~Altimeter();
    
        bool Initialize();
        bool Update();
        
        SignedInteger32 GetAltitude();
        UnsignedInteger32 GetPressure();
        UnsignedInteger32 GetStartingPressure();
        UnsignedInteger32 GetTemperature();
        
    private:
        UnsignedInteger32 InitialPressure;
        UnsignedInteger32 CurrentPressure;
        SignedInteger32 CurrentTemperature;
        
        SignedInteger32 t_fine;
        
        UnsignedInteger16 dig_T1;
        SignedInteger16 dig_T2;
        SignedInteger16 dig_T3;
        
        UnsignedInteger16 dig_P1;
        SignedInteger16 dig_P2;
        SignedInteger16 dig_P3;
        SignedInteger16 dig_P4;
        SignedInteger16 dig_P5;
        SignedInteger16 dig_P6;
        SignedInteger16 dig_P7;
        SignedInteger16 dig_P8;
        SignedInteger16 dig_P9;
        
        UnsignedInteger32 CompensatePressure(UnsignedInteger32 Raw);
        SignedInteger32 CompensateTemperature(SignedInteger32 Raw);
    };
}


#endif	/* ALTIMETER_H */


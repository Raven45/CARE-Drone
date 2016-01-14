/* 
 * File:   Altimeter.h
 * Author: aburns2
 *
 * Created on January 13, 2016, 8:17 PM
 */

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


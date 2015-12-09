/* 
 * File:   BMP280.h
 * Author: Aaron
 *
 * Created on December 8, 2015, 10:55 PM
 */

#ifndef BMP280_H
#define	BMP280_H
#include "Device.h"

typedef unsigned int BMP280_U32_t;
typedef signed int BMP280_S32_t;



class BMP280: Device {
    
    public:
        
        BMP280();
        SignedInteger32 RetrievePressure();
    private:
        
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

        SignedInteger32 bmp280_compensate_T_int32(BMP280_S32_t adc_T);
        UnsignedInteger32 bmp280_compensate_P_int32(BMP280_S32_t adc_P);
};



#endif	/* BMP280_H */


#include "BMP280.h"

BMP280::BMP280():Device() {
    
    //Sensor does not use a parity.
    SetParity(ParityTypes::NoParity);
    SetSlave(2);
    
    dig_T1 = SendAndReceive32S(DIGT1_ADDRESS, Device::TransmitMode::Address);
    dig_T2 = SendAndReceive32(DIGT2_ADDRESS, Device::TransmitMode::Address);
    dig_T3 = SendAndReceive32(DIGT3_ADDRESS, Device::TransmitMode::Address);
    
    dig_P1 = SendAndReceive32S(DIGP1_ADDRESS, Device::TransmitMode::Address);
    dig_P2 = SendAndReceive32(DIGP2_ADDRESS, Device::TransmitMode::Address);
    dig_P3 = SendAndReceive32(DIGP3_ADDRESS, Device::TransmitMode::Address);
    dig_P4 = SendAndReceive32(DIGP4_ADDRESS, Device::TransmitMode::Address);
    dig_P5 = SendAndReceive32(DIGP5_ADDRESS, Device::TransmitMode::Address);
    dig_P6 = SendAndReceive32(DIGP6_ADDRESS, Device::TransmitMode::Address);
    dig_P7 = SendAndReceive32(DIGP7_ADDRESS, Device::TransmitMode::Address);
    dig_P8 = SendAndReceive32(DIGP8_ADDRESS, Device::TransmitMode::Address);
    dig_P9 = SendAndReceive32(DIGP9_ADDRESS, Device::TransmitMode::Address);
}

SignedInteger32 BMP280::RetrievePressure() {
    
    SignedInteger32 IncomingTemp = 0;
    SignedInteger32 IncomingPressure = 0;
    SignedInteger32 Pressure = 0;
    
    IncomingTemp = SendAndReceive32S(TEMP_ADDRESS, Device::TransmitMode::Address);
    IncomingPressure = SendAndReceive32S(PRES_ADDRESS, Device::TransmitMode::Address);
 
    bmp280_compensate_T_int32(IncomingTemp);
    Pressure = bmp280_compensate_P_int32(IncomingPressure);
    
    return Pressure;
}

BMP280_S32_t BMP280::bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
    BMP280_S32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((BMP280_S32_t)dig_T1<<1))) * ((BMP280_S32_t)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((BMP280_S32_t)dig_T1)) * ((adc_T>>4) - ((BMP280_S32_t)dig_T1))) >> 12) *
    ((BMP280_S32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

BMP280_U32_t BMP280::bmp280_compensate_P_int32(BMP280_S32_t adc_P) {
    
    BMP280_S32_t var1, var2;
    BMP280_U32_t p;
    var1 = (((BMP280_S32_t)t_fine)>>1) - (BMP280_S32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((BMP280_S32_t)dig_P6);
    var2 = var2 + ((var1*((BMP280_S32_t)dig_P5))<<1);
    var2 = (var2>>2)+(((BMP280_S32_t)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BMP280_S32_t)dig_P2) * var1)>>1))>>18;
    var1 =((((32768+var1))*((BMP280_S32_t)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = (((BMP280_U32_t)(((BMP280_S32_t)1048576)-adc_P)-(var2>>12)))*3125;
    if (p < 0x80000000)
    {
        p = (p << 1) / ((BMP280_U32_t)var1);
    }
    else
    {
        p = (p / (BMP280_U32_t)var1) * 2;
    }
    var1 = (((BMP280_S32_t)dig_P9) * ((BMP280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((BMP280_S32_t)(p>>2)) * ((BMP280_S32_t)dig_P8))>>13;
    p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + dig_P7) >> 4));
    return p;
}

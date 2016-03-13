/*******************************************************************************
  * Copyright 2016  Aaron Burns,
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
*******************************************************************************/

#include "Altimeter.h"

HAL::Altimeter::Altimeter(ADDRESS Address, HAL::SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
    
    this->Parity = ParityTypes::NoParity;
}

HAL::Altimeter::~Altimeter() {
    
}
    
bool HAL::Altimeter::Initialize() {
    
    SendAndReceive(0xE0B6 & 0x7FFF);
    
    for (int i = 0; i<5000; i++);
    
    ChipID = SendAndReceive(0xD000);
    ChipID = ChipID & 0x00FF;
    
    if (ChipID != 0x0060) {
        return false;
    }
    
    SendAndReceive(0xF510 & 0x7FFF); //x16 filter, 0.5 ms
    SendAndReceive(0xF201 & 0x7FFF); //x1 oversampling for humidity.
    SendAndReceive(0xF447 & 0x7FFF); //x16 oversampling for pressure, x2 for temp, normal mode.
    
    BME280Status = SendAndReceive(0xF300);
    BME280Config = SendAndReceive(0xF500);
    BME280Ctrl   = SendAndReceive(0xF400);

    dig_T1 = ((SendAndReceive(0x8900)& 0x00ff) << 8) | (SendAndReceive(0x8800) & 0x00FF);
    dig_T2 = (SignedInteger16)((SendAndReceive(0x8B00)& 0x00ff) << 8) | (SendAndReceive(0x8A00) & 0x00FF);
    dig_T3 = (SignedInteger16)((SendAndReceive(0x8D00)& 0x00ff) << 8) | (SendAndReceive(0x8C00) & 0x00FF);

    dig_P1 = ((SendAndReceive(0x8F00)&0x00ff) << 8) | (SendAndReceive(0x8E00) & 0x00FF);
    dig_P2 = (SignedInteger16)((SendAndReceive(0x9100)&0x00ff) << 8) | (SendAndReceive(0x9000) & 0x00FF);
    dig_P3 = (SignedInteger16)((SendAndReceive(0x9300)&0x00ff) << 8) | (SendAndReceive(0x9200) & 0x00FF);
    dig_P4 = (SignedInteger16)((SendAndReceive(0x9500)&0x00ff) << 8) | (SendAndReceive(0x9400) & 0x00FF);
    dig_P5 = (SignedInteger16)((SendAndReceive(0x9700)&0x00ff) << 8) | (SendAndReceive(0x9600) & 0x00FF);
    dig_P6 = (SignedInteger16)((SendAndReceive(0x9900)&0x00ff) << 8) | (SendAndReceive(0x9800) & 0x00FF);
    dig_P7 = (SignedInteger16)((SendAndReceive(0x9B00)&0x00ff) << 8) | (SendAndReceive(0x9A00) & 0x00FF);
    dig_P8 = (SignedInteger16)((SendAndReceive(0x9D00)&0x00ff) << 8) | (SendAndReceive(0x9C00) & 0x00FF);
    dig_P9 = (SignedInteger16)((SendAndReceive(0x9F00)&0x00ff) << 8) | (SendAndReceive(0x9E00) & 0x00FF);

//     DeviceManager->SelectSlave(this->Address);
//
//    while (!SPI1STATbits.SPITBE);
//    SPI1BUF = 0xF700;
//    while (!SPI1STATbits.SPIRBF);
//    UnsignedInteger32 P1 = SPI1BUF;
//    P1 = P1 & 0x00FF;
//
//    while (!SPI1STATbits.SPITBE);
//    SPI1BUF = 0x0000;
//    while (!SPI1STATbits.SPIRBF);
//    UnsignedInteger16 P2 = SPI1BUF;
//
//    DeviceManager->ReleaseSlave(this->Address);
//
//    for (int i = 0; i<100; i++);

//    InitialPressure = (P1 <<12) | (P2>>4);    
//    InitialPressure = CompensatePressure(InitialPressure)/256;
    InitialPressure = 101325;
        
    UnsignedInteger32 T1 = SendAndReceive(0xFA00) & 0x00FF;
    UnsignedInteger16 T2 = SendAndReceive(0xFB00) & 0x00FF;
    UnsignedInteger16 T3 = SendAndReceive(0xFC00) & 0x00FF;

    CurrentTemperature = (T1 <<12) | (T2<<4) | (T3>>4);
    CurrentTemperature = CompensateTemperature(CurrentTemperature) + 27315;
//
//        UnsignedInteger32 P1 = SendAndReceive(0xF700) & 0x00FF;
//        UnsignedInteger16 P2 = SendAndReceive(0xF800) & 0x00FF;
//        UnsignedInteger16 P3 = SendAndReceive(0xF900) & 0x00FF;
//
//        InitialPressure = (P1 <<12) | (P2<<4) | (P3>>4);    
//        InitialPressure = CompensatePressure(InitialPressure)/256;
    
    //while (SendAndReceive(0xF300) & 0x000F);
    
   
 
    return true;
}

bool HAL::Altimeter::Update() {
        
    UnsignedInteger32 T1 = SendAndReceive(0xFA00) & 0x00FF;
    UnsignedInteger16 T2 = SendAndReceive(0xFB00) & 0x00FF;
    UnsignedInteger16 T3 = SendAndReceive(0xFC00) & 0x00FF;

    CurrentTemperature = (T1 <<12) | (T2<<4) | (T3>>4);
    CurrentTemperature = CompensateTemperature(CurrentTemperature) + 27315;

    UnsignedInteger32 P1 = SendAndReceive(0xF700) & 0x00FF;
    UnsignedInteger16 P2 = SendAndReceive(0xF800) & 0x00FF;
    UnsignedInteger16 P3 = SendAndReceive(0xF900) & 0x00FF;

    CurrentPressure = (P1 <<12) | (P2<<4) | (P3>>4);    
    CurrentPressure = CompensatePressure(CurrentPressure)/256;
    
    return true;
}
        
SignedInteger32 HAL::Altimeter::GetAltitude() {
    
    //Z = (RT/gM)log(Po/P)
    //8314 = ideal gas constant times 1000.
    //9.81 = acceleration due to gravity
    //28.97 = molar mass of air
    float temp = CurrentTemperature/100;
    return ((3.28f*8314*temp)/(9.81f * 28.97f)) * log((float)InitialPressure/(float)CurrentPressure);
    //return 3.28 * 44330.0 * (1.0 - pow((float)CurrentPressure / (float)InitialPressure, 0.1903));
}

UnsignedInteger32 HAL::Altimeter::GetPressure() {
    return CurrentPressure;
}

UnsignedInteger32 HAL::Altimeter::GetStartingPressure() {
    return InitialPressure;
}


UnsignedInteger32 HAL::Altimeter::GetTemperature() {
    return CurrentTemperature;
}

UnsignedInteger32 HAL::Altimeter::CompensatePressure(UnsignedInteger32 Raw) {
    
/*******************************************************************************
 * Trimming function for reading Pressure from a Bosch BMP280 sensor.
 * Refer to page 22 of the BMP280 datasheet at:
 * https://ae-bst.resource.bosch.com/media/products/dokumente/bmp280/BST-BMP280-DS001-12.pdf
*******************************************************************************/
    
    long long int var1, var2, p;  
    var1 = ((long long int)t_fine) - 128000;  
    var2 = var1 * var1 * (long long int)dig_P6;  
    var2 = var2 + ((var1*(long long int)dig_P5)<<17);  
    var2 = var2 + (((long long int)dig_P4)<<35);  
    var1 = ((var1 * var1 * (long long int)dig_P3)>>8) + ((var1 * (long long int)dig_P2)<<12);  
    var1 = (((((long long int)1)<<47)+var1))*((long long int)dig_P1)>>33;  
    if (var1 == 0) {   
        return 0; // avoid exception caused by division by zero  
    }  
    p = 1048576 - Raw;  
    p = (((p<<31)-var2)*3125)/var1;  
    var1 = (((long long int)dig_P9) * (p>>13) * (p>>13)) >> 25;  
    var2 = (((long long int)dig_P8) * p) >> 19;  
    p = ((p + var1 + var2) >> 8) + (((long long int)dig_P7)<<4);  
    return (UnsignedInteger32)p;
}

SignedInteger32 HAL::Altimeter::CompensateTemperature(SignedInteger32 Raw) {
    
/*******************************************************************************
 * Trimming function for reading Temperature from a Bosch BMP280 sensor.
 * Refer to page 22 of the BMP280 datasheet at:
 * https://ae-bst.resource.bosch.com/media/products/dokumente/bmp280/BST-BMP280-DS001-12.pdf
*******************************************************************************/
    
    SignedInteger32 var1  = ((((Raw>>3) - ((SignedInteger32)dig_T1<<1))) * ((SignedInteger32)dig_T2)) >> 11;
    SignedInteger32 var2  = (((((Raw>>4) - ((BMP280_S32_t)dig_T1)) * ((Raw>>4) - ((BMP280_S32_t)dig_T1))) >> 12) *    ((BMP280_S32_t)dig_T3)) >> 14;  
    t_fine = var1 + var2;  
    return (t_fine * 5 + 128) >> 8;  
}
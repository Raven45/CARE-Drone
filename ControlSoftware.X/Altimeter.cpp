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
    
    WriteSPI(0xD0);
    unsigned int incoming1 = ReadSPI();
    
    WriteSPI(0xF7);
    unsigned int incoming2 = ReadSPI();
    
    WriteSPI(0xFA);
    unsigned int incoming3 = ReadSPI();
    
    WriteSPI(0xAA);
    unsigned int incoming4 = ReadSPI();
    incoming1++;
//        
//    //Create an array of commands that will retrieve the trim values.
//    UnsignedInteger16 * GetTrimCommands = new UnsignedInteger16[24];
//    UnsignedInteger16* TrimValues = new UnsignedInteger16[24];
//    for (int i = 0, j = DIGT1_ADDRESS; i < 24; i++, j += 0x0100) {
//        //Assign individual commands, incrementing the address each frame.
//        GetTrimCommands[i] = j;
//    }
//    
//    UnsignedInteger16 Test = SendAndReceive(0xD0D0 & 0x7F7F);
//    
//    //Perform the actual burst
//    TrimValues = SendAndReceiveBurst(GetTrimCommands,24);
//    if (TrimValues == 0) {
//        //if we got nothing back, then it means that the device is not 
//        //communicating. Return false and let the software layers above deal
//        //with the issue.
//        return false;
//    }
//    
//    //If we got actual data back. Note that there is no safeguard against junk.
//    else {
//        
//        dig_T1 = (TrimValues[0] | (TrimValues[1] << 8));
//        dig_T2 = ((SignedInteger16)TrimValues[2] | ((SignedInteger16)TrimValues[3] << 8));
//        dig_T3 = (SignedInteger16)TrimValues[4] | ((SignedInteger16)TrimValues[5] << 8);
//        
//        dig_P1 = TrimValues[6] | (TrimValues[7] << 8);
//        dig_P2 = (SignedInteger16)TrimValues[8] | ((SignedInteger16)TrimValues[9] << 8);
//        dig_P3 = (SignedInteger16)TrimValues[10] | ((SignedInteger16)TrimValues[11] << 8);
//        dig_P4 = (SignedInteger16)TrimValues[12] | ((SignedInteger16)TrimValues[13] << 8);
//        dig_P5 = (SignedInteger16)TrimValues[14] | ((SignedInteger16)TrimValues[15] << 8);
//        dig_P6 = (SignedInteger16)TrimValues[16] | ((SignedInteger16)TrimValues[17] << 8);
//        dig_P7 = (SignedInteger16)TrimValues[18] | ((SignedInteger16)TrimValues[19] << 8);
//        dig_P8 = (SignedInteger16)TrimValues[20] | ((SignedInteger16)TrimValues[21] << 8);
//        dig_P9 = (SignedInteger16)TrimValues[22] | ((SignedInteger16)TrimValues[23] << 8);
//    }
//    
//    //Create an array of commands that will retrieve the pressure.
//    UnsignedInteger16 * GetPressureCommands = new UnsignedInteger16[3];
//    GetPressureCommands[0] = (PRES_ADDRESS + 0x0200);
//    GetPressureCommands[1] = (PRES_ADDRESS + 0x0100);
//    GetPressureCommands[2] = PRES_ADDRESS;
//    
//    //Perform burst transmission
//    UnsignedInteger16 * PressureArray = SendAndReceiveBurst(GetPressureCommands, 3);
//    if (PressureArray == 0) {
//        //if we got nothing back, then it means that the device is not 
//        //communicating. Return false and let the software layers above deal
//        //with the issue.
//        return false;
//    }
//    //If we got actual data back. Note that there is no safeguard against junk.
//    else {
//        UnsignedInteger32 PressureRaw = (PressureArray[0] >> 4);
//        PressureRaw |= ((UnsignedInteger32)PressureArray[1] << 4);
//        PressureRaw |= ((UnsignedInteger32)PressureArray[2] << 12);
//    
//        //Save the initial pressure reading. This will serve as the 
//        //reference point for the altimeter.
//        InitialPressure = CompensatePressure(PressureRaw) / 256;
//    }
//    
    return true;
}

bool HAL::Altimeter::Update() {
    
    UnsignedInteger16 * GetPressureCommands = new UnsignedInteger16[3];
    GetPressureCommands[0] = (PRES_ADDRESS + 0x0200);
    GetPressureCommands[1] = (PRES_ADDRESS + 0x0100);
    GetPressureCommands[2] = PRES_ADDRESS;
    
    UnsignedInteger16 * PressureArray = SendAndReceiveBurst(GetPressureCommands, 3);
    if (PressureArray == 0) {
        return false;
    }
    else {
        UnsignedInteger32 PressureRaw = (PressureArray[0] >> 4);
        PressureRaw |= ((UnsignedInteger32)PressureArray[1] << 4);
        PressureRaw |= ((UnsignedInteger32)PressureArray[2] << 12);
    
        CurrentPressure = CompensatePressure(PressureRaw) / 256;
    }
    
    UnsignedInteger16 * GetTemperatureCommands = new UnsignedInteger16[3];
    GetTemperatureCommands[0] = (TEMP_ADDRESS + 0x0200);
    GetTemperatureCommands[1] = (TEMP_ADDRESS + 0x0100);
    GetTemperatureCommands[2] = TEMP_ADDRESS;
    
    SignedInteger16 * TempArray = (SignedInteger16 *)SendAndReceiveBurst(GetTemperatureCommands, 3);
    if (TempArray == 0) {
        return false;
    }
    else {
        SignedInteger32 TemperatureRaw = (TempArray[0] >> 4);
        TemperatureRaw |= ((SignedInteger32)TempArray[1] << 4);
        TemperatureRaw |= ((SignedInteger32)TempArray[2] << 12);
    
        CurrentTemperature = CompensateTemperature(TemperatureRaw);
    }
    
    return true;
}
        
SignedInteger32 HAL::Altimeter::GetAltitude() {
    
    //Z = (RT/gM)log(Po/P)
    //8314 = ideal gas constant times 1000.
    //9.81 = acceleration due to gravity
    //28.97 = molar mass of air
    return ((8314*CurrentTemperature)/(9.81f * 28.97f)) * log(InitialPressure/CurrentPressure);
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
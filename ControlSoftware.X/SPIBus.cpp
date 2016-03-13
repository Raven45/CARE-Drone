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

#include "SPIBus.h"

HAL::SPIBus::SPIBus() { 
}

HAL::SPIBus::~SPIBus() {
    SpiChnClose(SPI_CHANNEL1);
}
    
bool HAL::SPIBus::Initialize() {
    
    //Configure the SPI port.
    unsigned int config=SPI_OPEN_MODE16 |           //Use 16 bit words.
                        //SPI_OPEN_CKP_HIGH |         //Clock idle high-active low.
                        SPI_OPEN_CKE_REV |          //Transmit active-to-idle.
                        //SPI_OPEN_SMP_END |
                        SPI_OPEN_ON |               //We should turn it on.
                        SPI_OPEN_MSTEN;             //Enable master mode.

    //Turn on the SPI port
    SpiChnOpen(SPI_CHANNEL1, (SpiOpenFlags)config, GetPeripheralClock()/5000000);
    
    return true;
}

bool HAL::SPIBus::Update() {
    return true;
}
    
void HAL::SPIBus::SelectSlave(ADDRESS Address) {
    
    switch (Address) {
        #if defined(__32MX270F256D__)
            case ADDRESS_GYRO:      PORTClearBits(IOPORT_A, BIT_1); break;
            case ADDRESS_ACCEL:     PORTClearBits(IOPORT_A, BIT_0); break;
            case ADDRESS_MAG:       PORTClearBits(IOPORT_B, BIT_0); break;
            case ADDRESS_BAROMETER: PORTClearBits(IOPORT_B, BIT_1); break;
            case ADDRESS_MOTOR_1:   PORTClearBits(IOPORT_B, BIT_2); break;
            case ADDRESS_MOTOR_2:   PORTClearBits(IOPORT_B, BIT_3); break;
            case ADDRESS_MOTOR_3:   PORTClearBits(IOPORT_C, BIT_0); break;
            case ADDRESS_MOTOR_4:   PORTClearBits(IOPORT_C, BIT_1); break;
            case ADDRESS_MOTOR_5:   PORTClearBits(IOPORT_C, BIT_2); break;
            case ADDRESS_MOTOR_6:   PORTClearBits(IOPORT_A, BIT_3); break;
        #elif defined(__32MX270F256B__)
            case ADDRESS_BAROMETER:   PORTClearBits(IOPORT_B, BIT_2); break;
        #endif
    }
}

void HAL::SPIBus::ReleaseSlave(ADDRESS Address) {
    
    switch (Address) {
        #if defined(__32MX270F256D__)
            case ADDRESS_GYRO:      PORTSetBits(IOPORT_A, BIT_1); break;
            case ADDRESS_ACCEL:     PORTSetBits(IOPORT_A, BIT_0); break;
            case ADDRESS_MAG:       PORTClearBits(IOPORT_B, BIT_0); break;
            case ADDRESS_BAROMETER: PORTSetBits(IOPORT_B, BIT_1); break;
            case ADDRESS_MOTOR_1:   PORTSetBits(IOPORT_B, BIT_2); break;
            case ADDRESS_MOTOR_2:   PORTSetBits(IOPORT_B, BIT_3); break;
            case ADDRESS_MOTOR_3:   PORTSetBits(IOPORT_C, BIT_0); break;
            case ADDRESS_MOTOR_4:   PORTSetBits(IOPORT_C, BIT_1); break;
            case ADDRESS_MOTOR_5:   PORTSetBits(IOPORT_C, BIT_2); break;
            case ADDRESS_MOTOR_6:   PORTSetBits(IOPORT_C, BIT_3); break;
        #elif defined(__32MX270F256B__)
            case ADDRESS_BAROMETER:   PORTSetBits(IOPORT_B, BIT_2); break;
        #endif
    }
}

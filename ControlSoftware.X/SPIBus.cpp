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
                        //SPI_OPEN_CKP_HIGH |       //Clock polarity high.
                        //SPI_OPEN_CKE_REV |        //Clock edge detect.
                        SPI_OPEN_ON |               //We should turn it on.
                        SPI_OPEN_MSTEN;             //Enable master mode.

    //Turn on the SPI port
    SpiChnOpen(SPI_CHANNEL1, (SpiOpenFlags)config, 8);
    
    /*
    //Configure pin B1 as an output for SPI.
    PPSOutput(2, RPB1, SDO1);
    PPSInput(2, SDI1, RPB5);
    
    //Initialize the slave select ports
    PORTSetPinsDigitalOut(IOPORT_A, BIT_0);
    PORTSetPinsDigitalOut(IOPORT_A, BIT_1);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_0);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_1);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_2);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_3);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_0);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_1);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_2);
    
    //Set the ports to HIGH
    PORTSetBits(IOPORT_A, BIT_0);
    PORTSetBits(IOPORT_A, BIT_1);
    PORTSetBits(IOPORT_B, BIT_0);
    PORTSetBits(IOPORT_B, BIT_1);
    PORTSetBits(IOPORT_B, BIT_2);
    PORTSetBits(IOPORT_B, BIT_3);
    PORTSetBits(IOPORT_C, BIT_0);
    PORTSetBits(IOPORT_C, BIT_1);
    PORTSetBits(IOPORT_C, BIT_2);
    */
    return true;
}
bool HAL::SPIBus::Update() {
    return true;
}
    
void HAL::SPIBus::SelectSlave(ADDRESS Address) {
    
    switch (Address) {
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
    }
}

void HAL::SPIBus::ReleaseSlave(ADDRESS Address) {
    
    switch (Address) {
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
    }
}

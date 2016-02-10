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

#include "SPIDevice.h"

HAL::SPIDevice::SPIDevice(ADDRESS Address, SPIBus* DeviceManager){
    this->Address = Address;
    this->DeviceManager = DeviceManager;
}

HAL::SPIDevice::~SPIDevice() {}

    
UnsignedInteger16 HAL::SPIDevice::SendAndReceive(UnsignedInteger16 Outgoing) {
    
    try {
        
        UnsignedInteger16 Incoming = 0;
        Outgoing = EncodeParity(Outgoing);

        DeviceManager->SelectSlave(this->Address);

        //Send the data to the slave.
        SpiChnPutC(SPI_CHANNEL1, Outgoing);

        //Capture response from slave.
        Incoming = SpiChnGetC(SPI_CHANNEL1);

        DeviceManager->ReleaseSlave(this->Address);

        if (VerifyParity(Incoming)) {
            return Incoming;
        }
        else {
            return 0;
        }
    }
    
    catch (...) {
        return 0;
    }
}

UnsignedInteger16 * HAL::SPIDevice::SendAndReceiveBurst(UnsignedInteger16 * Outgoing, UnsignedInteger16 Length) {

    try {
        
        UnsignedInteger16 * Incoming = new UnsignedInteger16[Length];

        for (UnsignedInteger16 i = 0; i < Length; i++) {
            Incoming[i] = SendAndReceive(Outgoing[i]);
        }

        return Incoming;
    }
    catch (...) {
        return 0;
    }
}

/*
SignedInteger16 HAL::SPIDevice::SendAndReceive(SignedInteger16 Outgoing) {
    
    try {
        
        UnsignedInteger16 Incoming = 0;
        Outgoing = EncodeParity(Outgoing);

        DeviceManager->SelectSlave(this->Address);

        //Send the data to the slave.
        SpiChnPutC(SPI_CHANNEL1, Outgoing);

        //Capture response from slave.
        Incoming = SpiChnGetC(SPI_CHANNEL1);

        DeviceManager->ReleaseSlave(this->Address);

        if (VerifyParity(Incoming)) {
            return Incoming;
        }
        else {
            return 0;
        }
    }
    
    catch (...) {
        return 0;
    }
}

SignedInteger16 * HAL::SPIDevice::SendAndRecieveBurst(SignedInteger16 * Outgoing, UnsignedInteger16 Length) {
    
    try {
        
        SignedInteger16 * Incoming = new SignedInteger16[Length];

        for (UnsignedInteger16 i = 0; i < Length; i++) {
            Incoming[i] = SendAndReceive(Outgoing[i]);
        }

        return Incoming;
    }
    catch (...) {
        return 0;
    }
}
 * */
    

UnsignedInteger16 HAL::SPIDevice::EncodeParity(UnsignedInteger16 Message) {
    
    //Add a parity bit if even parity is enabled.
    if (Parity == ParityTypes::EvenParity) {
        
        int NumOfOnes = 0;

        for (int i = 0; i < 15; i++) {
            if (CheckBit(Message, i)) {
                NumOfOnes++;
            }
        }
        
        if (NumOfOnes % 2 != 0) {
            Message ^= 1 << 15; //Set Parity
        }
    }
    
    //Add a parity bit if odd parity is enabled.
    else if (Parity == ParityTypes::OddParity) {
        
        int NumOfOnes = 0;
        
        for (int i = 0; i < 15; i++) {
            if (CheckBit(Message, i)) {
                NumOfOnes++;
            }
        }
        
        if (NumOfOnes % 2 == 0) {
            Message ^= 1 << 7; //Set Parity
        }
    }
    
    return Message;
}

bool HAL::SPIDevice::VerifyParity(UnsignedInteger16 Message) {
    return true;
}

std::string HAL::SPIDevice::GetName() {
    return this->Name;
}

void HAL::SPIDevice::SetName(std::string Name) {
    this->Name = Name;
}

void HAL::SPIDevice::ChangeAddress(unsigned char Address){
    this->Address = Address;
}
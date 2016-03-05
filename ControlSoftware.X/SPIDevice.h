/*******************************************************************************
 * Copyright 2016 Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


 * Created on January 11, 2016, 1:27 PM
*******************************************************************************/
#ifndef SPIDEVICE_H
#define	SPIDEVICE_H

#include "SPIBus.h"
#include <math.h>

namespace HAL {

enum ParityTypes {
    NoParity,
    OddParity,
    EvenParity
};

/*******************************************************************************
 * SPIDevice class
 * SPIDevice is a superclass that defines the minimum functionality to implement
 * the software component of a device attached to the flight computer?s SPI bus. 
 * The two minimum requirements for implementing a SPI device is that the device
 * has a hardware address and a function to send and receive data to and from 
 * the attached device. 
*******************************************************************************/
class SPIDevice: public Object {
    
public:
    SPIDevice(ADDRESS Address, SPIBus* DeviceManager);
    virtual ~SPIDevice() = 0;
    
    virtual bool Initialize() = 0;
    virtual bool Update() = 0;
    
    UnsignedInteger16 ReadSPI();
    void WriteSPI(UnsignedInteger8 Data);
    
    UnsignedInteger16 SendAndReceive(UnsignedInteger16 Outgoing);
    UnsignedInteger16 * SendAndReceiveBurst(UnsignedInteger16 * Outgoing, UnsignedInteger16 Length);
    /*
    SignedInteger16 SendAndReceiveS(SignedInteger16 Outgoing);
    SignedInteger16 * SendAndRecieveBurstS(SignedInteger16 * Outgoing, UnsignedInteger16 Length);
     * */
    
    UnsignedInteger16 EncodeParity(UnsignedInteger16 Message);
    bool VerifyParity(UnsignedInteger16 Message);
    
    std::string GetName();
    void SetName(std::string Name);
    
    void ChangeAddress(ADDRESS Address);
    
protected:
    ADDRESS Address;
    UnsignedInteger16 Parity;
    SPIBus* DeviceManager;
    std::string Name;
};

}


#endif	/* SPIDEVICE_H */


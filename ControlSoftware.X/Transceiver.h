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


 * Created on April 7, 2016, 7:47 AM
*******************************************************************************/


#ifndef TRANSMITTER_H
#define	TRANSMITTER_H

#include "SPIDevice.h"
#include "Quaternion.h"
#include "Timer.h"

#define COMMAND_NOP 0xFF00;

/*******************************************************************************
 * FloatBuf Union
 * This union is present for the purpose of converting a single precision,
 * IEEE-754 floating point number to an array of four chars. It will also help
 * convert an array of four chars to a float. This is present for the purpose
 * of sending floating point numbers over the SPI bus and the USB bus, of which
 * both operate with char arrays.
*******************************************************************************/
typedef union {
    float a;
    UnsignedInteger16 b[2];
}FloatBuf;

namespace HAL {
    
    class Transceiver: public SPIDevice {
    public:
        
        Transceiver(ADDRESS Address, SPIBus* DeviceManager);
        ~Transceiver();
    
        bool Initialize();
        bool Update();
        
        bool LoadBuffer(Math::Quaternion Orientation,
                        SignedInteger32 Altitude,
                        UnsignedInteger16 Motor1Speed,
                        UnsignedInteger16 Motor2Speed,
                        UnsignedInteger16 Motor3Speed,
                        UnsignedInteger16 Motor4Speed,
                        UnsignedInteger16 Motor5Speed,
                        UnsignedInteger16 Motor6Speed);
        
        bool Transmit();
        bool SetTimerReference(HAL::Timer* CoreTimerReference);
        
        
    private:
        UnsignedInteger16* Buffer;
        UnsignedInteger8 BufferSize;
        UnsignedInteger8 Status;
        
        HAL::Timer* CoreTimerReference;
        
        float Q0;
        float Q1;
    };
}

#endif	/* TRANSMITTER_H */


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
        
        bool Tansmit();
        
        
    private:
        UnsignedInteger16* Buffer;
        UnsignedInteger8 BufferSize;
        
        float Q0;
        float Q1;
    };
}

#endif	/* TRANSMITTER_H */


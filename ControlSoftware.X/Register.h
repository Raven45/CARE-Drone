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


 * Created on January 11, 2016, 1:37 PM
*******************************************************************************/


#ifndef REGISTER_H
#define	REGISTER_H

#include "Object.h"

namespace HAL {

/*******************************************************************************
 * Register class
 * The Register class encapsulates a specified register in a wrapper class that 
 * provides some security over the registers. Only the System object and other 
 * HAL objects, which are all specified as ?friend classes,? will be able to have 
 * write access to a register. All other objects will have read access to the 
 * registers. The Register class has a reference to the specified register along 
 * with a public read function and a private write function.
*******************************************************************************/
class Register: public Object {
private:
    
    friend class System;
    
    Register(UnsignedInteger32 RegisterAddress);
    ~Register();
    
    bool Initialize();
    bool Update();
    
    UnsignedInteger32 GetRegister();
    bool SetRegister(UnsignedInteger32 Value);

    UnsignedInteger32* RegisterAddress;
};
}

#endif	/* REGISTER_H */


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
 * 
 * Created on January 28, 2016, 9:08 PM
*******************************************************************************/

#ifndef TIME_H
#define	TIME_H

#include "Object.h"

class Time: public Object{
    
public:
    Time();
    ~Time();
    
    bool Initialize();
    bool Update();
    
    UnsignedInteger32 Tick();
    void SetClock(  UnsignedInteger32 Seconds,
                    UnsignedInteger32 MilliSeconds,
                    UnsignedInteger32 MicroSeconds);
        
    UnsignedInteger32 GetTime();
    UnsignedInteger32 GetTime_S();
    UnsignedInteger32 GetTime_MS();
    UnsignedInteger32 GetTime_US();
    
    UnsignedInteger32 operator[](const UnsignedInteger8 i) const;
    UnsignedInteger32& operator[](const UnsignedInteger8 i);
    
    bool operator!=(const Time& B) const;
    bool operator==(const Time& B) const;
    bool operator>(const Time& B) const;
    bool operator>=(const Time& B) const;
    bool operator<(const Time& B) const;
    bool operator<=(const Time& B) const;
    
private:
    UnsignedInteger32 Seconds;
    UnsignedInteger32 MilliSeconds;
    UnsignedInteger32 MicroSeconds;
};


#endif	/* TIME_H */


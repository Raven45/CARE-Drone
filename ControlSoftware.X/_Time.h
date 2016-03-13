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

/*******************************************************************************
 * Class:  Time									
 * Description: A "Time" object is a representative of time with respect to
 *              how a human would interpret time; computers have a different
 *              perception of time, so a "translation" is required. A time
 *              object represents time with respect to seconds, milliseconds, &
 *              microseconds. The PIC32MX270 can track up to 20 microsecond
 *              intervals while still maintaining performance. A faster chip
 *              can track smaller intervals with increases in clock speed. A 
 *              50 MHz chip can track 20 uS intervals, a 100 MHz chip can track
 *              10 uS intervals, and so forth.
 * 
 *              Time objects can be directly interfaced with the physical 
 *              timer 1 peripheral. The timer 1 ISR can called the "tick"
 *              function, of which increments the stored value by 20 uS.
 
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
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


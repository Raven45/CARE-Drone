/* 
 * File:   Time.h
 * Author: aburns2
 *
 * Created on January 28, 2016, 9:08 PM
 */

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
    
private:
    UnsignedInteger32 Seconds;
    UnsignedInteger32 MilliSeconds;
    UnsignedInteger32 MicroSeconds;
};


#endif	/* TIME_H */


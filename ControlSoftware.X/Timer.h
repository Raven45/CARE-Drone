/* 
 * File:   Timer.h
 * Author: aburns2
 *
 * Created on January 28, 2016, 8:52 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "_Time.h"
#include <vector>

//Output compare period.
#define OUTPUT_COMPARE_PERIOD 20


namespace HAL {

//std::vector<Time> TimerList;

class Timer: public Object {
    
public:
    Timer();
    ~Timer();
    
    bool Initialize();
    bool Update();
    
    //Delay in seconds.
    void Delay(Time Period);
    
    //Delay in microseconds.
    void Delay_us(Time Period);
    
    //Delay in milliseconds.
    void Delay_ms(Time Period);
    
    //static Timer* GetInstance();
    
    std::vector<Time> TimerList;
    
private:
    static Timer* Instance;
    
};
}





#endif	/* TIMER_H */


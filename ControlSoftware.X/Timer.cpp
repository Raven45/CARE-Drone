#include "Timer.h"

HAL::Timer* HAL::Timer::Instance = 0;

HAL::Timer::Timer() {
    
    //Insert timer1 initialization code here.
}

HAL::Timer::~Timer() {
    
}
    
bool HAL::Timer::Initialize() {
    return true;
}

bool HAL::Timer::Update() {
    return true;
}

void HAL::Timer::Delay(Time Period) {
    
    Time DelayTimer;
    DelayTimer.SetClock(0, 0, 0);
    
    while (DelayTimer < Period);
}
    

void HAL::Timer::Delay_us(Time Period) {
    
    Time DelayTimer;
    DelayTimer.SetClock(0, 0, 0);
    
    while (DelayTimer < Period);
}
    

void HAL::Timer::Delay_ms(Time Period) {
    
    Time DelayTimer;
    DelayTimer.SetClock(0, 0, 0);
    
    while (DelayTimer < Period);
}

HAL::Timer* HAL::Timer::GetInstance() {
    
    if (!Instance) {
        
        try{
            Instance = new Timer();
        }
        catch (std::bad_alloc) {
            return 0;
        }
    }
    return Instance;
}

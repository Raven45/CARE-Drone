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

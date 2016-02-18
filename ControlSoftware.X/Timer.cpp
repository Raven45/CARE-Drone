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
*******************************************************************************/

#include "Timer.h"

//HAL::Timer* HAL::Timer::Instance = 0;

HAL::Timer::Timer() {
    
    //Configure timer 1 interrupt
    ConfigIntTimer1(T1_INT_ON  |            //Turn on timer 1 interrupt.
                    T1_INT_PRIOR_7 |        //Set to highest priority.
                    T1_INT_SUB_PRIOR_3);    //Set to highest sub-priority.
    
    //Configure Timer 1
    OpenTimer1(     T1_ON |                 //Turn on timer 1.
                    T1_IDLE_CON |           //Continue on idle.
                    T1_PS_1_1,              //Set post scale to 1.
                    0x300);                  //Set timer 1 period to 1 us.
    
    ConfigIntTimer2(T2_INT_ON |             //Turn on timer 2 interrupt.
                    T2_INT_PRIOR_6);        //Set interrupt priority
    
    Time * RollTimer = new Time();
    Time * PitchTimer = new Time();
    Time * YawTimer = new Time();
    Time * ThrottleTimer = new Time();
    Time * DeltaTimer = new Time();
    Time * RCOutputTimer = new Time();
    
    TimerList.push_back(*RollTimer);   
    TimerList.push_back(*PitchTimer);  
    TimerList.push_back(*YawTimer);  
    TimerList.push_back(*ThrottleTimer);
    TimerList.push_back(*DeltaTimer);  
    TimerList.push_back(*RCOutputTimer);  
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

//HAL::Timer* HAL::Timer::GetInstance() {
//    
//    if (!Instance) {
//        
//        try{
//            Instance = new Timer();
//        }
//        catch (std::bad_alloc) {
//            return 0;
//        }
//    }
//    return Instance;
//}

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

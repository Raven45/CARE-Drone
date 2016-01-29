/*******************************************************************************
 * Copyright 2016   Aaron Burns,
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


 * Created on January 11, 2016, 10:27 AM
*******************************************************************************/

/*******************************************************************************
 * PIC32MX Configuration bits
 * Target Platform: PIC32MX270F256D
 * Target clock speed: 48 Mhz with 8 Mhz external crystal and 48 MHz USB clock
 * 
 * Note: Unless you've spent the last two weeks pouring over the datasheet
 * for the target AND and have poured over the detailed design documentation
 * for the CARE-Drone flight computer, you should not mess with the following
 * configurations bits, or anything in this file for that matter. In fact, shoo, 
 * go away before we smite you with a dangling pointer!
 * 
 * Required reading:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/60001168F.pdf
 * http://ww1.microchip.com/downloads/en/DeviceDoc/PIC32MX_FRM_Sect32_Config_61124C.pdf
*******************************************************************************/
#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config FPLLMUL  = MUL_24        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_2         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect

#include "System.h"
#include "Quaternion.h"


int main(int argc, char** argv) {
    
    //Create system object and attempt to initialize system.
    while ( System::GetInstance()->InitializeSystem() == false ) {
        //error condition.
        //Do something, anything.
    }
    
    //While system is running.
    while (System::GetInstance()->ClearToProceed()) {
        
        //Call the real main function.
        System::GetInstance()->Main();
    }
    
    return 0;
}

extern "C" {

#ifdef TIMER_H    
    
    void __ISR (_TIMER_1_VECTOR, IPL1AUTO) _Timer1Handler(void) {

        INTClearFlag(INT_T1);
        
        UnsignedInteger8 i = 0;
        while (i < HAL::Timer::GetInstance()->TimerList.size()) {
            HAL::Timer::GetInstance()->TimerList[i].Tick();
        }
    }
#endif
}

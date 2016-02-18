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


int Timer2OverflowCounter = 0;

void InitializeIO();

System CARE_Drone;

int main(int argc, char** argv) {
    
    InitializeIO();

    //Create system object and attempt to initialize system.
    while ( CARE_Drone.InitializeSystem() == false ) {
        //error condition.
        //Do something, anything.
    }

    //While system is running.
    while (CARE_Drone.ClearToProceed()) {

        //Call the real main function.
        CARE_Drone.Main();
    }

    
    return 0;
}

void InitializeIO() {
    
    //Initialize the slave select lines.
    PORTSetPinsDigitalOut(IOPORT_A, BIT_0);     //Slave select 0: Accelerometer
    PORTSetPinsDigitalOut(IOPORT_A, BIT_1);     //Slave select 1: Gyroscope
    PORTSetPinsDigitalOut(IOPORT_B, BIT_0);     //Slave select 2: Magnetometer
    PORTSetPinsDigitalOut(IOPORT_B, BIT_1);     //Slave select 3: Altimeter
    PORTSetPinsDigitalOut(IOPORT_B, BIT_2);     //Slave select 4: Motor 1
    PORTSetPinsDigitalOut(IOPORT_B, BIT_3);     //Slave select 5: Motor 2
    PORTSetPinsDigitalOut(IOPORT_C, BIT_0);     //Slave select 6: Motor 3
    PORTSetPinsDigitalOut(IOPORT_C, BIT_1);     //Slave select 7: Motor 4
    PORTSetPinsDigitalOut(IOPORT_C, BIT_2);     //Slave select 8: Motor 5
    PORTSetPinsDigitalOut(IOPORT_C, BIT_3);     //Slave select 9: Motor 6
    
    //Set the slave select lines to initial state.
    PORTSetBits(IOPORT_A, BIT_0);
    PORTSetBits(IOPORT_A, BIT_1);
    PORTSetBits(IOPORT_B, BIT_0);
    PORTSetBits(IOPORT_B, BIT_1);
    PORTSetBits(IOPORT_B, BIT_2);
    PORTSetBits(IOPORT_B, BIT_3);
    PORTSetBits(IOPORT_C, BIT_0);
    PORTSetBits(IOPORT_C, BIT_1);
    PORTSetBits(IOPORT_C, BIT_2);
    PORTSetBits(IOPORT_C, BIT_3);
    
    //Initialize the RC channels
    PORTSetPinsDigitalIn(IOPORT_B, BIT_13);     //RC channel 1
    PORTSetPinsDigitalIn(IOPORT_B, BIT_9);      //RC channel 2
    PORTSetPinsDigitalIn(IOPORT_B, BIT_15);     //RC channel 3
    PORTSetPinsDigitalIn(IOPORT_A, BIT_4);      //RC channel 4
    PORTSetPinsDigitalIn(IOPORT_A, BIT_8);      //RC channel 5
    PORTSetPinsDigitalOut(IOPORT_C, BIT_7);     //RC channel 6
    PORTClearBits(IOPORT_C, BIT_7);             //Set RC6 to LOW
    
    //Initialize Cargo control pins
    PORTSetPinsDigitalOut(IOPORT_C, BIT_4);     //Cargo+
    PORTSetPinsDigitalOut(IOPORT_C, BIT_5);     //Cargo-
    PORTClearBits(IOPORT_C, BIT_4);             //Set Cargo+ to LOW
    PORTClearBits(IOPORT_C, BIT_5);             //Set Cargo- to LOW
    
    //Configure serial bus
    PORTSetPinsDigitalOut(IOPORT_B, BIT_8);     //Master out, slave in
    PORTSetPinsDigitalOut(IOPORT_B, BIT_14);    //SCK
    PORTSetPinsDigitalIn(IOPORT_B, BIT_5);      //Master in, slave out
    PPSOutput(2, RPB8, SDO1);                   //Set SDO1 to B8
    PPSInput(2, SDI1, RPB5);                    //Set SDI1 to B5
    
    //Initialize unused ports as outputs and set to ground
    PORTSetPinsDigitalOut(IOPORT_A, BIT_9);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_4);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_7);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_6);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_8);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_9);
    PORTClearBits(IOPORT_A, BIT_9);
    PORTClearBits(IOPORT_B, BIT_4);
    PORTClearBits(IOPORT_B, BIT_7);
    PORTClearBits(IOPORT_C, BIT_6);
    PORTClearBits(IOPORT_C, BIT_8);
    PORTClearBits(IOPORT_C, BIT_9);
    
    //Configure Global interrupts
    INTEnableSystemMultiVectoredInt();
}

extern "C" {

#ifdef TIMER_H    
    
    void __ISR (_TIMER_1_VECTOR, IPL7AUTO) _Timer1Handler(void) {

        INTClearFlag(INT_T1);
        
        UnsignedInteger8 i = 0;
        while (i < CARE_Drone.CoreTimer.TimerList.size()) {
            CARE_Drone.CoreTimer.TimerList[i].Tick();
            i++;
        }
    }
    
    //Delay timer for cargo subsystem.
    void __ISR (_TIMER_2_VECTOR, IPL1AUTO) _Timer2Handler(void) {

        //Clear interrupt flag.
        INTClearFlag(INT_T2);
        
        if (Timer2OverflowCounter >= 2000) {
            PORTClearBits(IOPORT_C, BIT_4);
            PORTClearBits(IOPORT_C, BIT_5);
            CloseTimer2();
            Timer2OverflowCounter = 0;
        }
        else {
            Timer2OverflowCounter++;
        }
    }
    
    void __ISR (_CHANGE_NOTICE_VECTOR, IPL1AUTO) _CNHandler(void) {
        
        INTClearFlag(INT_CNA);
        
    }
#endif
}

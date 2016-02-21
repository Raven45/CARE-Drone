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

#define _PPSUnlock()    {SYSKEY=0x0;SYSKEY=0xAA996655;SYSKEY=0x556699AA;CFGCONbits.IOLOCK=0;} 
#define _PPSLock()      {SYSKEY=0x0;SYSKEY=0xAA996655;SYSKEY=0x556699AA;CFGCONbits.IOLOCK=1;}

#include "System.h"
#include "Quaternion.h"

unsigned int Roll = 0;

int Timer2OverflowCounter = 0;

void InitializeIO();
void ConfigurePPS();
void InitializeSlaveSelect();
void InitializeRC();
void InitializeCargoControl();
void InitializeSerial();
void SetUnusedPorts();

//<editor-fold defaultstate="collapsed" desc="Interrupt Vector Table">
extern "C" {
    void __ISR (_TIMER_1_VECTOR, IPL7AUTO) _Timer1Handler(void);
    void __ISR (_TIMER_2_VECTOR, IPL6AUTO) _Timer2Handler(void);
    void __ISR (_TIMER_3_VECTOR, IPL0AUTO) _Timer3Handler(void);
    void __ISR (_TIMER_4_VECTOR, IPL0AUTO) _Timer4Handler(void);
    void __ISR (_TIMER_5_VECTOR, IPL0AUTO) _Timer5Handler(void);
    void __ISR (_INPUT_CAPTURE_1_VECTOR, IPL4AUTO) _InputCapture1Handler(void);
    void __ISR (_INPUT_CAPTURE_2_VECTOR, IPL4AUTO) _InputCapture2Handler(void);
    void __ISR (_INPUT_CAPTURE_3_VECTOR, IPL4AUTO) _InputCapture3Handler(void);
    void __ISR (_INPUT_CAPTURE_4_VECTOR, IPL4AUTO) _InputCapture4Handler(void);
    void __ISR (_INPUT_CAPTURE_5_VECTOR, IPL4AUTO) _InputCapture5Handler(void);
    void __ISR (_OUTPUT_COMPARE_1_VECTOR , IPL4AUTO) _OutputCompare1Handler(void);
}
//</editor-fold>

//Global system object. This needs to be global so that interrupts can
//access time critical data.
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
    
    //ConfigurePPS();
    //InitializeSlaveSelect();
    //InitializeRC();
    InitializeCargoControl();
    //InitializeSerial();
    //SetUnusedPorts();

    //Configure Global interrupts
    INTEnableSystemMultiVectoredInt();
    IPC7bits.USBIP = 4; /* Interrupt priority, must set to != 0. */
    __asm volatile("ei");
}

void ConfigurePPS() {
    
    //Unlock pps.
    _PPSUnlock();
    
    #if defined(__32MX270F256D__)
        PPSOutput(2, RPB8, SDO1);                   //Set SDO1 to B8
        PPSInput(2, SDI1, RPB5);                    //Set SDI1 to B5

        PPSInput(3, IC1, RPB13);                    //Set IC1 to B13
        PPSInput(4, IC2, RPB9);                     //Set IC2 to B8
        PPSInput(1, IC4, RPB15);                    //Set IC3 to B15
        PPSInput(3, IC5, RPA4);                     //Set IC4 to A4
        PPSInput(2, IC3, RPA8);                     //Set IC5 to A8
        PPSOutput(1, RPC7, OC1);                    //Set OC1 to C7
    #elif defined(__32MX270F256B__)

    #endif
    
    
    
    //Re-lock the pps.
    _PPSLock();
}

void InitializeSlaveSelect() {
    
    #if defined(__32MX270F256D__)
    
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
    
    #elif defined(__32MX270F256B__)
        
    #endif
}

void InitializeRC() {
    
    #if defined(__32MX270F256D__)

        //Initialize the RC channels
        PORTSetPinsDigitalIn(IOPORT_B, BIT_13);     //RC channel 1
        PORTSetPinsDigitalIn(IOPORT_B, BIT_9);      //RC channel 2
        PORTSetPinsDigitalIn(IOPORT_B, BIT_15);     //RC channel 3
        PORTSetPinsDigitalIn(IOPORT_A, BIT_4);      //RC channel 4
        PORTSetPinsDigitalIn(IOPORT_A, BIT_8);      //RC channel 5
        PORTSetPinsDigitalOut(IOPORT_C, BIT_7);     //RC channel 6
        PORTClearBits(IOPORT_C, BIT_7);             //Set RC6 to LOW

        //Configure input capture
        OpenCapture1(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        OpenCapture2(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        OpenCapture3(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        OpenCapture4(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        OpenCapture5(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        ConfigIntCapture1(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
        ConfigIntCapture2(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
        ConfigIntCapture3(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
        ConfigIntCapture4(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
        ConfigIntCapture5(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
    
    #elif defined(__32MX270F256B__)
        
    #endif
}

void InitializeCargoControl() {
    
    #if defined(__32MX270F256D__)

        //Initialize Cargo control pins
        PORTSetPinsDigitalOut(IOPORT_C, BIT_4);     //Cargo+
        PORTSetPinsDigitalOut(IOPORT_C, BIT_5);     //Cargo-
        PORTClearBits(IOPORT_C, BIT_4);             //Set Cargo+ to LOW
        PORTClearBits(IOPORT_C, BIT_5);             //Set Cargo- to LOW
    
    #elif defined(__32MX270F256B__)
        
        //Initialize Cargo control pins
        PORTSetPinsDigitalOut(IOPORT_A, BIT_0);     //Cargo+
        PORTSetPinsDigitalOut(IOPORT_A, BIT_1);     //Cargo-
        PORTClearBits(IOPORT_A, BIT_0);             //Set Cargo+ to LOW
        PORTClearBits(IOPORT_A, BIT_1);             //Set Cargo- to LOW
    #endif
}

void InitializeSerial() {
    
    #if defined(__32MX270F256D__)

        //Configure serial bus
        PORTSetPinsDigitalOut(IOPORT_B, BIT_8);     //Master out, slave in
        PORTSetPinsDigitalOut(IOPORT_B, BIT_14);    //SCK
        PORTSetPinsDigitalIn(IOPORT_B, BIT_5);      //Master in, slave out
    #elif defined(__32MX270F256B__)
        
    #endif
    
    
}

void SetUnusedPorts() {
    
    #if defined(__32MX270F256D__)
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
    #elif defined(__32MX270F256B__)

    #endif
    
    
}

extern "C" {

#ifdef TIMER_H    
    
    
    void __ISR (_TIMER_1_VECTOR, IPL7AUTO) _Timer1Handler(void) {

        INTClearFlag(INT_T1);
        
        CARE_Drone.CoreTimer.DeltaTimer.Tick();
        CARE_Drone.CoreTimer.DelayTimer.Tick();
    }
    
    //Delay timer for cargo subsystem.
    void __ISR (_TIMER_2_VECTOR, IPL6AUTO) _Timer2Handler(void) {

        //Clear interrupt flag.
        INTClearFlag(INT_T2);
        
//        if (Timer2OverflowCounter >= 2000) {
        if (CARE_Drone.CargoIsReleased)    
            #if defined(__32MX270F256D__)
                PORTClearBits(IOPORT_C, BIT_4);
                PORTClearBits(IOPORT_C, BIT_5);
            #elif defined(__32MX270F256B__)
                PORTClearBits(IOPORT_A, BIT_0);
                PORTClearBits(IOPORT_A, BIT_1);
            #endif
            
            //CloseTimer2();
//            Timer2OverflowCounter = 0;
//        }
//        else {
//            Timer2OverflowCounter++;
//        }
    }
    
    void __ISR (_TIMER_3_VECTOR, IPL0AUTO) _Timer3Handler(void) {
        INTClearFlag(INT_T3);
    }
    
    void __ISR (_TIMER_4_VECTOR, IPL0AUTO) _Timer4Handler(void) {
        INTClearFlag(INT_T4);
    }
    
    void __ISR (_TIMER_5_VECTOR, IPL0AUTO) _Timer5Handler(void) { 
        INTClearFlag(INT_T5); 
    }
    
    void __ISR (_INPUT_CAPTURE_1_VECTOR , IPL4AUTO) _InputCapture1Handler(void) {
        
        INTClearFlag(INT_IC1);
        
        //Rising edge
        if (PORTBbits.RB13 == 1) {
            WriteTimer3(0);
            unsigned int trash = IC1BUF;
        }
        //Falling edge
        else {
            CARE_Drone.SetRollInput(IC1BUF);
        }
    }
    
    void __ISR (_INPUT_CAPTURE_2_VECTOR , IPL4AUTO) _InputCapture2Handler(void) {
        
        INTClearFlag(INT_IC2);
        
        //Rising edge
        if (PORTBbits.RB9 == 1) {
            WriteTimer3(0);
            unsigned int trash = IC2BUF;
        }
        //Falling edge
        else {
            CARE_Drone.SetRollInput(IC2BUF);
        }
    }
    
    void __ISR (_INPUT_CAPTURE_3_VECTOR , IPL4AUTO) _InputCapture3Handler(void) {
        
        INTClearFlag(INT_IC3);
        
        //Rising edge
        if (PORTBbits.RB15 == 1) {
            WriteTimer3(0);
            unsigned int trash = IC3BUF;
        }
        //Falling edge
        else {
            CARE_Drone.SetRollInput(IC3BUF);
        }
    }
    
    void __ISR (_INPUT_CAPTURE_4_VECTOR , IPL4AUTO) _InputCapture4Handler(void) {
        
        INTClearFlag(INT_IC4);
        
        //Rising edge
        if (PORTAbits.RA4 == 1) {
            WriteTimer3(0);
            unsigned int trash = IC4BUF;
        }
        //Falling edge
        else {
            CARE_Drone.SetRollInput(IC4BUF);
        }
    }
    
    void __ISR (_INPUT_CAPTURE_5_VECTOR , IPL4AUTO) _InputCapture5Handler(void) {
        
        INTClearFlag(INT_IC5);
        
//        //Rising edge
//        if (PORTAbits.RA8 == 1) {
//            WriteTimer3(0);
//            unsigned int trash = IC5BUF;
//        }
//        //Falling edge
//        else {
//            CARE_Drone.SetRollInput(IC5BUF);
//        }

    }
    
    void __ISR (_OUTPUT_COMPARE_1_VECTOR , IPL4AUTO) _OutputCompare1Handler(void) {
        INTClearFlag(INT_OC1);
    }
#endif
}

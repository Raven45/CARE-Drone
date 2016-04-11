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
#pragma config FUSBIDIO = OFF           // USB USID Selection (Controlled by Port Function)
#pragma config FVBUSONIO= OFF           // USB VBUS ON Selection (Controlled by Port Function)
#pragma config JTAGEN   = OFF           //Turn the JTAG port off to allow SPI.
#pragma config ICESEL   = ICS_PGx4      //Use debug port 4.

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


/*******************************************************************************
 * Subroutine:  Main										
 * Description: The main point of entry for the PIC's firmware. Main
 *              will initialize the system and then run the System.Main 
 *              function.
 
 * Prerequisites:   All configuration bits must be configured.
 *                  CPU speed must be set to 48 MHz.
 *                  USB PLL must be configured for 48 MHz clock.
 *                  Debug port must be set to PGEx4.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 01-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
int main(int argc, char** argv) {
     
    InitializeIO();

    //Create system object and attempt to initialize system.
    CARE_Drone.InitializeSystem();

    //While system is running.
    while (true) {

        //Call the real main function.
        CARE_Drone.Main();
    }

    
    return 0;
}

/*******************************************************************************
 * Subroutine:  InitializeIO										
 * Description: This function will initialize and configure all physical
 *              pins of the target PIC32MX. This will also turn on
 *              global interrupts.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
void InitializeIO() {
    
    ConfigurePPS();
    InitializeSlaveSelect();
    InitializeRC();
    InitializeCargoControl();
    InitializeSerial();
    SetUnusedPorts();

    //Configure Global interrupts
    INTEnableSystemMultiVectoredInt();
    IPC7bits.USBIP = 4; /* Interrupt priority, must set to != 0. */
    __asm volatile("ei");
}

/*******************************************************************************
 * Subroutine:  ConfigurePPS									
 * Description: This function will perform a system unlock and configure
 *              the peripheral pin select subsystem of the PIC32MX.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
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
        
        PPSOutput(2, RPB8, SDO1);                   //Set SDO1 to B8
        PPSInput(2, SDI1, RPB5);                    //Set SDI1 to B5
        
        PPSInput(3, IC1, RPB13);                    //Set IC1 to B13
        PPSInput(4, IC2, RPB9);                     //Set IC2 to B8
        PPSInput(1, IC4, RPB15);                    //Set IC3 to B15
        PPSInput(3, IC5, RPA4);                     //Set IC4 to A4
    #endif
 
    //Re-lock the pps.
    _PPSLock();
}

/*******************************************************************************
 * Subroutine:  InitializeSlaveSelect										
 * Description: This function will initialize the slave select pins as
 *              general purpose I/O. All slave select pins will be set to 
 *              active high states in order to implement SPI mode 0.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
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

        PORTSetPinsDigitalOut(IOPORT_B, BIT_2);     //Slave select 4: Motor 1
        PORTSetBits(IOPORT_B, BIT_2);
    #endif
}

/*******************************************************************************
 * Subroutine:  InitializeRC									
 * Description: This function will initialize and configure the physical pins
 *              and interrupts required to implement RC inputs. This will
 *              setup timer 3 as the interrupt capture timer and will configure
 *              input capture channels 1-5 for inputs.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
void InitializeRC() {
    
    OpenTimer3(     T3_ON |                 //Turn on timer 1.
                    T3_IDLE_CON |           //Continue on idle.
                    T3_PS_1_4,
                    0xFFFF);                  //Set timer 1 period to 1 us.
    
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
        
        //Initialize the RC channels
        PORTSetPinsDigitalIn(IOPORT_B, BIT_13);     //RC channel 1
        PORTSetPinsDigitalIn(IOPORT_B, BIT_9);      //RC channel 2
        PORTSetPinsDigitalIn(IOPORT_B, BIT_15);     //RC channel 3
        PORTSetPinsDigitalIn(IOPORT_A, BIT_4);      //RC channel 4

        //Configure input capture
        OpenCapture1(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        OpenCapture2(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        OpenCapture3(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        OpenCapture4(IC_ON | IC_EVERY_EDGE | IC_TIMER3_SRC);
        ConfigIntCapture1(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
        ConfigIntCapture2(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
        ConfigIntCapture3(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
        ConfigIntCapture4(IC_INT_ON | IC_INT_PRIOR_4 | IC_INT_SUB_PRIOR_3);
    #endif
}

/*******************************************************************************
 * Subroutine:  InitializeCargoControl										
 * Description: This function will initialize and configure the outpus
 *              needed for the cargo control subsystem.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
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

/*******************************************************************************
 * Subroutine:  InitializeSerial										
 * Description: This function will initialize and configure the SPI interface.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
void InitializeSerial() {
    
    //#if defined(__32MX270F256D__)

        //Configure serial bus
//        PORTSetPinsDigitalOut(IOPORT_B, BIT_8);     //Master out, slave in
//        PORTSetPinsDigitalOut(IOPORT_B, BIT_14);    //SCK
//        PORTSetPinsDigitalIn(IOPORT_B, BIT_5);      //Master in, slave out
    //#elif defined(__32MX270F256B__)
        
    //#endif
    
    
}

/*******************************************************************************
 * Subroutine:  SetUnusedPorts									
 * Description: This function will turn on the PIC32MX's internal pull-down
 *              resistors for unused pins; all unused I/O will be set to
 *              active low operation.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip PIC32MX270F256D/PIC32MX270F256B
 * Author:          Aaron Burns.
 * Change-log:
 * 02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
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
        unsigned int Capture = IC1BUF;
        
        //Rising edge
        if (PORTBbits.RB13 == 1) {
            WriteTimer3(0);
            unsigned int trash = IC1BUF;
        }
        //Falling edge
        else {
            CARE_Drone.SetYawInput(Capture);
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
            CARE_Drone.SetPitchInput(IC2BUF);
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
            CARE_Drone.SetThrottleInput(IC3BUF);
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
        
        //Rising edge
        if (PORTAbits.RA8 == 1) {
            WriteTimer3(0);
            unsigned int trash = IC5BUF;
        }
        //Falling edge
        else {
            CARE_Drone.SetThrottleInput(IC5BUF);
        }

    }
    
    void __ISR (_OUTPUT_COMPARE_1_VECTOR , IPL4AUTO) _OutputCompare1Handler(void) {
        INTClearFlag(INT_OC1);
    }
#endif
}

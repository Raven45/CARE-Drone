/*******************************************************************************
Copyright 2016 Department of Engineering, Harding University
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
  
    http://www.apache.org/licenses/LICENSE-2.0
 
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 
Created on January 15, 2016, 9:14 PM
*******************************************************************************/

/*******************************************************************************
"but those who hope in the Lord will renew their strength. They will soar on 
wings like eagles; they will run and not grow weary, they will walk and
not be faint." -Isaiah 40:31
*******************************************************************************/

/*******************************************************************************
CARE-Drone ESC Control Software

Target Microcontroller: 	dsPIC30F2010 w/ 8MHz crystal oscillator
Software Author:			Aaron Burns
Hardware Designer(s):		Joshua Donaway
							Aaron Burns
Version:					0.0.0.5
Date:						February 2016
School:						Harding University, Department of Engineering.
Class:						Sr. Design.

This firmware is meant for the CARE-Drone's ESC (Eletronic Speed Controller).
The ESC for the CARE-Drone is designed to be operated as a SPI (Serial
peripherial interface) slave to another microprocessor; it is not meant to
act as a standalone unit. The other microprocessor can be an Arduino, S12,
another PIC, etc; the requirement for the other microprocessor is that it can
operate at 100 KHz - 20 MHz in SPI Mode 0 as a master. This code has been 
tested, and is rated for up to 20 MHz operation through SPI.

This firmware essentially acts as a command interpreter. The ESC will boot,
and then wait for commands to be received from a SPI master. The ESC, at
command, can start the motor in open loop mode, run the motor in closed loop
mode, calculate the speed of the motor, and brake the motor. Technically 
speaking, this ESC can be used for far more purposes than simply a drone, but
rather it can be used for any project that uses a BLDC (brushless direct 
current) motor.

This firmware utilizes Back-EMF sampling in order to control an attached BLDC.
Back-EMF is simply the electrical potential measured on a motor lead that is
undriven; essentially the voltage due to the motor acting as a generator.
Care must be taken with the hardware; the hardware must somehow isolate the 
PIC's ADC inputs away from the motor while still allowing the ADC to sample
the motor potentials. Capacitors and voltage followers both work well here.
In fact, I've seen one design where someone used a capacitor sandwiched by
two voltage followers. Our project used a 330 uF polarized capacitor, which
works well if you connect the negative lead to the ADC input; this prevents 
both the motor and capacitor from discharging current through the ADC.

The Back-EMF allows the ESC to do three things:
1) It allows the ESC to operate the motor in closed loop mode by using the
Back-EMF to calculate the time until the next motor commutation.
2) It allows the ESC to tell what physical sector the motor is in in order
to determine the correct firing pattern.
3) It allows the ESC to calculate the speed of the motor.

It could also allow a PID loop to be set for more precise control, but this
was not implemented in our project due to time contrainsts. In short, just
the ESC alone is enough to fulfill any computer engineering student's 
obligations for Sr Design at most ABET schools.

The firmware relies on hardware filtering; there is no software based
filter used here. The hardware filter must filter the PWM signal out of the
back-EMF signals. Fortunately, there is some room for error, as the code
here is very resilent to noisy and malformed back-EMF, though it's not perfect.
This design decision was made because we felt like we could implement a hardware
filter with a smaller filter delay than a 4th order Butterworth implemented
in software. Also, the 4th order Butterworth didn't work.

If a Sr Design team ever picks this up to use for a future project, I have a 
suggested change to the ESC design: port the code over to a dsPIC33EP 
microprocessor with two SPI ports. Use one as the slave port to the SPI master
and use the other SPI port to link to an external ADC that is at least 16 bits
in resolution; a higher resolution ADC means that the ESC will spend less time
and power in its very harsh ramp up routine and that it can run the motor at
a lower speed without stalling.
*******************************************************************************/

#include "p30fxxxx.h"
#include <xc.h>
#define FOSC 64000000UL
#define FCY 16000000UL
#include <libpic30.h>
#include <p30F2010.h>
#include <spi.h>

//<editor-fold defaultstate="collapsed" desc="Macros">

//Set configuration to use external oscillator with x8 PLL for 16 MHz clock.
_FOSC(PRI & XT_PLL8)
_FWDT(WDT_OFF)
        

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1<<(pos)))
#define SET_BIT(var,pos)   ((var) |= (1<<(pos)))

#define OUTPUT  0
#define INPUT   1        
        
#define GetSystemClock() (FCY)    
#define _EmergencyStop() {LATDbits.LATD1 = 0;LATDbits.LATD0 = 1;OVDCON = 0;}        
        
#define ENABLE_SPI        
#define ENABLE_SPI_LOOPBACK
//#define ENABLE_THROTTLE_TEST

#define bool short int        
#define false 0x00
#define true 0xFF
        
#define SPI_COMMAND_BITMASK     	0x7F00
#define SPI_DATA_BITMASK        	0x00FF
        
#define SPI_COMMAND_START_MOTOR     0x01
#define SPI_COMMAND_STOP_MOTOR      0x02
#define SPI_COMMAND_SET_THROTTLE    0x03
#define SPI_COMMAND_GET_THROTTLE    0x04
#define SPI_COMMAND_GET_KP			0x05
#define SPI_COMMAND_SET_KP			0x06
#define SPI_COMMAND_GET_KI			0x07
#define SPI_COMMAND_SET_KI			0x08
#define SPI_COMMAND_GET_KD			0x09
#define SPI_COMMAND_SET_KD			0x0A
#define SPI_COMMAND_GET_SPEED     	0x0B
#define SPI_COMMAND_GET_PHASE     	0x0C
#define SPI_COMMAND_SET_PHASE		0x0D
#define SPI_COMMAND_GET_POLES		0x0E
#define SPI_COMMAND_SET_POLES		0x0F
#define SPI_COMMAND_GET_PWM			0x10
#define SPI_COMMAND_SET_COMP		0x11
#define SPI_COMMAND_SET_IND			0x12
#define SPI_COMMAND_MOTOR_STARTED	0x13
#define SPI_COMMAND_ENABLE_PID		0x14
#define SPI_COMMAND_DISABLE_PID		0x15

#define PWM1L_FORCE             	0x0001
#define PWM1H_FORCE             	0x0002     
#define PWM2L_FORCE             	0x0004
#define PWM2H_FORCE             	0x0008
#define PWM3L_FORCE             	0x0010
#define PWM3H_FORCE             	0x0020
        
#define PWM1L_ON                	0x0100
#define PWM1H_ON                	0x0200
#define PWM2L_ON                	0x0400
#define PWM2H_ON                	0x0800        
#define PWM3L_ON                	0x1000
#define PWM3H_ON                	0x2000   
        
//PWM frequency in hertz
#define PWM_FREQ 20000
        
//</editor-fold>
        
//<editor-fold defaultstate="collapsed" desc="Global variables">        
unsigned int PWM_Resolution = 0;
unsigned int PDC_MAX = 0;
int CurrentSector = 5;

struct Global {
	
    bool MotorIsStarted;					//false=not started, 
    										//true=started
    bool Direction; 						//true=clockwise, 
    										//false=counter-clockwise.
    bool ComplimentaryPWM; 					//false=independent switching
    										//true=complimentary switching.
    bool BipolarSwitching;					//false=unipolar switching
    										//true=bipolar switching
    										//Not implemented
    unsigned short int NumPoles;			//Number of motor poles.
    										//Used for determining current speed.
    unsigned long long int CurrentSpeed;	//Current speed of motor in rpm.
    unsigned short int FilterDelay;			//Delay of the physical filter in 
    										//clock cycles.
    unsigned short int ADCProc;				//ADC processing delay in clock cycles
    unsigned short int PhaseAdvance;		//Phase advance in clock cycles.
    unsigned int ZeroCrossPoint;			//Virtual motor neutral point.
    unsigned int CurrentThrottle;			//Current throttle of the motor.
    unsigned int CommutationTime;			//Time until next commutation.
    										//Measured in clock cycles.
} Globals;

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="prototypes">

void InitializeGlobals();
void InitializePWM();
void InitializeADC();
void InitializeSPI();
unsigned int VerifyMessage(unsigned int message);
unsigned int EncodeParity(unsigned int message);
unsigned int ReadSPI(unsigned int data);
void WriteSPI(unsigned int data);
void CloseSPI();
void SetThrottle(unsigned int Throttle);
unsigned int GetThrottle();
void SectorChange();
void AlignMotor();
void StartupMotor();
void StopMotor();

//SPI command prototypes
void Command_Start(); 
void Command_Stop();
void Command_SetThrottle(unsigned int Throttle);
void Command_GetThrottle();
void Command_GetSpeed();
void Command_GetPhaseAdvance();
void Command_SetPhaseAdvance();
void Command_GetNumPoles();
void Command_SetNumPoles();
void Command_GetPWMSwitching();
void Command_SetComplimentarySwitching();
void Command_SetIndependentSwitching();
void Command_IsMotorStarted();
void Command_EnablePID();
void Command_DisablePID();

//dsPIC30f trap table. Support on all dsPIC30f's.
void __attribute__((__interrupt__)) _OscillatorFail(void);
void __attribute__((__interrupt__)) _AddressError(void);
void __attribute__((__interrupt__)) _StackError(void);
void __attribute__((__interrupt__)) _MathError(void);
void __attribute__((__interrupt__)) _AltOscillatorFail(void);
void __attribute__((__interrupt__)) _AltAddressError(void);
void __attribute__((__interrupt__)) _AltStackError(void);
void __attribute__((__interrupt__)) _AltMathError(void);

//The entire dsPIC30f interrupt vector table. Some not supported by target.
void __attribute__((__interrupt__, no_auto_psv)) _INT0Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC1Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC2Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _ADCInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _NVMInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _NVMInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _SI2CInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _MI2CInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC7Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC8Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC3Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC4Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _SPI2Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _C1Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC3Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC4Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC5Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _IC6Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC5Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC6Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC7Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _OC8Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _INT3Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _INT4Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _C2Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _PWMInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _QEIInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _DCIInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _LVDInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _FLTAInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _FLTBInterrupt(void);

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Functions">


/*******************************************************************************
 * Subroutine:  InitializeGlobals	
 * Description: Set all global variables to some initial value. Right now, for
 								the purposes of this particular project (Harding Sr Design 2016)
 								these default values are hard coded. For an actual commercial 
 								project, or if a future Sr Design team wanted to continue the
 								project, then these values would ideally be retrieved from 
 								an EEPROM chip connected via SPI.
 
 * Prerequisites:   A properly configured instruction clock of 16 MHz.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-2016       	Original code for CARE-Drone ESC.
*******************************************************************************/
void InitializeGlobals () {
    
    Globals.MotorIsStarted = false;		//Initialize to motor is not started.
    Globals.Direction = false; 			//Clockwise rotation.
    Globals.ComplimentaryPWM = true; 	//Enable complimentary PWM.
    Globals.BipolarSwitching = false;	//Enable unipolar switching.
    Globals.NumPoles = 14;				//14 for 470 Kv, 22 for 580 Kv motors.
    Globals.CurrentSpeed = 0;			//Initialize current speed to zero.
    Globals.FilterDelay = 0; 			//Initialize to 0.
    Globals.ADCProc = 0; 				//Initialize to 0.
    Globals.PhaseAdvance = 0x7F;		//Set phase advance to roughly 8 ms.
    Globals.CommutationTime = 0;		//Initialize to 0.
}

/*******************************************************************************
 * Subroutine:  InitializePWM											
 * Description: Performs the initial steps to start the pulse-width modulation
 *              (PWM) subsystem for the dsPIC30F2010. In the CARE-Drone 
 *              implementation, this function will initialize port E as 
 *              six channel, complementary PWM output at 20 MHz.
 
 * Prerequisites:   A properly configured instruction clock of 16 MHz.
 * Input:           None.
 * Output:          PDC_MAX:    a variable containing the maximum possible 
 *                              duty cycle.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-02-2016       Original code for CARE-Drone ESC.
*******************************************************************************/
void InitializePWM() {
    
    //Set PortE to digital out.
    TRISE = OUTPUT;
    
    //Calculate the PWM frequency. 
    PTPER = FCY/PWM_FREQ -1;
    
    //Calculate the PWM resolution.
    //PWM_Resolution = log(((2*FCY)/PWM_FREQ))/log(2) +1;
    PWM_Resolution = 11;
    
    //Set the maximum duty cycle.
    short unsigned int i = 1;
    while (i < PWM_Resolution) {
        SET_BIT(PDC_MAX, i);
        i++;
    }

    PTCON = 0x0010;             //Set the PTCON register.
    PWMCON1 = 0x00FF;           //Set the PWMCON1 register.
    PTMR = 0;                   //Initialize the PWM timer.
    DTCON1 = 0x6F;              //Set dead time. 
    _PTEN = 1;                  //Fire up the PWM subsystem.
}

/*******************************************************************************
 * Subroutine:  InitializeADC											
 * Description: Performs the initial steps to start the Analog-to-digital 
 *              converter subsystem for the dsPIC30F2010. For the CARE-Drone
 *              implementation, the ADC is configured to sample back-EMF signals
 *              that are input to analog inputs AN3-5. AN3 is DC channel 1, 
 *              AN4 is DC channel 2, and AN5 is DC channel 3. The ADC should be 
 *              setup to sample at 2.5 times the PWM frequency. 
 
 * Prerequisites:   A low-pass filter, either hardware or digital.
 *                  The PWM subsystem should be initialized.
 *                  A Butterworth filter created by MDS.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-02-2016       Original code for CARE-Drone ESC.
 * 02-06-2016       Removed references to Butterworth filter. System is now
 *                  reliant on external filtering.
*******************************************************************************/ 
void InitializeADC() {

    //Manually configure the ADC subsystem. 
    TRISBbits.TRISB0 = 1;       //Set B3 as an input.
    TRISBbits.TRISB1 = 1;       //Set B4 as an input.
    TRISBbits.TRISB2 = 1;       //Set B5 as an input.
    ADPCFGbits.PCFG3 = 0;       //Enable B3 as an analog input.
    ADPCFGbits.PCFG4 = 0;       //Enable B4 as an analog input.
    ADPCFGbits.PCFG5 = 0;       //Enable B5 as an analog input.          
    ADCON1 = 0x006C;            //Enable auto sample.
                                //Enable simultaneous sampling.
                                //PWM ends sampling and starts conversion.
    ADCHS = 0x0022;             //Ch 1-3 inputs are AN3-5.
                                //Ch 0 inputs is AN2 - Needs to be changed.
    ADCSSL = 0x0000;            //Disable scanning.
    ADCON3 = 0x0002;            //TAD = 5*Tcy/2.
                                //ADC clock derived from system clock.
                                //Auto-sample time set to zero TAD
    ADCON2 = 0x0300;            //Convert channels 0-3.
    
    IPC2bits.ADIP = 6;          //Set interrupt priority.
    IFS0bits.ADIF = 0;          //Clear interrupt flag.
    //IEC0bits.ADIE = 1;
    ADCON1bits.ADON = 1;        //Enable the ADC
    
    //Allow ADC to settle.
    __delay_ms(100);
}

/*******************************************************************************
 * Subroutine:  InitializeSPI										
 * Description: Performs the initial steps to start the Serial Peripheral 
 *              Interface (SPI) in mode 0 as a four-wire slave.
 
 * Prerequisites:   None.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 12-05-2015       Original code for detailed design milestone.
 * 02-06-2016       Ported to Spring 2016 ESC design.
*******************************************************************************/ 
void InitializeSPI() {
    
    unsigned int SPICONValue;
    unsigned int SPISTATValue;
    
    CloseSPI1();
    ConfigIntSPI1(SPI_INT_EN &  SPI_INT_PRI_4);
    
    SPI1CON |= 1 << 7;
    
    CNEN1 = 0;   
    CNEN2 = 0;
    INTCON1 = 0;
    INTCON2 = 0;
  
    TRISBbits.TRISB2 = INPUT;
    
    SPICONValue  =  FRAME_ENABLE_OFF &      //Disable SPI framed mode.
                    FRAME_SYNC_OUTPUT &     //Enable synchronized output.
                    ENABLE_SDO_PIN  &      	//Enable 4-wire SPI.
                    SPI_MODE16_ON &         //Use 16 bit words.
                    SPI_SMP_OFF &           //Set edge detect mode
                    SPI_CKE_ON &           	//Set clock polarity
                    SLAVE_ENABLE_ON &       //Enable slave mode.
                    CLK_POL_ACTIVE_LOW &    //Set clock polarity.
                    MASTER_ENABLE_OFF;      //Disable master mode.
    
    SPISTATValue =  SPI_ENABLE &            //Turn the SPI port on.
                    SPI_IDLE_CON &          
                    SPI_RX_OVFLOW_CLR;
    
    OpenSPI1(SPICONValue, SPISTATValue);
    
    SPICONValue = SPI1CON;
}

/*******************************************************************************
 * Subroutine:  VerifyMessage											
 * Description: Takes an incoming, 16 bit block of data and validates the
 *              data's parity bit. Function expects odd parity.
 
 * Prerequisites:   None.
 * Input:           Message, a 16 bit value containing the message to verify.
 *                  Message should be using odd parity.
 * Output:          Returns 1 if no parity error was detected.
 *                  Returns 0 if a single bit parity error was detected.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 12-05-2015       Original code for detailed design milestone.
 * 02-06-2016       Ported to Spring 2016 ESC design.
   02-26-2016		Parity bit checking removed.
*******************************************************************************/ 
unsigned int VerifyMessage(unsigned int message) {
    
    return 1;
    
//    int NumOfOnes = 0;
//    
//    //Count the number of ones in the message.
//    int i = 0;
//    while (i < 16) {
//        if (CHECK_BIT(message, i)) {
//            NumOfOnes++;
//        }
//        i++;
//    }
//    
//    //If no parity errors were detected.
//    //Use even parity.
//    if (NumOfOnes % 2 == 0) {
//        return 1;
//    }
//    
//    //A single bit parity error was detected
//    else {
//        return 0;
//    }
}

/*******************************************************************************
 * Subroutine:  EncodeParity											
 * Description: Takes an incoming, 16 bit block of data and encodes an odd
 *              parity bit into the data.
 
 * Prerequisites:   None.
 * Input:           Message, a 16 bit value containing the message to encode.
 *                  The most significant bit should not be used for data, as
 *                  this bit will be overwritten with the parity bit.
 * Output:          Returns the data with an encoded parity bit.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 12-05-2015       Original code for detailed design milestone.
 * 02-06-2016       Ported to Spring 2016 ESC design.
*******************************************************************************/ 
unsigned int EncodeParity(unsigned int message) {
 
    int NumOfOnes = 0;
    
    //Count the number of ones in the message.
    int i = 0;
    while (i < 15) {
        if (CHECK_BIT(message, i)) {
            NumOfOnes++;
        }
        i++;
    }
    
    //Use odd parity.
    if (NumOfOnes % 2 != 0) {
        SET_BIT(message, 15);
    }
    else {
        CLEAR_BIT(message, 15);
    }
    
    return message;
}

/*******************************************************************************
 * Subroutine:  ReadSPI										
 * Description: This will both send a 16 bit value to the SPI master and will 
 *              simultaneously receive a 16 bit value from the SPI master.
 
 * Prerequisites:   InitializeSPI must be called before calling this function.
 * Input:           Message, a 16 bit value containing the message to send to
 *                  the SPI master.
 * Output:          Returns data received from the SPI master.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 12-05-2015       Original code for detailed design milestone.
 * 02-06-2016       Ported to Spring 2016 ESC design.
 * 02-26-2016		Modified per testing. Parity bit encoding and checking
 					was removed, as it was found to be unneccesary.
*******************************************************************************/ 
unsigned int ReadSPI(unsigned int data) {
    
    unsigned int Incoming = 0;
    
    //Encode an odd parity bit if needed.
    //EncodeParity(data);
    
    //Wait until we are summoned by the master.
    while (PORTBbits.RB2 != 0){}
    
    //Send response to master.
    WriteSPI1(0x7F00);
    while (SPI1STATbits.SPITBF) {}
    
    //Capture data from master.
    while (!DataRdySPI1()){}
    Incoming = ReadSPI1();
    
    return Incoming;
}

/*******************************************************************************
 * Subroutine:  WriteSPI										
 * Description: This will send a 16 bit value to the SPI master,
 
 * Prerequisites:   InitializeSPI must be called before calling this function.
 * Input:           Message, a 16 bit value containing the message to send to
 *                  the SPI master.
 * Output:          None
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-25-2016       Original code for spring 2016 ESC.
 * 02-26-2016		Modified slightly and tested with an attached Arduino Uno.
*******************************************************************************/ 
void WriteSPI(unsigned int data){
    
    //Wait until we are summoned by the master.
    while (PORTBbits.RB2 != 0){}
    
    //Reply with data.
    WriteSPI1(data);  
    while(SPI1STATbits.SPITBF);
    
    //Capture data from master in order to clear buffer.
    //discard the results from here.
    while (!DataRdySPI1()){}
    unsigned int Incoming = ReadSPI1();
}

/*******************************************************************************
 * Subroutine:  CloseSPI										
 * Description: Will close and shutdown the SPI subsystem of the dsPIC30f.
 
 * Prerequisites:   InitializeSPI must be called before calling this function.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 12-05-2015       Original code for detailed design milestone.
 * 02-06-2016       Ported to Spring 2016 ESC design.
*******************************************************************************/ 
void CloseSPI() {
    CloseSPI1();
}

/*******************************************************************************
 * Subroutine:  SetThrottle(unsigned int Throttle)									
 * Description: Sets the duty cycle for all PWM channels to the specified input.
 *              Input represents the duty cycle in percentages; an input of 
 *              "55" indicates a target duty cycle of 55%.
 
 * Prerequisites:   PWM subsystem must be initialized.
 * Input:           Throttle: an int containing the specified duty cycle ranging
 *                  from zero to 99.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
 * 02-2016          Formula for calculating PDC1 changed.
 * 02-25-2016       Changed CurrentThrottle to be set to Throttle as opposed
 *                  to being set to the value in PDC1.
*******************************************************************************/
void SetThrottle(unsigned int Throttle) {
    
    if (Throttle >=0 || Throttle < 100) {
        PDC1 = (Throttle*(PTPER+1))/40;
        PDC2 = PDC1;
        PDC3 = PDC1;
        Globals.CurrentThrottle = Throttle;
    }
}

/*******************************************************************************
 * Subroutine:  SectorChange()									
 * Description: Sets the PWM override register in order to force a commuta-
 *              tion of an attached BLDC motor. Will also keep track of
 *              the current sector the motor is in if the motor is in open
 *              loop control. This function assumes that some other method is
 *              used to track what sector the motor is in if the motor is in
 *              closed loop control.
 
 * Prerequisites:   PWM subsystem must be initialized.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
 * 02-06-2016       Modified for direction control.
*******************************************************************************/
void SectorChange() {
    
    if (Globals.MotorIsStarted == false) {
        
        if (Globals.Direction && CurrentSector == 5) {
            CurrentSector = 0;
        }
        else if (!Globals.Direction && CurrentSector == 0) {
            CurrentSector = 5;
        }
        
        else {
            if (Globals.Direction) {
                CurrentSector++;
            }
            else {
                CurrentSector--;
            }
        }
    }

    //Unipolar, independent PWM switching method. Verified working for 
    //10 - 50% duty cycles with a 470 Kv motor and a 580 Kv motor. 
    if (Globals.ComplimentaryPWM == false && Globals.BipolarSwitching == false) {
        switch (CurrentSector) {
            case 0: OVDCON = PWM1H_ON | PWM2L_FORCE; break;
            case 1: OVDCON = PWM1H_ON | PWM3L_FORCE; break;
            case 2: OVDCON = PWM2H_ON | PWM3L_FORCE; break;
            case 3: OVDCON = PWM2H_ON | PWM1L_FORCE; break;
            case 4: OVDCON = PWM3H_ON | PWM1L_FORCE; break;
            case 5: OVDCON = PWM3H_ON | PWM2L_FORCE; break;
        }
    }
    
    //Unipolar, complementary PWM switching method. Verified working for
    //20% - 80% duty cycles with a 470 Kv and a 580 Kv motor. 
    else if (Globals.ComplimentaryPWM == true && Globals.BipolarSwitching == false) {
        switch (CurrentSector) {
            case 0: OVDCON = PWM1H_ON | PWM1L_ON | PWM2L_FORCE; break;
            case 1: OVDCON = PWM1H_ON | PWM1L_ON | PWM3L_FORCE; break;
            case 2: OVDCON = PWM2H_ON | PWM2L_ON | PWM3L_FORCE; break;
            case 3: OVDCON = PWM2H_ON | PWM2L_ON | PWM1L_FORCE; break;
            case 4: OVDCON = PWM3H_ON | PWM3L_ON | PWM1L_FORCE; break;
            case 5: OVDCON = PWM3H_ON | PWM3L_ON | PWM2L_FORCE; break;
        }
    }

}

/*******************************************************************************
 * Subroutine:  AlignMotor()								
 * Description: Forces the motor into a known position. Used for dry starting
 *              the motor and also for fast-n-hard braking. Will effectively 
 *              lock the motor into position.
 * 
 *              Please note that locking the motor does require a bit of power.
 *              Setting the duty cycle after a brake can cut down on the power
 *              needed to lock the motor into place. If only braking is desired,
 *              then ReleaseMotor should be called after this in order to cut
 *              power to the BLDC. 
 
 * Prerequisites:   PWM subsystem must be initialized.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
 * 02-02-2016       Changed locking method to use a set PWM signal to prevent
 *                  too much current from being used.
*******************************************************************************/
void AlignMotor() {
    
    //Manually lock the motor into alignment.
    OVDCON = PWM1H_ON | PWM2H_ON | PWM3L_FORCE;
    
    //Set current sector to first known sector.
    CurrentSector = 0;
    
    //Allow time for motor to move into position. Will need to fiddle with this.
    __delay_ms(10);
}

/*******************************************************************************
 * Subroutine:  ReleaseMotor									
 * Description: De-energizes the motor windings, allowing the motor to spin
 *              freely. This is equivalent to putting a car in neutral. This
 *              is done by writing digital lows to all PWM outputs.
 
 * Prerequisites:   PWM subsystem must be initialized.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-02-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void ReleaseMotor() {
    
    //Kill all PWM channels.
    OVDCON = 0;
    
    //Wait for ten milliseconds.
    __delay_ms(10);
}

/*******************************************************************************
 * Subroutine:  StartupMotor									
 * Description: Will dry start the motor. This function will begin with an 
 *              open loop ramp-up procedure to get the motor moving fast enough
 *              for the back-EMF to be strong enough for the PIC's ADC to 
 *              sample something other than noise. The motor does not need to
 *              be moving as fast as some application notes claim, the motor
 *              just needs to be moving fast enough to act as a generator on
 *              the undriven lead. 
 
 * Prerequisites:   PWM subsystem must be initialized.
 *                  ADC subsystem must be initialized.
 *                  Globals.MotorStarted must be set to false.
 *                  ADC interrupt must be disabled to prevent a false start.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
 * 02-06-2016       Tuned for the 580 KV motor used on the CARE-Drone.
*******************************************************************************/
void StartupMotor() {
    
    unsigned int i = 10000;
    unsigned int Time = 14000;
    
    AlignMotor();
    
    while (!Globals.MotorIsStarted) {
        
        SectorChange();
        __delay_us(Time);
        
        SectorChange();
        __delay_us(Time);
        
        SectorChange();
        __delay_us(Time);
        
        SectorChange();
        __delay_us(Time);
        
        SectorChange();
        __delay_us(Time);
        
        SectorChange();
        T1CONbits.TON = 0; 
        TMR1 = 0; 
        T1CONbits.TON = 1;
        __delay_us(Time);
        
        i--;
        if (i % 2 == 0 && Time > 1000) {
            Time -= 500;
        }
        else if (Time <= 1000) {
            IEC0bits.ADIE = 1;
            return;
        }
    }
}

/*******************************************************************************
 * Subroutine:  StopMotor									
 * Description: Forces the motor to a sudden stop, effectively braking the
 *              motor. It does this by causing a forced alignment. ReleaseMotor
 *              should be called at a time period after calling this function
 *              in order to prevent the motor from using excess current.
 
 * Prerequisites:   PWM subsystem must be initialized.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-02-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void StopMotor() {
    
    //Kill the ADC and Zero-Cross detection.
    IEC0bits.ADIE = 0;
    
    //Set the flag.
    Globals.MotorIsStarted = false;
    
    //Brake.
    AlignMotor();
}

/*******************************************************************************
 * Subroutine:  Command_Start									
 * Description: An SPI command instructing the PIC to start the motor. Right
 *              now, this simply calls the StartupMotor function. The reason
 *              this is a separate function is so that it can be expanded to 
 *              more advanced handling of the command. For an actual commercial 
 *              purpose, it would be good practice for this function to return
 *              to the SPI master that the motor successfully or unsuccessfully
 *              started, along with other related data such as starting speed.
 
 * Prerequisites:   PWM subsystem must be initialized.
 *                  SPI subsystem must be initialized.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-06-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void Command_Start() { 
    
    if (Globals.MotorIsStarted == false) { StartupMotor(); }
}

/*******************************************************************************
 * Subroutine:  Command_Stop						
 * Description: An SPI command instructing the PIC to stop the motor. It will
 *              brake the motor and then de-energize the windings. It can be
 *              expanded for actual commercial use.
 
 * Prerequisites:   PWM subsystem must be initialized.
 *                  SPI subsystem must be initialized.
 * Input:           None.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-06-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void Command_Stop() { 
    
    if (Globals.MotorIsStarted == true) {
        StopMotor();				//Lock motor into position.
        __delay_ms(500);		//Wait 500 milliseconds.
        ReleaseMotor(); 		//Kill power to motors.
    }
}

/*******************************************************************************
 * Subroutine:  Command_SetThrottle									
 * Description: An SPI command instructing the PIC to set the throttle of the
 *              motor to a specified value. Right now, it directly modifies
 *              the PWM duty cycle registers. This will need to be changed 
 *              before the build design milestone.
 
 * Prerequisites:   PWM subsystem must be initialized.
 *                  Motor must be started and running (Globals.MotorIsStarted
 *                  should be set to "true").
 *                  SPI subsystem must be initialized.
 * Input:           Throttle: should be a number between 0 and 100 specifying
 *                  the duty cycle to set. Until other parts of the code is 
 *                  fixed, this will temporarily specify the value to write
 *                  directly to the duty cycle registers.
 * Output:          None.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-06-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void Command_SetThrottle(unsigned int Throttle) {
    PDC1 = Throttle;
    PDC2 = Throttle;
    PDC3 = Throttle;
}

/*******************************************************************************
 * Subroutine:  Command_GetThrottle							
 * Description: An SPI command instructing the PIC to send to the SPI master
 *              the current throttle of the motor. Under construction.
 
 * Prerequisites:   PWM subsystem must be initialized.
 *                  SPI subsystem must be initialized.
 * Input:           None.
 * Output:          The current throttle of the motor.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-06-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void Command_GetThrottle() {
    WriteSPI(Globals.CurrentThrottle);
}

/*******************************************************************************
 * Subroutine:  Command_GetSpeed					
 * Description: An SPI command instructing the PIC to send to the SPI master
 *              the current speed of the motor.
 
 * Prerequisites:   PWM subsystem must be initialized.
 *                  SPI subsystem must be initialized.
 * Input:           None.
 * Output:          The current speed of the motor.
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 02-25-2016       Original code for CARE-Drone ESC.
   02-26-2016		Modified to send a 16 bit value, name changed from
   					Command_GetSpeedLower to Command_GetSpeed.
*******************************************************************************/
void Command_GetSpeed(){
    WriteSPI(Globals.CurrentSpeed);
}

void Command_GetPhaseAdvance() {
	WriteSPI(Globals.PhaseAdvance);
}

void Command_SetPhaseAdvance(unsigned int PhaseAdvance) {
	if (Globals.MotorIsStarted == false) { 
		Globals.PhaseAdvance = PhaseAdvance;
	}
}

void Command_GetNumPoles() {
	WriteSPI(Globals.NumPoles);
}

void Command_SetNumPoles(unsigned int NumPoles) {
	Globals.NumPoles = NumPoles;
}

void Command_GetPWMSwitching(){
	WriteSPI(Globals.ComplimentaryPWM);
}

void Command_SetComplimentarySwitching() {
	if (Globals.MotorIsStarted ==false) {
		Globals.ComplimentaryPWM == true;
	}
}

void Command_SetIndependentSwitching() {
	if (Globals.MotorIsStarted ==false) {
		Globals.ComplimentaryPWM == false;
	}
}
void Command_IsMotorStarted() {
	WriteSPI(Globals.MotorIsStarted);	
}




//</editor-fold>


/*******************************************************************************
 * Subroutine:  Main							
 * Description: The point of entry for the PIC's firmware. Main will
 *              initialize the system and then monitor and manage the SPI bus.
 
 * Prerequisites:   None
 * Input:           None.
 * Output:          None
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
int main(int argc, char** argv) {
    
    TRISDbits.TRISD1 = OUTPUT;      //Set D1 as an output.
    TRISDbits.TRISD0 = OUTPUT;      //Set D0 as an output.
    
    if (RCONbits.TRAPR == 1) {      //Check if device has been reset.
        _EmergencyStop();           //Kill power to motor and light red LED.
        while(1);                   //Loop endlessly.
    }
    
    else {
    
        LATDbits.LATD1 = 1;         //Turn on green status LED.
        LATDbits.LATD0 = 0;         //Turn off red status. 
        
        __delay_ms(5000);           //Allow time for master to boot.
    
        //Turn off nested interrupts
        INTCON1bits.NSTDIS = 0;     //Turn off nested interrupts.
        T1CONbits.TON = 1;          //Turn on timer 1, but keep its interrupt off.
        IPC1bits.T2IP = 5;          //Set timer 2 to priority 5.
        IFS0bits.T2IF = 0;          //Clear timer 2 interrupt flag.
        IEC0bits.T2IE = 1;          //Enable timer 2 interrupt.

        InitializeGlobals();        //Initialize global variables.
        InitializeSPI();            //Initialize the SPI subsystem
        InitializePWM();            //Startup the PWM system at 20 KHz.
        ReleaseMotor();             //Release motor from PWM outputs.
        SetThrottle(55);            //Set initial duty cycle to 55%.
        InitializeADC();            //Start the ADC for back-EMF detection.
        
        #ifdef ENABLE_THROTTLE_TEST  
            StartupMotor();         //Dry start motor in open-loop mode.
        #endif

        while (1) {

        #ifdef ENABLE_THROTTLE_TEST   
            SetThrottle(55);
            __delay_ms(10000);

             SetThrottle(70);
            __delay_ms(10000);
        #endif


        #ifdef ENABLE_SPI
            //Manage SPI communications
            unsigned int Incoming = ReadSPI(0);

            //Handle incoming command/information
            if (VerifyMessage(Incoming)) {

                unsigned int Command = ((Incoming & 0x7F00) >> 8);
                unsigned int Data = (Incoming & 0x00FF);

                switch (Command) {

                    case SPI_COMMAND_START_MOTOR:   Command_Start(); 
                    								LATDbits.LATD0 = 1; break;
                    case SPI_COMMAND_STOP_MOTOR:    Command_Stop(); 
                    								LATDbits.LATD0 = 0; break;
                    case SPI_COMMAND_GET_KP: 		break;
                   	case SPI_COMMAND_SET_KP: 		break;
                   	case SPI_COMMAND_GET_KI: 		break;
                   	case SPI_COMMAND_SET_KI: 		break;
                   	case SPI_COMMAND_GET_KD: 		break;
                   	case SPI_COMMAND_SET_KD: 		break;	
                    case SPI_COMMAND_SET_THROTTLE:  Command_SetThrottle(Data); break;
                    case SPI_COMMAND_GET_THROTTLE:  Command_GetThrottle(); break;
                    case SPI_COMMAND_GET_SPEED:   	Command_GetSpeed(); break;
                    case SPI_COMMAND_GET_PHASE: 	Command_GetPhaseAdvance(); break;
                   	case SPI_COMMAND_SET_PHASE: 	Command_SetPhaseAdvance(Data); break;
                   	case SPI_COMMAND_GET_POLES: 	Command_GetNumPoles(); break;
                   	case SPI_COMMAND_SET_POLES: 	Command_SetNumPoles(Data); break;
                   	case SPI_COMMAND_GET_PWM: 		Command_GetPWMSwitching(); break;
                   	case SPI_COMMAND_SET_COMP: 		Command_SetComplimentarySwitching(); break;
                   	case SPI_COMMAND_SET_IND: 		Command_SetIndependentSwitching(); break;
                   	case SPI_COMMAND_MOTOR_STARTED: Command_IsMotorStarted(); break;
                   	case SPI_COMMAND_ENABLE_PID: 	break;
                   	case SPI_COMMAND_DISABLE_PID: 	break;
                }  
            }
        #endif
        }
    }
    return (0);
}

//<editor-fold defaultstate="collapsed" desc="Interrupt Service Routines">
void __attribute__((__interrupt__, no_auto_psv)) _INT0Interrupt(void) {
    IFS0bits.INT0IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void) {
    IFS0bits.IC1IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _OC1Interrupt(void) {
    IFS0bits.OC1IF = 0;
    _EmergencyStop();
}

/*******************************************************************************
 * Subroutine:  _T1Interrupt					
 * Description: An interrupt service routine that is called when timer 1 has
 *              a rollover event.        
 
 * Prerequisites:   Timer subsystem must be initialized.
 *                  Timer 1 interrupt must be initialized.
 * Priority:        0
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    
    IFS0bits.T1IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void) {
    IFS0bits.IC2IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _OC2Interrupt(void) {
    IFS0bits.OC2IF = 0;
    _EmergencyStop();
}

/*******************************************************************************
 * Subroutine:  _T2Interrupt					
 * Description: An interrupt service routine that is called when timer 2 has
 *              expired.
 * 
 *              Timer 2 is sometimes referred to as the commutation timer. When
 *              timer 2 expires, the motor is commutated. Because timer 2 is
 *              absolutely critical to the motor running, this interrupt should
 *              be the highest priority interrupt in the system.
 
 * Prerequisites:   Timer subsystem must be initialized.
 *                  Timer 2 interrupt must be initialized.
 *                  PWM must be initialized.
 * Priority:        5
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
*******************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    
    //Clear interrupt flag.
    IFS0bits.T2IF = 0;
    
    //Perform sector change.
    SectorChange();
    
    //Disarm timer 2
    T2CONbits.TON = 0; 
    TMR2 = 0; 
}

void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0;
    _EmergencyStop();
}

/*******************************************************************************
 * Subroutine:  _SPI1Interrupt					
 * Description: An interrupt service routine that is called when a SPI 
 *              transaction is completed.
 
 * Prerequisites:   SPI subsystem is initialized.
 * Priority:        0
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 12-05-2015          Original code for CARE-Drone ESC.
*******************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void)
{  
    IFS0bits.SPI1IF = 0;
    SPI1STATbits.SPIROV = 0;
} 

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0;
    _EmergencyStop();
}

/*******************************************************************************
 * Subroutine:  _ADCInterrupt						
 * Description: An interrupt service routine that is called when an ADC 
 *              conversion has completed. 
 * 
 *              The ADC ISR will pull ADC channels 1-3 in order to to examine
 *              the three back-EMF signals. The three signals are averaged in
 *              order to find the virtual motor neutral point. The three
 *              signals and the calculated neutral point are compared in order
 *              to find two things: 1) the current sector that the motor is
 *              currently in and 2) if a zero cross event has occurred.
 * 
 *              If a zero cross event has occurred, timer 1 is looked at in
 *              order to see ho much time it took for the motor to travel 60
 *              electrical degrees. Timer 2 is armed with half this amount,
 *              minus filter delay time and phase advance times. Timer 1 is 
 *              then reset.             
 
 				Timer 2 must have a lower priority than the ADC; failure
 				to properly setup the priorities will cause an address trap
 				to occur at the moment that the motor switches from open
 				loop to closed loop control.
 				
 				If a future Sr Design team ever decides to pick this project
 				up for a generation 2, then I would like to see this 
 				expanded to implement Microchip's high speed algorithm
 				according to Microchip application note AN1083.
 
 * Prerequisites:   ADC subsystem must be initialized.
 *                  Motor must be ramped up fast enough to generate back-EMF.
 *                  The physical circuit must provide a low-pass filter that
 *                  filters the PWM signals out of the back-EMF signals.
 * Priority:        6
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
 * 02-05-2016       Added logic to prevent misfires.
 * 02-06-2016       Got rid of the digital filters; ISR is now reliant on
 *                  physical low-pass filters.
 * 02-21-2016       Modified to remove arrays in order to eliminate possible
 *                  address errors. Interrupt priority changed from 5 to 6.
 * 02-25-2016       Added current speed calculations.
 * 02-26-2016		Verified working at 26,000 rpm.
*******************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _ADCInterrupt(void) {
    
    //Clear interrupt flag.
    IFS0bits.ADIF = 0;
    
    //Set initial Zero point flag.
    bool ZeroCrossDetected = false;
    
    //Detect current sector.
    if      (ADCBUF1 > ADCBUF3 && ADCBUF2 < ADCBUF3) { CurrentSector = 0; }
    else if (ADCBUF1 > ADCBUF2 && ADCBUF3 < ADCBUF2) { CurrentSector = 1; }
    else if (ADCBUF2 > ADCBUF1 && ADCBUF3 < ADCBUF1) { CurrentSector = 2; }
    else if (ADCBUF2 > ADCBUF3 && ADCBUF1 < ADCBUF3) { CurrentSector = 3; }
    else if (ADCBUF3 > ADCBUF2 && ADCBUF1 < ADCBUF2) { CurrentSector = 4; }
    else if (ADCBUF3 > ADCBUF1 && ADCBUF2 < ADCBUF1) { CurrentSector = 5; }

    //Estimate the virtual neutral point.
    Globals.ZeroCrossPoint = (ADCBUF1 + ADCBUF2 + ADCBUF3)/3;
    
    //Check for zero-cross event.
    if (CurrentSector == 0 && ADCBUF3 <= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 1 && ADCBUF2 >= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 2 && ADCBUF1 <= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 3 && ADCBUF3 >= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 4 && ADCBUF2 <= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 5 && ADCBUF1 >= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    
    //If zero-cross event has occurred and we have a valid Back-EMF signal
    if (ZeroCrossDetected == true &&    //If a zero cross event was detected.
        Globals.ZeroCrossPoint != 0 &&  //If the virtual neutral point is not zero (motor stalled).
        T2CONbits.TON == 0) {           //If we're not expecting a commutation.
        
        //Set the started flag so we can kick out of open-loop mode.
        Globals.MotorIsStarted = true;
        
        //Capture the time between commutations.
        Globals.CommutationTime = TMR1;
        
        //Calculate current speed in rpm.
        if (Globals.CommutationTime != 0) {
            Globals.CurrentSpeed = (GetSystemClock()*60)/(Globals.NumPoles*Globals.CommutationTime);
        }
        else {
            Globals.CurrentSpeed = 0;   //Avoid divide by zero condition.
        }
        
        //Arm commutation timer.
        PR2 = Globals.CommutationTime/2  - Globals.PhaseAdvance;
        T2CONbits.TON = 1;
        
        //Reset Timer 1
        T1CONbits.TON = 0; 
        TMR1 = 0; 
        T1CONbits.TON = 1;
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _NVMInterrupt(void) {
    IFS0bits.NVMIF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _SI2CInterrupt(void){
    IFS0bits.SI2CIF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _MI2CInterrupt(void) {
    IFS0bits.MI2CIF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    IFS0bits.CNIF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _IC7Interrupt(void) {
    IFS1bits.IC7IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _IC8Interrupt(void) {
    IFS1bits.IC8IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt(void) {
    IFS1bits.INT2IF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _PWMInterrupt(void) {
    IFS2bits.PWMIF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _QEIInterrupt(void) {
    IFS2bits.QEIIF = 0;
    _EmergencyStop();
}

void __attribute__((__interrupt__, no_auto_psv)) _FLTAInterrupt(void) {
    IFS2bits.FLTAIF = 0;
    _EmergencyStop();
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Trap Service Routines">
/* Primary Exception Vector handlers:
   These routines are used if INTCON2bits.ALTIVT = 0.
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application. */

void __attribute__((__interrupt__)) _OscillatorFail(void)
{
    INTCON1bits.OSCFAIL = 0;        //Clear the trap flag
    
    _EmergencyStop();
    while (1);
}

void __attribute__((__interrupt__)) _AddressError(void)
{
    INTCON1bits.ADDRERR = 0;        //Clear the trap flag
    
    _EmergencyStop();
    while (1);
}

void __attribute__((__interrupt__)) _StackError(void)
{
    INTCON1bits.STKERR = 0;         //Clear the trap flag
    
    _EmergencyStop();
    while (1);
    
}

void __attribute__((__interrupt__)) _MathError(void)
{
    INTCON1bits.MATHERR = 0;        //Clear the trap flag
    
    _EmergencyStop();
    while (1);
}

/* Alternate Exception Vector handlers:
   These routines are used if INTCON2bits.ALTIVT = 1.
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application. */

void __attribute__((__interrupt__)) _AltOscillatorFail(void)
{
    INTCON1bits.OSCFAIL = 0;
    
    _EmergencyStop();
    while (1);
}

void __attribute__((__interrupt__)) _AltAddressError(void)
{
    INTCON1bits.ADDRERR = 0;
    
    _EmergencyStop();
    while (1);
}

void __attribute__((__interrupt__)) _AltStackError(void)
{
    INTCON1bits.STKERR = 0;
    
    _EmergencyStop();
    while (1);
}

void __attribute__((__interrupt__)) _AltMathError(void)
{
    //Clear trap
    INTCON1bits.MATHERR = 0;

    _EmergencyStop();
    while (1);
}

//</editor-fold>
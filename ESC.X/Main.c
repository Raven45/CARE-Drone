/*******************************************************************************
 * Copyright 2016 Department of Engineering, Harding University
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
  
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 
 * Created on January 15, 2016, 9:14 PM
*******************************************************************************/

/*******************************************************************************
 * "but those who hope in the Lord will renew their strength. They will soar on 
 * wings like eagles; they will run and not grow weary, they will walk and
 * not be faint." -Isaiah 40:31
*******************************************************************************/

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1<<(pos)))
#define SET_BIT(var,pos)   ((var) |= (1<<(pos)))

#define OUTPUT  0
#define INPUT   1


#include "p30fxxxx.h"
#include <xc.h>
#define FOSC 64000000UL
#define FCY 16000000UL
#include <libpic30.h>
#include <p30F2010.h>
#include <spi.h>

//Set configuration to use external oscillator with x8 PLL for 16 MHz clock.
_FOSC(PRI & XT_PLL8)
_FWDT(WDT_OFF)

//Macros
//#define ENABLE_SPI        
#define ENABLE_THROTTLE_TEST

#define bool short int        
#define false 0x00
#define true 0xFF
        
#define SPI_COMMAND_BITMASK     0x7F00
#define SPI_DATA_BITMASK        0x00FF

#define PWM1L_FORCE             0x0001
#define PWM1H_FORCE             0x0002     
#define PWM2L_FORCE             0x0004
#define PWM2H_FORCE             0x0008
#define PWM3L_FORCE             0x0010
#define PWM3H_FORCE             0x0020
        
#define PWM1L_ON                0x0100
#define PWM1H_ON                0x0200
#define PWM2L_ON                0x0400
#define PWM2H_ON                0x0800        
#define PWM3L_ON                0x1000
#define PWM3H_ON                0x2000   
        
//PWM frequency in hertz
#define PWM_FREQ 20000
        
unsigned int PWM_Resolution = 0;
unsigned int PDC_MAX = 0;
int CurrentSector = 5;

struct Global {
    bool MotorIsStarted;
    bool Direction; //true for clockwise, false for counter-clockwise.
    bool ComplimentaryPWM; 
    bool BipolarSwitching;
    unsigned short int NumPoles;
    unsigned int CurrentSpeed;
    unsigned short int FilterDelay;
    unsigned short int ADCProc;
    unsigned short int PhaseAdvance;
    unsigned int ZeroCrossPoint;
    unsigned int CurrentThrottle;
} Globals;

void InitializeGlobals();
void InitializePWM();
void InitializeADC();
void InitializeSPI();
unsigned int VerifyMessage(unsigned int message);
unsigned int EncodeParity(unsigned int message);
unsigned int ReadSPI(unsigned int data);
void CloseSPI();
void SetThrottle(unsigned int Throttle);
unsigned int GetThrottle();
void SectorChange();
void AlignMotor();
void StartupMotor();
void StopMotor();

void Command_Start(); 
void Command_Stop();
void Command_SetThrottle(unsigned int Throttle);
void Command_GetThrottle();

void InitializeGlobals () {
    
    Globals.MotorIsStarted = false;
    Globals.Direction = false; //Clockwise rotation.
    
    //Modes of switching operation.
    Globals.ComplimentaryPWM = false; 
    Globals.BipolarSwitching = false;
    
    //Set up for our 580 KV motor
    Globals.NumPoles = 22;
    
    Globals.CurrentSpeed = 0;
    Globals.FilterDelay = 1739; //calculated value for 108 ms filter delay at 16MHz clock.
    Globals.ADCProc = 0; 
    Globals.PhaseAdvance = 0;
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
    DTCON1 = 0x6F;               //Set dead time.
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
    
    IPC2bits.ADIP = 5;          //Set interrupt priority.
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
    ConfigIntSPI1(SPI_INT_EN &  SPI_INT_PRI_6);
    
    SPI1CON |= 1 << 7;
    
    CNEN1 = 0;   
    CNEN2 = 0;
    INTCON1 = 0;
    INTCON2 = 0;
  
    TRISBbits.TRISB2 = INPUT;
    
    SPICONValue  =  FRAME_ENABLE_OFF &      //Disable SPI framed mode
                    FRAME_SYNC_OUTPUT &     //Enable synchronized output
                    DISABLE_SDO_PIN  &      //Enable 4-wire SPI
                    SPI_MODE16_ON &         //Use 16 bit words.
                    SPI_SMP_OFF &           //Set edge detect mode
                    SPI_CKE_OFF &           //Set clock polarity
                    SLAVE_ENABLE_ON &       //Enable slave mode.
                    CLK_POL_ACTIVE_HIGH &   
                    MASTER_ENABLE_OFF;      //Disable master mode.
    
    SPISTATValue =  SPI_ENABLE &            //Turn the SPI port on
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
*******************************************************************************/ 
unsigned int VerifyMessage(unsigned int message) {
    
    int NumOfOnes = 0;
    
    //Count the number of ones in the message.
    int i = 0;
    while (i < 16) {
        if (CHECK_BIT(message, i)) {
            NumOfOnes++;
        }
        i++;
    }
    
    //If no parity errors were detected.
    //Use even parity.
    if (NumOfOnes % 2 == 0) {
        return 1;
    }
    
    //A single bit parity error was detected
    else {
        return 0;
    }
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
*******************************************************************************/ 
unsigned int ReadSPI(unsigned int data) {
    
    unsigned int Incoming = 0;
    
    //Encode an odd parity bit if needed.
    EncodeParity(data);
    
    //Wait until we are summoned by the master.
    while (PORTBbits.RB2 != 0){}
    
    //Capture data from master.
    while (!DataRdySPI1()){}
    Incoming = ReadSPI1();
    
    //Send response to master.
    WriteSPI1(data);
    while (SPI1STATbits.SPITBF) {}
    
    return Incoming;
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
*******************************************************************************/
void SetThrottle(unsigned int Throttle) {
    
    if (Throttle >=0 || Throttle < 100) {
        PDC1 = (Throttle*(PTPER+1))/40;
        PDC2 = PDC1;
        PDC3 = PDC1;
        Globals.CurrentThrottle = PDC1;
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
        if (CurrentSector == 0) { 
            OVDCON = PWM1H_ON | PWM2L_FORCE;
        }
        else if (CurrentSector == 1) {
            OVDCON = PWM1H_ON | PWM3L_FORCE;
        }
        else if (CurrentSector == 2) {
            OVDCON = PWM2H_ON | PWM3L_FORCE;
        }
        else if (CurrentSector == 3) {
            OVDCON = PWM2H_ON | PWM1L_FORCE;
        }
        else if (CurrentSector == 4) {
            OVDCON = PWM3H_ON | PWM1L_FORCE;
        }
        else if (CurrentSector == 5) {
            OVDCON = PWM3H_ON | PWM2L_FORCE;
        }
    }
    
    //Unipolar, complementary PWM switching method.
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
            //return;
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
    Globals.MotorIsStarted = 0;
    
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
    
    StartupMotor(); 
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
    
    StopMotor(); 
    __delay_ms(500);
    ReleaseMotor(); 
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
    ReadSPI(PDC1);
}


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
    
    INTCON1bits.NSTDIS = 0;
    T1CONbits.TON = 1;
    IPC1bits.T2IP = 7;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    
    InitializeGlobals();        //Initialize global variables.
    InitializePWM();            //Startup the PWM system at 20 KHz
    SetThrottle(60);            //Set initial duty cycle to 45%
    Globals.ComplimentaryPWM = true;
    InitializeADC();            //Start the ADC for back-EMF detection.
    StartupMotor();             //Dry start motor in open-loop mode.
    
    while (1) {
       
    #ifdef ENABLE_THROTTLE_TEST   /*
        SetThrottle(20);
        __delay_ms(10000);
        
        SetThrottle(40);
        __delay_ms(10000);*/
        
        SetThrottle(60);
        __delay_ms(10000);
        
        PDC1 = 0x8FF;
        PDC2 = 0x8FF;
        PDC3 = 0x8FF;
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
                
                case 1: Command_Start(); break;
                case 2: Command_Stop(); break;
                case 3: Command_SetThrottle(Data); break;
                case 4: Command_GetThrottle(); break;
            }  
        }
    #endif
    }

    return (0);
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
 
 * Prerequisites:   ADC subsystem must be initialized.
 *                  Motor must be ramped up fast enough to generate back-EMF.
 *                  The physical circuit must provide a low-pass filter that
 *                  filters the PWM signals out of the back-EMF signals.
 * Priority:        5
 * Platform:        Microchip dsPIC30F2010.
 * Author:          Aaron Burns.

 * Change-log:
 * 01-2016          Original code for CARE-Drone ESC.
 * 02-05-2016       Added logic to prevent misfires.
 * 02-06-2016       Got rid of the digital filters; ISR is now reliant on
 *                  physical low-pass filters.
*******************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _ADCInterrupt(void) {
    
    //Clear interrupt flag.
    IFS0bits.ADIF = 0;
    
    //Set initial Zero point flag.
    bool ZeroCrossDetected = false;
    
    //Grab the ADC values for the measured back-EMF.
    unsigned int BackEMF[3] = { ADCBUF1, ADCBUF2, ADCBUF3 };
    
    //Detect current sector.
    if      (BackEMF[0] > BackEMF[2] && BackEMF[1] < BackEMF[2]) { CurrentSector = 0; }
    else if (BackEMF[0] > BackEMF[1] && BackEMF[2] < BackEMF[1]) { CurrentSector = 1; }
    else if (BackEMF[1] > BackEMF[0] && BackEMF[2] < BackEMF[0]) { CurrentSector = 2; }
    else if (BackEMF[1] > BackEMF[2] && BackEMF[0] < BackEMF[2]) { CurrentSector = 3; }
    else if (BackEMF[2] > BackEMF[1] && BackEMF[0] < BackEMF[1]) { CurrentSector = 4; }
    else if (BackEMF[2] > BackEMF[0] && BackEMF[1] < BackEMF[0]) { CurrentSector = 5; }

    //Estimate the virtual neutral point.
    Globals.ZeroCrossPoint = (BackEMF[0] + BackEMF[1] + BackEMF[2])/3;
    
    //Check for zero-cross event.
    if (CurrentSector == 0 && BackEMF[2] <= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 1 && BackEMF[1] >= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 2 && BackEMF[0] <= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 3 && BackEMF[2] >= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 4 && BackEMF[1] <= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    else if (CurrentSector == 5 && BackEMF[0] >= Globals.ZeroCrossPoint) {
        ZeroCrossDetected = true;
    }
    
    //If zero-cross event has occurred and we have a valid Back-EMF signal
    if (ZeroCrossDetected == true &&    //If a zero cross event was detected.
        Globals.ZeroCrossPoint != 0 &&  //If the virtual neutral point is not zero (motor stalled).
        T2CONbits.TON == 0) {           //If we're not expecting a commutation.
        
        //Set the started flag so we can kick out of open-loop mode.
        Globals.MotorIsStarted = true;
        
        //Arm commutation timer.
        PR2 = TMR1/2 - 0x3F/*- Globals.FilterDelay - Globals.ADCProc - Globals.PhaseAdvance*/;
        T2CONbits.TON = 1;
        
        //Reset Timer 1
        T1CONbits.TON = 0; 
        TMR1 = 0; 
        T1CONbits.TON = 1;
    }
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
 * Priority:        7
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
/* 
 * File:   Main.c
 * Author: Aaron
 *
 * Created on December 4, 2015, 3:59 PM
 */

#include <p30f2010.h>
#define FCY 7372800UL
#include <libpic30.h>


//Macros
#define OUTPUT  0
#define INPUT   1
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1<<(pos)))
#define SET_BIT(var,pos)   ((var) |= (1<<(pos)))

unsigned int PWM_Channel_1 = 0;
unsigned int PWM_Channel_2 = 0;
unsigned int PWM_Channel_3 = 0;

int PWM_Resolution = 0;
int PWM_Frequency = 0;
int PWM_Period = 0;
int PWM_Prescale = 1;
int Clock_Speed = 0;
int Clock_Period = 0;
unsigned int CurrentThrottle = 0;
int CurrentSector = 0;

void InitializePWM();
void TogglePWMChannel(unsigned int Channel);
void SetThrottle(unsigned int Throttle);
unsigned int GetThrottle();

void InitializePWM() {
    
    
    //PWM_Period = (Clock_Speed/(PWM_Frequency * PWM_Prescale)) + 1;
    //PWM_Resolution = log((2*(1/PWM_Frequency))/(1/Clock_Speed))/log(2);
    
    TRISE = OUTPUT;
    
    PTCON   = 0x0010;
    PWMCON1 = 0x00FF;
    PTPER = 0x5F;
    SetThrottle(0);
    PTMR = 0;
    DTCON1 = 0x1;
    _PTEN = 1;
    
    TogglePWMChannel(1);
    TogglePWMChannel(2);
    TogglePWMChannel(3);
}

void TogglePWMChannel(unsigned int Channel) {
    
    if (Channel == 1) {
        if (PWM_Channel_1 == 0) {
            PWM_Channel_1 = 1;
        }
        else {
            PWM_Channel_1 = 0;
        }
    }
    
    else if (Channel == 2) {
        if (PWM_Channel_2 == 0) {
            PWM_Channel_2 = 1;
        }
        else {
            PWM_Channel_2 = 0;
        }
    }
    else if (Channel == 3) {
        if (PWM_Channel_3 == 0) {
            PWM_Channel_3 = 1;
        }
        else {
            PWM_Channel_3 = 0;
        }
    }
}

void SetThrottle(unsigned int Throttle) {
    
    if (Throttle >=0 || Throttle < 100) {
        
        CurrentThrottle = Throttle;
        PDC1 = (Throttle * (2*PTPER))/100;
        PDC2 = (Throttle * (2*PTPER))/100;
        PDC3 = (Throttle * (2*PTPER))/100;
    }
}

unsigned int GetThrottle() {
    
    return (PDC1*100)/0xFF;
}

void SectorChange() {
    
    if (CurrentSector == 5) {
        CurrentSector = 0;
    }
    else {
        CurrentSector++;
    }
    
    if (CurrentSector == 0) { 
        OVDCON = 0x24;
    }
    else if (CurrentSector == 1) {
        OVDCON = 0x21;
    }
    else if (CurrentSector == 2) {
        OVDCON = 0x09;
    }
    else if (CurrentSector == 3) {
        OVDCON = 0x18;
    }
    else if (CurrentSector == 4) {
        OVDCON = 0x12;
    }
    else if (CurrentSector == 5) {
        OVDCON = 0x06;
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    int Throttle = 30;

    InitializePWM();
    SetThrottle(Throttle);
    
    while (1) {
        
        SectorChange();
        __delay_ms(10);
    }
    
    return (0);
}
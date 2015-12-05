/* 
 * File:   Timer.h
 * Author: Aaron
 *
 * Created on December 4, 2015, 2:57 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <p30f2010.h>

int TimerEnabled;
int TimeElapsed;


void InitializeTimer() {
    
    INTCON1bits.NSTDIS = 1;
    
    IPC0bits.T1IP   = 5;        //Set timer1 priority level to 5
    IFS0bits.T1IF   = 0;        //Clear timer 1 interrupt
    IEC0bits.T1IE   = 1;        //Enable timer 1 interrupt
    T1CONbits.TCKPS = 0;        //Set pre-scaler to 8
    PR1             = 0xE666;   //Set period register for 1 ms increments.
    T1CONbits.TON   = 1;        //Enable the timer
}

void ToggleTimer() {
    
    TimerEnabled = TimerEnabled ? 0 : 1;
}

void Delay(unsigned int Period){
    
    TimeElapsed = 0;
    while (TimeElapsed < Period) {}
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    
    IFS0bits.T1IF = 0;
    TimeElapsed++;
}


#endif	/* TIMER_H */


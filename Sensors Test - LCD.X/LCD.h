/* 
 * File:   LCD.h
 * Author: Aaron
 *
 * Created on December 4, 2015, 2:50 PM
 * 
 * 
 * 
 * Pinout:
 * B3: RS
 * B1: R/W
 * B0: E
 * E0-E5: D0-D5
 * B4: DB6
 * B5: DB7
 */

#ifndef LCD_H
#define	LCD_H

#include <p30f2010.h>
#include "Timer.h"

//Constants
#define LCD_INITIALIZE 0x30

#define OUTPUT  0
#define INPUT   1

//Macros
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1<<(pos)))
#define SET_BIT(var,pos)   ((var) |= (1<<(pos)))

struct LCD {
    
    unsigned int Rows;
    unsigned int Columns;
};

void SendCommand(char Command) {
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB3 = 0;
    
    if (CHECK_BIT(Command, 0))  { LATEbits.LATE0 = 1; }
    else                        { LATEbits.LATE0 = 0; }
    if (CHECK_BIT(Command, 1))  { LATEbits.LATE1 = 1; }
    else                        { LATEbits.LATE1 = 0; }
    if (CHECK_BIT(Command, 2))  { LATEbits.LATE2 = 1; }
    else                        { LATEbits.LATE2 = 0; }
    if (CHECK_BIT(Command, 3))  { LATEbits.LATE3 = 1; }
    else                        { LATEbits.LATE3 = 0; }
    if (CHECK_BIT(Command, 4))  { LATEbits.LATE4 = 1; }
    else                        { LATEbits.LATE4 = 0; }
    if (CHECK_BIT(Command, 5))  { LATEbits.LATE5 = 1; }
    else                        { LATEbits.LATE5 = 0; }
    if (CHECK_BIT(Command, 6))  { LATBbits.LATB4 = 1; }
    else                        { LATBbits.LATB4 = 0; }
    if (CHECK_BIT(Command, 7))  { LATBbits.LATB5 = 1; }
    else                        { LATBbits.LATB5 = 0; }
    
    //Set falling clock edge
    LATBbits.LATB0 = 0;
    Delay(1);
    LATBbits.LATB0 = 1;
}

void WriteCharacter(char Data) {
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB3 = 1;
    
    if (CHECK_BIT(Data, 0))     { LATEbits.LATE0 = 1; }
    else                        { LATEbits.LATE0 = 0; }
    if (CHECK_BIT(Data, 1))     { LATEbits.LATE1 = 1; }
    else                        { LATEbits.LATE1 = 0; }
    if (CHECK_BIT(Data, 2))     { LATEbits.LATE2 = 1; }
    else                        { LATEbits.LATE2 = 0; }
    if (CHECK_BIT(Data, 3))     { LATEbits.LATE3 = 1; }
    else                        { LATEbits.LATE3 = 0; }
    if (CHECK_BIT(Data, 4))     { LATEbits.LATE4 = 1; }
    else                        { LATEbits.LATE4 = 0; }
    if (CHECK_BIT(Data, 5))     { LATEbits.LATE5 = 1; }
    else                        { LATEbits.LATE5 = 0; }
    if (CHECK_BIT(Data, 6))     { LATBbits.LATB4 = 1; }
    else                        { LATBbits.LATB4 = 0; }
    if (CHECK_BIT(Data, 7))     { LATBbits.LATB5 = 1; }
    else                        { LATBbits.LATB5 = 0; }
    
    //Set falling clock edge
    LATBbits.LATB0 = 0;
    Delay(1);
    LATBbits.LATB0 = 1;
}


void InitializeLCD() {
    
    ADCON1 = 0x0000;
    ADCON2 = 0x0000;
    ADCON3 = 0x0000;
    ADPCFG = 0xFFFF;
    
    TRISBbits.TRISB0 = OUTPUT;  //E pin
    TRISBbits.TRISB1 = OUTPUT;  //R/W pin
    TRISBbits.TRISB3 = OUTPUT;  //RS pin
    TRISBbits.TRISB4 = OUTPUT;  //DB6 pin
    TRISBbits.TRISB5 = OUTPUT;  //DB7 pin
    
    TRISEbits.TRISE0 = OUTPUT;  //DB0 pin
    TRISEbits.TRISE1 = OUTPUT;  //DB1 pin
    TRISEbits.TRISE2 = OUTPUT;  //DB2 pin
    TRISEbits.TRISE3 = OUTPUT;  //DB3 pin
    TRISEbits.TRISE4 = OUTPUT;  //DB4 pin
    TRISEbits.TRISE5 = OUTPUT;  //DB5 pin
    
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 0;
    LATBbits.LATB3 = 0;
    
    LATEbits.LATE0 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE4 = 0;
    LATEbits.LATE5 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
    
    Delay(40);    
    SendCommand(LCD_INITIALIZE);
    Delay(1);
    SendCommand(LCD_INITIALIZE);
    Delay(4);
    SendCommand(LCD_INITIALIZE);
    Delay(1);
    
    SendCommand(0x38);
    Delay(1);
    SendCommand(0x06);
    Delay(1);
    SendCommand(0x0C);
    Delay(1);
    //SendCommand(0x01);
    //Delay(5);
    
    WriteCharacter(0xFF);
    Delay(2);
    WriteCharacter(0x42);
    Delay(2);
    WriteCharacter(0x44);
    Delay(2);
    //WriteCharacter(0x45);
    //Delay(2);
}

#endif	/* LCD_H */


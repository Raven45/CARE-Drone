/* 
 * File:   Main.c
 * Author: Aaron
 *
 * Created on December 4, 2015, 3:59 PM
 */

#include "LCD.h"
#include "dsp30SPI.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    int Incoming = 0;
    int DoOnce = 0;
    char ascii_character = 0;

    //InitializeTimer();
    InitializeLCD();
    InitializeSPI();
    
    while (1) {
        
        //Get message from master
        Incoming = ReadSPI(0);
        //Incoming = 0x0F0F;
        //If message has no single bit errors.
        //if (Incoming != 0) {
      
            char Pressure1 = 0;
            char Pressure2 = 0;
            char Pressure3 = 0;
            char Pressure4 = 0;
                
            if (CHECK_BIT(Incoming, 0)) { SET_BIT(Pressure1,0); }
            else                        { CLEAR_BIT(Pressure1,0); }
            if (CHECK_BIT(Incoming, 1)) { SET_BIT(Pressure1,1); }
            else                        { CLEAR_BIT(Pressure1,1); }
            if (CHECK_BIT(Incoming, 2)) { SET_BIT(Pressure1,2); }
            else                        { CLEAR_BIT(Pressure1,2); }
            if (CHECK_BIT(Incoming, 3)) { SET_BIT(Pressure1,3); }
            else                        { CLEAR_BIT(Pressure1,3); }
            if (CHECK_BIT(Incoming, 4)) { SET_BIT(Pressure2,0); }
            else                        { CLEAR_BIT(Pressure2,0); }
            if (CHECK_BIT(Incoming, 5)) { SET_BIT(Pressure2,1); }
            else                        { CLEAR_BIT(Pressure2,1); }
            if (CHECK_BIT(Incoming, 6)) { SET_BIT(Pressure2,2); }
            else                        { CLEAR_BIT(Pressure2,2); }
            if (CHECK_BIT(Incoming, 7)) { SET_BIT(Pressure2,3); }
            else                        { CLEAR_BIT(Pressure2,3); }
            if (CHECK_BIT(Incoming, 8)) { SET_BIT(Pressure3,0); }
            else                        { CLEAR_BIT(Pressure3,0); }
            if (CHECK_BIT(Incoming, 9)) { SET_BIT(Pressure3,1); }
            else                        { CLEAR_BIT(Pressure3,1); }
            if (CHECK_BIT(Incoming,10)) { SET_BIT(Pressure3,2); }
            else                        { CLEAR_BIT(Pressure3,2); }
            if (CHECK_BIT(Incoming,11)) { SET_BIT(Pressure3,3); }
            else                        { CLEAR_BIT(Pressure3,3); }
            if (CHECK_BIT(Incoming,12)) { SET_BIT(Pressure4,0); }
            else                        { CLEAR_BIT(Pressure4,0); }
            if (CHECK_BIT(Incoming,13)) { SET_BIT(Pressure4,1); }
            else                        { CLEAR_BIT(Pressure4,1); }
            if (CHECK_BIT(Incoming,14)) { SET_BIT(Pressure4,2); }
            else                        { CLEAR_BIT(Pressure4,2); }
            if (CHECK_BIT(Incoming,15)) { SET_BIT(Pressure4,3); }
            else                        { CLEAR_BIT(Pressure4,3); }
                
            if (Pressure1 < 10) {
                Pressure1 += 0x30;
            }
            else {
                Pressure1 += 0x30;
                Pressure1 += 0x7;
            }
                
            if (Pressure2 < 10) {
                Pressure2 += 0x30;
            }
            else {
                Pressure2 += 0x30;
                Pressure2 += 0x7;
            }
            
            if (Pressure3 < 10) {
                Pressure3 += 0x30;
            }
            else {
                Pressure3 += 0x30;
                Pressure3 += 0x7;
            }
                
            if (Pressure4 < 10) {
                Pressure4 += 0x30;
            }
            else {
                Pressure4 += 0x30;
                Pressure4 += 0x7;
            }
                
            WriteCharacter(ASCII_P);
            WriteCharacter(ASCII_COLIN);
            WriteCharacter(Pressure4);
            WriteCharacter(Pressure3);
            WriteCharacter(Pressure2);
            WriteCharacter(Pressure1);
            WriteCharacter(0x20);
            WriteCharacter(ASCII_P);
            WriteCharacter(ASCII_A);
                
            SendCommand(0x02);
            __delay_ms(1);
            
        //}
    }
    
    return (0);
}



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
    char ascii_character = 0;

    //InitializeTimer();
    InitializeLCD();
    InitializeSPI();
    
    while (1) {
        
        //Get message from master
        Incoming = ReadSPI(0);
        //Incoming = 0x8441;
        
        //If message has no single bit errors.
        if (VerifyMessage(Incoming) && Incoming != 0) {
            
            //Check for write ascii character command
            if (      CHECK_BIT(Incoming, 10) && 
                    !(CHECK_BIT(Incoming, 9)) && 
                    !(CHECK_BIT(Incoming, 8))) {  
                
                //Encode data character for LCD screen from message
                if (CHECK_BIT(Incoming, 0)) { SET_BIT(ascii_character, 0); } 
                else                        { CLEAR_BIT(ascii_character, 0); } 
                if (CHECK_BIT(Incoming, 1)) { SET_BIT(ascii_character, 1); } 
                else                        { CLEAR_BIT(ascii_character, 1); } 
                if (CHECK_BIT(Incoming, 2)) { SET_BIT(ascii_character, 2); } 
                else                        { CLEAR_BIT(ascii_character, 2); } 
                if (CHECK_BIT(Incoming, 3)) { SET_BIT(ascii_character, 3); } 
                else                        { CLEAR_BIT(ascii_character, 3); } 
                if (CHECK_BIT(Incoming, 4)) { SET_BIT(ascii_character, 4); } 
                else                        { CLEAR_BIT(ascii_character, 4); } 
                if (CHECK_BIT(Incoming, 5)) { SET_BIT(ascii_character, 5); } 
                else                        { CLEAR_BIT(ascii_character, 5); } 
                if (CHECK_BIT(Incoming, 6)) { SET_BIT(ascii_character, 6); } 
                else                        { CLEAR_BIT(ascii_character, 6); } 
                if (CHECK_BIT(Incoming, 7)) { SET_BIT(ascii_character, 7); } 
                else                        { CLEAR_BIT(ascii_character, 7); } 
                
                //Send char to LCD screen.
                WriteCharacter(ascii_character);
            }
            
            //Check for clear LCD screen command
            else if ( CHECK_BIT(Incoming, 10) && 
                    !(CHECK_BIT(Incoming, 9)) && 
                     (CHECK_BIT(Incoming, 8))) { 
            
                ClearLCD();
            }
            
        }
    }
    
    return (0);
}



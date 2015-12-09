



#include "Device.h"

#pragma config ICESEL  = ICS_PGx3

unsigned int EncodeAsciiCharacter(char character) {
    
    unsigned int Message = character;
    SET_BIT(Message, 10);
    return Message;
}

int main() {
    
    Device LCDScreen;
    LCDScreen.SetParity(ParityTypes::NoParity);
    int Incoming = 0; 
    
    while(1) {
    
        LCDScreen.SendAndReceive(0xF0F0); 
    }
}


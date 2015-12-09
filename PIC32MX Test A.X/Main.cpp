



//#include "Device.h"
#include "BMP280.h"

#pragma config ICESEL  = ICS_PGx3

int main() {
    
    Device LCDScreen;
    BMP280 PressureSensor;
    LCDScreen.SetParity(ParityTypes::NoParity);
    int Pressure = 0; 
    
    while(1) {
        Pressure = PressureSensor.RetrievePressure();
        LCDScreen.SendAndReceive(Pressure); 
    }
}






//#include "Device.h"
#include "BMP280.h"

#pragma config ICESEL  = ICS_PGx3

int main() {
    
    Device LCDScreen;
    BMP280 PressureSensor;
    LCDScreen.SetParity(ParityTypes::NoParity);
    LCDScreen.SetSlave(1);
    //PressureSensor.SetParity(ParityTypes::NoParity);
    //PressureSensor.SetSlave(2);
    int Pressure = 0; 
    
    LOOP_INFINITELY {
        Pressure = PressureSensor.RetrievePressure();
        LCDScreen.SendAndReceive(Pressure); 
    }
}


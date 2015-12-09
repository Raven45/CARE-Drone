#include "Device.h"



Device::Device() {
    
    //Specify the parity type to be used, if any.
    Parity = ParityTypes::EvenParity;
    
    //Configure the SPI port.
    unsigned int config=SPI_OPEN_MODE16 |       //Use 16 bit words.
                        SPI_OPEN_CKP_HIGH |     //Clock polarity high.
                        SPI_OPEN_CKE_REV |      //Clock edge detect.
                        SPI_OPEN_ON |           //We should turn it on.
                        SPI_OPEN_MSTEN;         //Enable master mode.

    //Turn on the SPI port
    SpiChnOpen(SPI_CHANNEL1, (SpiOpenFlags)config, 8);
    
    //Set pin B0 as the slave select pin.
    PORTSetPinsDigitalOut(IOPORT_B, BIT_0);
    PORTSetPinsDigitalOut(IOPORT_B, BIT_2);
    PORTClearBits(IOPORT_B, BIT_0);
    PORTClearBits(IOPORT_B, BIT_2);
    PORTToggleBits(IOPORT_B, BIT_0);
    PORTToggleBits(IOPORT_B, BIT_2);
    
    //Configure pin B1 as an output for SPI.
    PPSOutput(2, RPB1, SDO1);
    
    CurrentSlave = 1;
}

void Device::SetSlave(unsigned int Slave) {
    CurrentSlave = Slave;
}

Device::~Device() {
    SpiChnClose(SPI_CHANNEL1);
}

unsigned int Device::SendAndReceive(unsigned int DataToSend) {
    
    //Add a parity bit if even parity is enabled.
    if (Parity == ParityTypes::EvenParity) {
        
        int NumOfOnes = 0;

        for (int i = 0; i < 15; i++) {
            if (CHECK_BIT(DataToSend, i)) {
                NumOfOnes++;
            }
        }
        
        if (NumOfOnes % 2 != 0) {
            DataToSend ^= 1 << 15; //Set Parity
        }
    }
    
    //Add a parity bit if odd parity is enabled.
    else if (Parity == ParityTypes::OddParity) {
        
        int NumOfOnes = 0;
        
        for (int i = 0; i < 15; i++) {
            if (CHECK_BIT(DataToSend, i)) {
                NumOfOnes++;
            }
        }
        
        if (NumOfOnes % 2 == 0) {
            DataToSend ^= 1 << 7; //Set Parity
        }
    }
    
    unsigned int DataToReceive = 0;
    
    //Enable the slave select line.
    if      (CurrentSlave == 1) { PORTToggleBits(IOPORT_B, BIT_0); }
    else if (CurrentSlave == 2) { PORTToggleBits(IOPORT_B, BIT_2); }
    
    //Send the data to the slave.
    SpiChnPutC(SPI_CHANNEL1, DataToSend);

    //Capture response from slave.
    DataToReceive = SpiChnGetC(SPI_CHANNEL1);
    
    //Issue shut up command.
    if      (CurrentSlave == 1) { PORTToggleBits(IOPORT_B, BIT_0); }
    else if (CurrentSlave == 2) { PORTToggleBits(IOPORT_B, BIT_2); }
    
    //Return the data that we received from the slave.
    return DataToReceive;
}

bool Device::InitializeBus() {
    return true;
}

void Device::SetParity(unsigned short int Parity) {
    
    if (Parity >= 0 || Parity < 3) {
        this->Parity = Parity;
    }
}
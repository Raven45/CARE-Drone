/* 
 * File:   SPI.h
 * Author: Aaron
 *
 * Created on January 19, 2016, 11:00 AM
 */

#ifndef SPI_H
#define	SPI_H

#include <spi.h>

//Function prototypes
unsigned int VerifyMessage(unsigned int message);
unsigned int EncodeParity(unsigned int message);
unsigned int ReadSPI(unsigned int data);
void InitializeSPI();
void CloseSPI();

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
    
    //If no parity errors were detected.
    //Use even parity.
    if (NumOfOnes % 2 != 0) {
        SET_BIT(message, 15);
    }
    else {
        CLEAR_BIT(message, 15);
    }
    
    return message;
}

unsigned int ReadSPI(unsigned int data) {
    
    unsigned int Incoming = 0;
    
    //Encode an even parity bit if needed.
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


void __attribute__((__interrupt__)) _SPI1Interrupt(void)
{  
    IFS0bits.SPI1IF = 0;
    SPI1STATbits.SPIROV = 0;
} 

#endif	/* SPI_H */


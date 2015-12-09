/* 
 * File:   Device.h
 * Author: aburns2
 *
 * Created on November 14, 2015, 1:33 PM
 */

#ifndef DEVICE_H
#define	DEVICE_H

#define INPUT   1
#define OUTPUT  0

#ifndef NULL
#define NULL 0
#endif

#include "Object.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1<<(pos)))
#define SET_BIT(var,pos)   ((var) |= (1<<(pos)))

enum ParityTypes {
        NoParity,
        OddParity,
        EvenParity
};

class Device: Object {
    
    enum DeviceStates {
        Ready,
        Busy
    };
    
    public:
        Device();
        ~Device();
        
        unsigned int SendAndReceive(unsigned int DataToSend);
        bool InitializeBus();
        
        void SetParity(unsigned short int Parity);
        
        void SetSlave(unsigned int Slave);
    private:
        unsigned int SPICONValue;
        unsigned int SPISTATValue;
        unsigned short int Parity;
        unsigned int CurrentSlave;
            
};


#endif	/* DEVICE_H */

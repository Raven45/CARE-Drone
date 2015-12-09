/* 
 * File:   Object.h
 * Author: aburns2
 *
 * Created on November 14, 2015, 1:09 PM
 */

#ifndef OBJECT_H
#define	OBJECT_H

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>

#define LOOP_INFINITELY while(1)

typedef unsigned int UnsignedInteger32;
typedef signed int SignedInteger32;
typedef unsigned short int UnsignedInteger16;
typedef signed int SignedInteger16;
typedef unsigned char UnsignedByte;
typedef signed char SignedByte;


class Object {
    
    enum ObjectStates {
        Uninitialized,
        Initialized
    };
    
    public:
        Object();
        ~Object();
        
        short unsigned int GetState();
    protected:
        Object * Reference;
        short unsigned int State;
};


#endif	/* OBJECT_H */


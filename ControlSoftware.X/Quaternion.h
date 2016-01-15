/* 
 * File:   Quaternion.h
 * Author: aburns2
 *
 * Created on January 14, 2016, 10:15 PM
 */

#ifndef QUATERNION_H
#define	QUATERNION_H

#include "Object.h"
#include <math.h>

namespace Math {

class Quaternion {
public:
    Quaternion();
    Quaternion(Quaternion &Copy);
    Quaternion(float X, float Y, float Z);
    Quaternion(float q0, float q1, float q2, float q3);
    
    float& operator[](UnsignedInteger8 i);
    Quaternion operator*(Quaternion B);
    
    void Normalize();
    Quaternion GetComplexConjugate();
    
    float GetRoll();
    float GetPitch();
    float GetYaw();
    
private:
    float q0;
    float q1;
    float q2;
    float q3;
    
    float FastInverseSquareRoot(float x);
};
}

#endif	/* QUATERNION_H */


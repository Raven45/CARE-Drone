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

#define Q_EPSILON 1.0e-14

namespace Math {

class Quaternion {
    
public:
    Quaternion();
    Quaternion(const Quaternion &Copy);
    Quaternion(float theta, float phi);
    Quaternion(float Alpha, float Beta, float Gamma);
    Quaternion(float q0, float q1, float q2, float q3);
    
    //operator overloads
    Quaternion& operator=(const Quaternion& C);
    Quaternion& operator+=(const Quaternion& C);
    Quaternion operator-() const;
    Quaternion operator+(const float t) const;
    Quaternion operator-(const float t) const;
    Quaternion operator*(const float t) const;
    Quaternion operator/(const float t) const;
    Quaternion operator+(const Quaternion& B) const;
    Quaternion operator-(const Quaternion& B) const;
    Quaternion operator*(const Quaternion& B) const;
    Quaternion operator/(const Quaternion& B) const;
    float operator[](const UnsignedInteger8 i) const;
    float& operator[](const UnsignedInteger8 i);
    bool operator!=(const Quaternion& B) const;
    bool operator==(const Quaternion& B) const;
    
    //Mathematical operations
    Quaternion CrossProduct(const Quaternion& B) const;
    Quaternion Commutator(const Quaternion& B) const;
    float DotProduct(const Quaternion& B) const;
    float AbsoluteValue() const;
    Quaternion Exp() const;
    Quaternion Sqrt() const;
    Quaternion Angle() const;
    Quaternion Inverse() const;
    Quaternion Conjugate() const;
    Quaternion Normalize() const;
    
private:
    float q0;
    float q1;
    float q2;
    float q3;
    
    float FastInverseSquareRoot(float x) const;
};

Quaternion operator+(const float A, const Quaternion& B);
Quaternion operator-(const float A, const Quaternion& B);
Quaternion operator*(const float A, const Quaternion& B);
Quaternion operator/(const float A, const Quaternion& B);
Quaternion Exp(const Quaternion& B);



}

#endif	/* QUATERNION_H */


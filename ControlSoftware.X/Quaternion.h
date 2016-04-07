/*******************************************************************************
 * Copyright 2016 Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


 * Created on January 14, 2016, 10:15 PM
 * Redesigned on January 26th, 2016
*******************************************************************************/

#ifndef QUATERNION_H
#define	QUATERNION_H

#include "Object.h"
#include <math.h>

#define Q_EPSILON 1.0e-14
#define PI 3.14159265359f

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
    Quaternion& operator-=(const Quaternion& C);
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
    bool operator>(const Quaternion & B) const;
    bool operator>=(const Quaternion& B) const;
    bool operator<(const Quaternion& B) const;
    bool operator<=(const Quaternion& B) const;
    
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
    
    float GetRoll() const;
    float GetPitch() const;
    float GetYaw() const;
    
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


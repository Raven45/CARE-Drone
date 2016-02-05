/*******************************************************************************
  * Copyright 2016  Aaron Burns,
 *                  Joshua Donaway,
 *                  Matthew Love,
 *                  Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*******************************************************************************/

#include "Quaternion.h"

Math::Quaternion::Quaternion() {
    
    q0 = 0.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
}

Math::Quaternion::Quaternion(const Quaternion &Copy) {
    
    this->q0 = Copy.q0;
    this->q1 = Copy.q1;
    this->q2 = Copy.q2;
    this->q3 = Copy.q3;
}

Math::Quaternion::Quaternion(float theta, float phi){
    
    *this = Math::Exp((phi/2)*Quaternion(0.0f, 0.0f, 0.0f, 1.0f)) * 
            Math::Exp((theta/2)*Quaternion(0.0f, 0.0f, 1.0f, 0.0f));
}

Math::Quaternion::Quaternion(float Alpha, float Beta, float Gamma) {
    
    *this = Math::Exp((Alpha/2)*Quaternion(0.0f, 0.0f, 0.0f, 1.0f))* 
            Math::Exp((Beta/2)*Quaternion(0.0f, 0.0f, 1.0f, 0.0f)) * 
            Math::Exp((Gamma/2)*Quaternion(0.0f, 0.0f, 0.0f, 1.0f));;
}

Math::Quaternion::Quaternion(float q0, float q1, float q2, float q3) {
    
    this->q0 = q0;
    this->q1 = q1;
    this->q2 = q2;
    this->q3 = q3;
}

//operator overloads
Math::Quaternion& Math::Quaternion::operator=(const Quaternion& C){

    this->q0 = C.q0;
    this->q1 = C.q1;
    this->q2 = C.q2;
    this->q3 = C.q3;    
    return *this;
}

Math::Quaternion& Math::Quaternion::operator+=(const Quaternion& C){
    
    this->q0 += C.q0;
    this->q1 += C.q1;
    this->q2 += C.q2;    
    this->q3 += C.q3;    
    return *this;
}

Math::Quaternion& Math::Quaternion::operator-=(const Quaternion& C){
    
    this->q0 -= C.q0;
    this->q1 -= C.q1;
    this->q2 -= C.q2;    
    this->q3 -= C.q3;    
    return *this;
}

Math::Quaternion Math::Quaternion::operator-() const {

    Quaternion N(-q0, -q1, -q2, -q3);
    return N;
}

Math::Quaternion Math::Quaternion::operator+(const float t) const {
    
    Quaternion N(q0+t, q1, q2, q3);
    return N;
}

Math::Quaternion Math::Quaternion::operator-(const float t) const {
    
    Quaternion N(q0-t, q1, q2, q3);
    return N;
}

Math::Quaternion Math::Quaternion::operator*(const float t) const {
    
    Quaternion N(q0*t, q1*t, q2*t, q3*t);
    return N;
}

Math::Quaternion Math::Quaternion::operator/(const float t) const {
    
    Quaternion N(q0/t, q1/t, q2/t, q3/t);
    return N;
}

Math::Quaternion Math::Quaternion::operator+(const Quaternion& B) const {
    
    Quaternion N(q0+B.q0, q1+B.q1, q2+B.q2, q3+B.q3);
    return N;
}

Math::Quaternion Math::Quaternion::operator-(const Quaternion& B) const {

    Quaternion N(q0-B.q0, q1-B.q1, q2-B.q2, q3-B.q3);
    return N;
}

Math::Quaternion Math::Quaternion::operator*(const Quaternion& B) const {
    
    Quaternion N;
    N.q0 = q0*B.q0 - q1*B.q1 - q2*B.q2 - q3*B.q3;
    N.q1 = q0*B.q1 + q1*B.q0 + q2*B.q3 - q3*B.q2;
    N.q2 = q0*B.q2 - q1*B.q3 + q2*B.q0 + q3*B.q1;
    N.q3 = q0*B.q3 + q1*B.q2 - q2*B.q1 + q3*B.q0;
    return N;
}

Math::Quaternion Math::Quaternion::operator/(const Quaternion& B) const {
    
    return (*this)*B.Inverse();
}

float Math::Quaternion::operator[](const UnsignedInteger8 i) const {
    
    switch (i) {
        case 0: return q0;
        case 1: return q1;
        case 2: return q2;
        case 3: return q3;
        default: throw "Out of bounds error!";
    }
}

float& Math::Quaternion::operator[](const UnsignedInteger8 i) {
    
    switch (i) {
        case 0: return q0;
        case 1: return q1;
        case 2: return q2;
        case 3: return q3;
        default: throw "Out of bounds error!";
    }
}

bool Math::Quaternion::operator!=(const Quaternion& B) const {
    return !((*this) == B);
}

bool Math::Quaternion::operator==(const Quaternion& B) const {
    
    if (q0 == B.q0 &&
        q1 == B.q1 &&
        q2 == B.q2 &&
        q3 == B.q3) {
        
        return true;
    }
    else {
        return false;
    }
}


Math::Quaternion Math::Quaternion::CrossProduct(const Quaternion& B) const {
    
    Quaternion CrossProduct;
    
    CrossProduct.q0 = 0;
    CrossProduct.q1 = (-q3*B.q2) + (q2*B.q3);
    CrossProduct.q2 = (q3*B.q1) + (-q1*B.q3);
    CrossProduct.q3 = (-q2*B.q1) + (q1*B.q2);
    
    return CrossProduct;
}

Math::Quaternion Math::Quaternion::Commutator(const Quaternion& B) const {
    
    Quaternion N;
    
    N.q0 = 0;
    N.q1 = (-2*q3*B.q2) + (2*q2*B.q3);
    N.q2 = (2*q3*B.q1) + (-2*q1*B.q3);
    N.q3 = (-2*q2*B.q1) + (2*q1*B.q2);
    
    return N;
}

float Math::Quaternion::DotProduct(const Quaternion& B) const {

    return (q1*B.q1 + q2 * B.q2 + q3 * B.q3);
}

float Math::Quaternion::AbsoluteValue() const {
    
    return std::sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
}


Math::Quaternion Math::Quaternion::Exp() const{
    
    Quaternion N;
    const float b = std::sqrtf(q1*q1+q2*q2+q3*q3);
    
    if (std::abs(b) <= Q_EPSILON*std::abs(q0)) {
        N.q0 = std::exp(q0);
    }
    
    else {
        const float e = std::exp(q0);
        const float f = std::sin(b)/b;
        N.q0 = e*std::cos(b);
        N.q1 = e*f*q1;
        N.q2 = e*f*q2;
        N.q3 = e*f*q3;
    }
    
    return N;
}

Math::Quaternion Math::Quaternion::Sqrt() const {
    
    const float abs = AbsoluteValue();
    return (*this/abs+1)*std::sqrtf(abs/(2+2*q0/abs));
}

Math::Quaternion Math::Quaternion::Inverse() const {
    
    return Conjugate()/(q0*q0 + q1*q1 + q2*q2 + q3*q3);
}

Math::Quaternion Math::Quaternion::Conjugate() const {
    
    return Quaternion(q0, -q1, -q2, -q3);
}

Math::Quaternion Math::Quaternion::Normalize() const {
    
    Quaternion N = *this;
    N.q0 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    N.q1 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    N.q2 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    N.q3 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    
    return N;
}

float Math::Quaternion::FastInverseSquareRoot(float x) const{
    
//	float halfx = 0.5f * x;
//	float y = x;
//	long i = *(long*)&y;
//	i = 0x5f3759df - (i>>1);
//	y = *(float*)&i;
//	y = y * (1.5f - (halfx * y * y));
//	return y;
    
    UnsignedInteger32 i = 0x5F1F1412 - (*(UnsignedInteger32*)&x >>1);
    float tmp = *(float*)&i;
    return tmp * (1.69000231f - 0.714158169f * x * tmp * tmp);
}


Math::Quaternion Math::operator+(const float A, const Math::Quaternion& B) {
    return B+A;
}

Math::Quaternion Math::operator-(const float A, const Math::Quaternion& B) {
    return (-B)+A;
}

Math::Quaternion Math::operator*(const float A, const Math::Quaternion& B) {
    return B*A;
}

Math::Quaternion Math::operator/(const float A, const Math::Quaternion& B) {
    return B.Inverse()*A;
}

Math::Quaternion Math::Exp(const Math::Quaternion& B) {
    return B.Exp();
}
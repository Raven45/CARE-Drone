#include "Quaternion.h"

Math::Quaternion::Quaternion() {
    
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
}

Math::Quaternion::Quaternion(Math::Quaternion &Copy) {
    
    this->q0 = Copy.q0;
    this->q1 = Copy.q1;
    this->q2 = Copy.q2;
    this->q3 = Copy.q3;
}

Math::Quaternion::Quaternion(float Roll, float Pitch, float Yaw) {
    
    Quaternion A;
    Quaternion B(0.0f, Roll, Pitch, Yaw);
    *this = A*B;
    Normalize();
}

Math::Quaternion::Quaternion(float q0, float q1, float q2, float q3) {
    
    this->q0 = q0;
    this->q1 = q1;
    this->q2 = q2;
    this->q3 = q3;
}
    
float& Math::Quaternion::operator[](UnsignedInteger8 i) {
    
    switch(i) {
        case 0: return q0; break;
        case 1: return q1; break;
        case 2: return q2; break;
        case 3: return q3; break;
    }
}

Math::Quaternion Math::Quaternion::operator*(Math::Quaternion B) {
    
    Quaternion N;
    N.q0 = q0*B.q0 - q1*B.q1 - q2*B.q2 - q3*B.q3;
    N.q1 = q0*B.q1 + q1*B.q0 + q2*B.q3 - q3*B.q2;
    N.q2 = q0*B.q2 - q1*B.q3 + q2*B.q0 + q3*B.q1;
    N.q3 = q0*B.q3 + q1*B.q2 - q2*B.q1 + q3*B.q0;
    
    return N;
}
    
void Math::Quaternion::Normalize() {
    q0 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q1 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q2 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q3 *= FastInverseSquareRoot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
}

Math::Quaternion Math::Quaternion::GetComplexConjugate() {
    
    
    return *(new Math::Quaternion(q0, -q1, -q2, -q3));
}
    
float Math::Quaternion::GetPitch() {
    return atan2(2*q1*q2 - 2*q0*q3, (2*q0*q0 + 2*q1*q1) - 1);
}

float Math::Quaternion::GetRoll() {
    return -1.0f * (asin(2*q1*q3 + 2*q0*q2));
}

float Math::Quaternion::GetYaw() {
    return atan2(2*q2*q3 - 2*q0*q1, (2*q0*q0 + 2*q3*q3) -1);
}

float Math::Quaternion::FastInverseSquareRoot(float x) {
    
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
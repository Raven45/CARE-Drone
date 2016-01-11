#include "Register.h"

HAL::Register::Register(UnsignedInteger32 RegisterAddress) {
    *(this->RegisterAddress) = RegisterAddress;
}

HAL::Register::~Register() {
    this->RegisterAddress = NULL;
}
   
bool HAL::Register::Initialize() {
    return true;
}

bool HAL::Register::Update() {
    return true;
}
    
UnsignedInteger32 HAL::Register::GetRegister() {
    
    if (RegisterAddress != NULL) {
        return *RegisterAddress;
    }
    else {
        return 0;
    }
}

bool HAL::Register::SetRegister(UnsignedInteger32 Value) {
    
    if (RegisterAddress != NULL) {
        *RegisterAddress = Value;
        return true;
    }
    else {
        return false;
    }
}

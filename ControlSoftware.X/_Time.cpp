#include "_Time.h"

Time::Time() { }


Time::~Time() { }

bool Time::Initialize(){
    MilliSeconds = 0;
    Seconds = 0;
}

bool Time::Update() {
    
}

UnsignedInteger32 Time::Tick() {
    
    MicroSeconds++;
    
    if (MicroSeconds == 1000) {
        MicroSeconds = 0;
        MilliSeconds++;
    }
    
    if (MilliSeconds == 1000) {
        MilliSeconds = 0;
        Seconds++;
    }
}

void Time::SetClock(UnsignedInteger32 Seconds,
                    UnsignedInteger32 MilliSeconds,
                    UnsignedInteger32 MicroSeconds) {
    
    this->Seconds = Seconds;
    this->MilliSeconds = MilliSeconds;
    this->MicroSeconds = MicroSeconds;
}

UnsignedInteger32 Time::GetTime() {
    return GetTime_S();
}

UnsignedInteger32 Time::GetTime_S() {
    
    if (MilliSeconds >= 500) {
        return Seconds + 1;
    }
    else {
        return Seconds;
    }
}

UnsignedInteger32 Time::GetTime_MS() {
    
    if (MicroSeconds >= 500) {
        return Seconds*1000 + MilliSeconds+1;
    }
    else {
        return Seconds*1000 + MilliSeconds;
    }
}

UnsignedInteger32 Time::GetTime_US() {
    
    return Seconds*1000*1000 + MilliSeconds*1000 + MicroSeconds;
}


UnsignedInteger32 Time::operator[](const UnsignedInteger8 i) const {
    
    switch (i) {
        case 0: return Seconds; break;
        case 1: return MilliSeconds; break;
        case 2: return MicroSeconds; break;
        default: throw "Out of bounds error!";
    }
}

UnsignedInteger32& Time::operator[](const UnsignedInteger8 i) {
    
    switch (i) {
        case 0: return Seconds; break;
        case 1: return MilliSeconds; break;
        case 2: return MicroSeconds; break;
        default: throw "Out of bounds error!";
    }
}


bool Time::operator !=(const Time& B) const {
    
    if (this->Seconds != B.Seconds) { return true; }
    else if (this->MilliSeconds != B.MilliSeconds) { return true; }
    else if (this->MicroSeconds != B.MicroSeconds) { return true; }
    else { return false; }
}

bool Time::operator ==(const Time& B) const {
    return !(*this != B);
}

bool Time::operator >(const Time& B) const {
    
    if (*this != B) {
        
        if (this->Seconds == B.Seconds) {
            
            if (this->MilliSeconds == B.MilliSeconds) {
                return this->MicroSeconds > B.MicroSeconds;
            }
            else {
                return this->MilliSeconds > B.MilliSeconds;
            }
        }
        else {
            return this->Seconds > B.Seconds;
        }
    }
    else {
        return false;
    }
}

bool Time::operator >=(const Time& B) const {
    
    return (*this > B) || (*this == B);
}

bool Time::operator <(const Time& B) const {
    
    if (*this != B) {
        
        if (this->Seconds == B.Seconds) {
            
            if (this->MilliSeconds == B.MilliSeconds) {
                return this->MicroSeconds < B.MicroSeconds;
            }
            else {
                return this->MilliSeconds < B.MilliSeconds;
            }
        }
        else {
            return this->Seconds < B.Seconds;
        }
    }
    else {
        return false;
    }
}

bool Time::operator <=(const Time& B) const {
    
    return (*this < B) || (*this == B);
}
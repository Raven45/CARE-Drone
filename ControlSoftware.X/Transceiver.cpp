//#include "Transceiver.h"
//
//HAL::Transceiver::Transceiver(ADDRESS Address, SPIBus* DeviceManager): 
//                HAL::SPIDevice::SPIDevice(Address, DeviceManager) {
//    
//    this->BufferSize = 16;
//    this->Buffer = new UnsignedInteger16[BufferSize];
//}
//
//HAL::Transceiver::~Transceiver() {
//    
//}
//
//bool HAL::Transceiver::Initialize() {
//    
//    Time Delay;
//    Delay.SetClock(0,100,0);
//    CoreTimerReference->Delay(Delay);
//    
//    PORTClearBits(IOPORT_C, BIT_7);             //Set RC6 to LOW
//    
//    Status = SendAndReceive(0xFFFF) & 0x00FF;
//    SendAndReceive(0x200A);     //Power the chip up.
//    
//    
//}
//
//bool HAL::Transceiver::Update() {
//    
//    Status = SendAndReceive(0xFFFF) & 0x00FF;
//    
//}
//
//bool HAL::Transceiver::LoadBuffer(  Math::Quaternion Orientation,
//                                    SignedInteger32 Altitude,
//                                    UnsignedInteger16 Motor1Speed,
//                                    UnsignedInteger16 Motor2Speed,
//                                    UnsignedInteger16 Motor3Speed,
//                                    UnsignedInteger16 Motor4Speed,
//                                    UnsignedInteger16 Motor5Speed,
//                                    UnsignedInteger16 Motor6Speed) {
//    
//    UnsignedInteger16 FIFO_Status = SendAndReceive(0x17FF) & 0x00FF;
//    
//    if (this->Buffer != NULL) {
//        FloatBuf buf;
//
//        buf.a = Orientation[0];
//        this->Buffer[0] = buf.b[0];
//        this->Buffer[1] = buf.b[1];
//
//        buf.a = Orientation[1];
//        this->Buffer[2] = buf.b[0];
//        this->Buffer[3] = buf.b[1];
//
//        buf.a = Orientation[2];
//        this->Buffer[4] = buf.b[0];
//        this->Buffer[5] = buf.b[1];
//
//        buf.a = Orientation[3];
//        this->Buffer[6] = buf.b[0];
//        this->Buffer[7] = buf.b[1];
//
//        this->Buffer[8] = (UnsignedInteger16)(Altitude >> 16);
//        this->Buffer[9] = (UnsignedInteger16)(Altitude & 0x0000FFFF);
//
//        this->Buffer[10] = Motor1Speed;
//        this->Buffer[11] = Motor2Speed;
//        this->Buffer[12] = Motor3Speed;
//        this->Buffer[13] = Motor4Speed;
//        this->Buffer[14] = Motor5Speed;
//        this->Buffer[15] = Motor6Speed;
//    
//        SendAndReceiveBurst(this->Buffer, BufferSize);
//        
//        return true;
//    }
//    else {
//        return false;
//    }
//}
//
//bool HAL::Transceiver::Transmit() {
//    
//    PORTSetBits(IOPORT_C, BIT_7);             //Set RC6 to LOW
//    
//    Time Delay;
//    Delay.SetClock(0,3,0);
//    CoreTimerReference->Delay(Delay);
//    
//    PORTClearBits(IOPORT_C, BIT_7);             //Set RC6 to LOW
//}
//
//bool HAL::Transceiver::SetTimerReference(HAL::Timer* CoreTimerReference) {
//    
//    this->CoreTimerReference = CoreTimerReference;
//    
//    if (this->CoreTimerReference == NULL) {
//        return false;
//    }
//    else {
//        return true;
//    }
//}
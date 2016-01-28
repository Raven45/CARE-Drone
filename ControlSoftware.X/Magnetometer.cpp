#include "Magnetometer.h"

HAL::Magnetometer::Magnetometer(ADDRESS Address, SPIBus* DeviceManager): 
                HAL::SPIDevice::SPIDevice(Address, DeviceManager){
    
    this->Parity = ParityTypes::OddParity;
}

HAL::Magnetometer::~Magnetometer() {}

bool HAL::Magnetometer::Initialize() { 

    //Perform soft reset on magnetometer.
    SendAndReceive(MAG_SOFT_RESET);
    Delay(100);
    
    //Wake the darn thing up.
    SendAndReceive(MAG_WAKE_UP);
    Delay(100);
    
    //Initialize Magnetometer to "normal" mode. See datasheet.
    SendAndReceive(MAG_NORMAL_MODE);
    
    SendAndReceive(MAG_REPS_XY);
    SendAndReceive(MAG_REPS_Z);
    
#ifdef EXTENDED_REGISTERS
    dig_x1 =    SendAndReceive(MAG_DIG_X1);
    dig_x2 =    SendAndReceive(MAG_DIG_X2);
    dig_y1 =    SendAndReceive(MAG_DIG_Y1);
    dig_y2 =    SendAndReceive(MAG_DIG_Y2);
    dig_xy1 =   SendAndReceive(MAG_DIG_XY1);
    dig_xy2 =   SendAndReceive(MAG_DIG_XY2);
    dig_z1 =    (SendAndReceive(MAG_DIG_Z1_MSB) << 8) | 
                (SendAndReceive(MAG_DIG_Z1_LSB));
    dig_z2 =    (SendAndReceive(MAG_DIG_Z2_MSB) << 8) | 
                (SendAndReceive(MAG_DIG_Z2_LSB));
    dig_z3 =    (SendAndReceive(MAG_DIG_Z3_MSB) << 8) | 
                (SendAndReceive(MAG_DIG_Z3_LSB));
    dig_z4 =    (SendAndReceive(MAG_DIG_Z4_MSB) << 8) | 
                (SendAndReceive(MAG_DIG_Z4_LSB));
    dig_xyz1 =  (SendAndReceive(MAG_DIG_XYZ1_MSB) << 8) | 
                (SendAndReceive(MAG_DIG_XYZ1_LSB));
#endif
}

bool HAL::Magnetometer::Update() {
    
    SignedInteger16 MagX = 0;
    SignedInteger16 MagY = 0;
    SignedInteger16 MagZ = 0;
    
    UnsignedInteger16 Commands[8] = {
        MAG_X_LSB,
        MAG_X_MSB,
        MAG_Y_LSB,
        MAG_Y_MSB,
        MAG_Z_LSB,
        MAG_Z_MSB,
        MAG_HALL_LSB,
        MAG_HALL_MSB
    };
    SignedInteger16 * Incoming = (SignedInteger16*)SendAndReceiveBurst(Commands, 8);
    
    /***************************************************************************
     * Refer to page 131 of the BMX055 datasheet: the data coming through is a 
     * 12 bit signed integer. bmm050 
     **************************************************************************/
    if (Incoming[6] & 0x01) {
        MagX = (Incoming[1] << 8) | Incoming[0] >> 3;
        MagY = (Incoming[3] << 8) | Incoming[2] >> 3;
        MagZ = (Incoming[5] << 8) | Incoming[4] >> 1;
        HallResistance = (Incoming[7] << 8) | Incoming[6] >> 2;
    
    
#ifdef EXTENDED_REGISTERS
    SignedInteger16 Temperature = 0;
    
    //Don't ask.
    Temperature = ((SignedInteger16)(((UnsignedInteger16)
            ((((SignedInteger32)dig_xyz1) << 14)/
            (HallResistance != 0 ? HallResistance : dig_xyz1))) - 
            ((UnsignedInteger16)0x4000)));
    this->MagX = ((SignedInteger16)((((SignedInteger32)MagX) *
				((((((((SignedInteger32)dig_xy2) * ((((SignedInteger32)Temperature) * ((SignedInteger32)Temperature)) >> 7)) +
			     (((SignedInteger32)Temperature) * ((SignedInteger32)(((SignedInteger16)dig_xy1) << 7)))) >> 9) +
			   ((SignedInteger32)0x100000)) * ((SignedInteger32)(((SignedInteger16)dig_x2) + ((SignedInteger16)0xA0)))) >> 12)) >> 13)) +
			(((SignedInteger16)dig_x1) << 3);
    Temperature = ((SignedInteger16)(((UnsignedInteger16)((((SignedInteger32)dig_xyz1) << 14)/(HallResistance != 0 ? HallResistance : dig_xyz1))) - ((UnsignedInteger16)0x4000)));
    this->MagY = ((SignedInteger16)((((SignedInteger32)MagY) *
				((((((((SignedInteger32)dig_xy2) * ((((SignedInteger32)Temperature) * ((SignedInteger32)Temperature)) >> 7)) + 
			     (((SignedInteger32)Temperature) * ((SignedInteger32)(((SignedInteger16)dig_xy1) << 7)))) >> 9) +
		           ((SignedInteger32)0x100000)) * ((SignedInteger32)(((SignedInteger16)dig_y2) + ((SignedInteger16)0xA0)))) >> 12)) >> 13)) +
			(((SignedInteger16)dig_y1) << 3);
    this->MagZ = (((((SignedInteger32)(MagZ - dig_z4)) << 15) - ((((SignedInteger32)dig_z3) * ((SignedInteger32)(((SignedInteger16)HallResistance) -
	((SignedInteger16)dig_xyz1))))>>2))/(dig_z2 + ((SignedInteger16)(((((SignedInteger32)dig_z1) * ((((SignedInteger16)HallResistance) << 1)))+(1<<15))>>16))));
#endif
    }
}

float HAL::Magnetometer::GetMagX() {
    return MagX;
}

float HAL::Magnetometer::GetMagY() {
    return MagY;
}

float HAL::Magnetometer::GetMagZ() {
    return MagZ;
}

float HAL::Magnetometer::GetHallResistance() {
    return HallResistance;
}

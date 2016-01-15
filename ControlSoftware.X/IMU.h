/* 
 * File:   IMU.h
 * Author: aburns2
 *
 * Created on January 14, 2016, 2:49 PM
 */

#ifndef IMU_H
#define	IMU_H

#include "SPIDevice.h"
#include "Quaternion.h"

#define ACC_CHIPID      0x00
#define ACC_ACCD_X      0x02
#define ACC_ACCD_Y      0x04
#define ACC_ACCD_Z      0x06
#define ACC_ACCD_TEMP   0x08

#define GYRO_CHIPID     0x00
#define GYRO_RATE_X     0x02
#define GYRO_RATE_Y     0x04
#define GYRO_RATE_Z     0x06




namespace HAL {

class IMU: public SPIDevice {
    
public:
    IMU(ADDRESS Address, SPIBus* DeviceManager);
    ~IMU();
    
    bool Initialize();
    bool Update();
    
    Math::Quaternion GetOrientation();
    
private:
    ADDRESS AccelAddress;
    ADDRESS GyroAddress;
    ADDRESS MagAddress;
    
    Math::Quaternion Orientation;
    
    bool UsingMagnetometer;
};
}

#endif	/* IMU_H */


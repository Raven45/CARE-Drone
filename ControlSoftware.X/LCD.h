/* 
 * File:   LCD.h
 * Author: aburns2
 *
 * Created on January 14, 2016, 2:50 PM
 */

#ifndef LCD_H
#define	LCD_H


#include "SPIDevice.h"


namespace HAL {

class LCD: public SPIDevice {
    
public:
    LCD(ADDRESS Address, SPIBus* DeviceManager);
    ~LCD();
    
    bool Initialize();
    bool Update();
    
    bool PrintString(std::string Message);
    
private:
};

}


#endif	/* LCD_H */
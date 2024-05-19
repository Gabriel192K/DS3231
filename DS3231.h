#ifndef __DS3231_H__
#define __DS3231_H__

/* Dependencies */
#include "../TWI/TWI.h"
#include "DS3231 Mnemonics.h"

class DS3231
{
    public:
        DS3231(__TWI__* twi);
        ~DS3231();
        const uint8_t begin         (void);
        const uint8_t isConnected   (void);
        const uint8_t setTime       (const uint8_t hours, const uint8_t minutes, const uint8_t seconds);
        const uint8_t getTime       (uint8_t* hours, uint8_t* minutes, uint8_t* seconds);
        const uint8_t setDayOfWeek  (const uint8_t dow);
        const uint8_t getDayOfWeek  (uint8_t* dow);
        const uint8_t setDate       (const uint8_t date, const uint8_t month, const uint8_t year);
        const uint8_t getDate       (uint8_t* date, uint8_t* month, uint8_t* year);
        const uint8_t getTemperature(uint16_t* temperature);
        const uint8_t end           (void);
    private:
        uint8_t began;
        __TWI__* twi;
        const uint8_t bcd2dec(const uint8_t bcd);
        const uint8_t dec2bcd(const uint8_t dec);
};

#endif

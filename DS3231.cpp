/* Dependencies */
#include "DS3231.h"

/*!
 * @brief  Constructor of DS3231 class
 * @param  twi
 *         The TWI bus object pointer
 */
DS3231::DS3231(__TWI__* twi)
{
    this->twi = twi;
}

/*!
 * @brief  Destructor of DS3231 class
 */
DS3231::~DS3231()
{
    this->twi = NULL;
}

/*!
 * @brief  Begins the DS3231 implementation
 * @return Returns 0 if the DS3231 implementation already began, otherwise returns status of TWI connection
 */
const uint8_t DS3231::begin(void)
{
    if (this->began)
        return (0);
    this->began = 1;
    this->twi->begin();
    return (this->isConnected());
}

/*!
 * @brief  Checks the DS3231 TWI connection
 * @return Returns status of TWI connection
 */
const uint8_t DS3231::isConnected(void)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    return (this->twi->endTransmission() == 1);
}

/*!
 * @brief  Setting the value of time specific registers
 * @param  hours
 *         The hours value to set
 * @param  minutes
 *         The minutes value to set
 * @param  seconds
 *         The seconds value to set
 * @return Returns status of TWI transmission
 */
const uint8_t DS3231::setTime(const uint8_t hours, const uint8_t minutes, const uint8_t seconds)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    this->twi->write(DS3231_SECONDS_REGISTER_ADDRESS);
    this->twi->write(dec2bcd(seconds));
    this->twi->write(dec2bcd(minutes));
    this->twi->write(dec2bcd(hours));
    return (this->twi->endTransmission() == 1);
}

/*!
 * @brief  Getting the value of time specific registers
 * @param  hours
 *         The hours value to get
 * @param  minutes
 *         The minutes value to get
 * @param  seconds
 *         The seconds value to get
 * @return Returns 0 if the requested amount did not match, otherwise returns 1
 */
const uint8_t DS3231::getTime(uint8_t* hours, uint8_t* minutes, uint8_t* seconds)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    this->twi->write(DS3231_SECONDS_REGISTER_ADDRESS);
    this->twi->endTransmission();
    if (this->twi->requestFrom(DS3231_TWI_ADDRESS, (const uint8_t)3, (const uint8_t)1) != (const uint8_t)3)
        return (0);
    *seconds = this->bcd2dec(this->twi->read());
    *minutes = this->bcd2dec(this->twi->read());
    *hours = this->bcd2dec(this->twi->read());
    return (1);
}

/*!
 * @brief  Setting the value of day of week register
 * @param  dow
 *         The day of week value to set
 * @return Returns status of TWI transmission
 */
const uint8_t DS3231::setDayOfWeek(const uint8_t dow)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    this->twi->write(DS3231_DAY_OF_WEEK_REGISTER_ADDRESS);
    this->twi->write(dec2bcd(dow));
    return (this->twi->endTransmission() == 1);
}

/*!
 * @brief  Getting the value of day of week register
 * @param  dow
 *         The day of week value to get
 * @return Returns 0 if the requested amount did not match, otherwise returns 1
 */
const uint8_t DS3231::getDayOfWeek(uint8_t* dow)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    this->twi->write(DS3231_DAY_OF_WEEK_REGISTER_ADDRESS);
    this->twi->endTransmission();
    if (this->twi->requestFrom(DS3231_TWI_ADDRESS, (const uint8_t)1, (const uint8_t)1) != (const uint8_t)1)
        return (0);
    *dow = this->bcd2dec(this->twi->read());
    return (1)
}

/*!
 * @brief  Setting the value of date specific registers
 * @param  date
 *         The date value to set
 * @param  month
 *         The month value to set
 * @param  year
 *         The year value to set
 * @return Returns status of TWI transmission
 */
const uint8_t DS3231::setDate(const uint8_t date, const uint8_t month, const uint8_t year)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    this->twi->write(DS3231_DATE_REGISTER_ADDRESS);
    this->twi->write(dec2bcd(date));
    this->twi->write(dec2bcd(month));
    this->twi->write(dec2bcd(year));
    return (this->twi->endTransmission() == 1);
}

/*!
 * @brief  Getting the value of date specific registers
 * @param  date
 *         The date value to get
 * @param  month
 *         The month value to get
 * @param  year
 *         The year value to get
 * @return Returns 0 if the requested amount did not match, otherwise returns 1
 */
const uint8_t DS3231::getDate(uint8_t* date, uint8_t* month, uint8_t* year)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    this->twi->write(DS3231_DATE_REGISTER_ADDRESS);
    this->twi->endTransmission();
    if (this->twi->requestFrom(DS3231_TWI_ADDRESS, (const uint8_t)3, (const uint8_t)1) != (const uint8_t)3)
        return (0);
    *date = this->bcd2dec(this->twi->read());
    *month = this->bcd2dec(this->twi->read());
    *year = this->bcd2dec(this->twi->read());
    return (1);
}

/*!
 * @brief  Getting the value of date specific registers
 * @param  temperature
 *         The temperature value to get
 * @return Returns 0 if the requested amount did not match, otherwise returns 1
 */
const uint8_t DS3231::getTemperature(uint16_t* temperature)
{
    this->twi->beginTransmission(DS3231_TWI_ADDRESS);
    this->twi->write(DS3231_TEMPERATURE_REGISTER_ADDRESS);
    this->twi->endTransmission();
    if (this->twi->requestFrom(DS3231_TWI_ADDRESS, (const uint8_t)2, (const uint8_t)1) != (const uint8_t)2)
        return (0);
    *temperature = ((((this->twi->read() << 8) | this->twi->read()) >> 6) >> 2);
    return (1);
}

/*!
 * @brief  Ends the DS3231 implementation
 * @return Returns 0 if the DS3231 implementation already ended, otherwise returns 1
 */
const uint8_t DS3231::end(void)
{
    if (!this->began)
        return (0);
    this->began = 0;
    return (1);
}

/*!
 * @brief  Converts a byte sized binary coded decimal number to a decimal number
 * @param  bcd
 *         The number to convert
 * @return Returns the converted byte
 */
const uint8_t bcd2dec(const uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

/*!
 * @brief  Converts a byte sized decimal number to a binary coded decimal number
 * @param  dec
 *         The number to convert
 * @return Returns the converted byte
 */
const uint8_t dec2bcd(const uint8_t dec)
{
    return ((dec / 10) * 16) + (dec % 10);
}

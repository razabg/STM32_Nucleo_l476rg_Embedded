#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>

typedef struct DateTime DateTime; // opaque


DateTime* DateTime_Create(void);
void DateTime_Destroy(DateTime* dt);

// Read current time/date from RTC into the DateTime object (binary internally)
void DateTime_ReadFromRTC(DateTime* dt);

// Write DateTime object's values to RTC (converts binary -> BCD internally)
void DateTime_WriteToRTC(const DateTime* dt);

// Setters (take plain binary/decimal values, e.g. SetTime(dt, 14, 30, 0))
void DateTime_SetTime(DateTime* dt, uint8_t hours, uint8_t minutes, uint8_t seconds);
void DateTime_SetDate(DateTime* dt, uint8_t year, uint8_t month, uint8_t date, uint8_t weekday);

// Print in "YY/MM/DD-d-hh:mm:ss" format over UART
void DateTime_Print(const DateTime* dt);

// BCD <-> binary helpers (exposed since you'll need them at the RTC boundary)
uint8_t BCD_To_Binary(uint8_t bcd);
uint8_t Binary_To_BCD(uint8_t bin);

#endif

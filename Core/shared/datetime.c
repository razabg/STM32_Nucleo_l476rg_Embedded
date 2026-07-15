#include "datetime.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

extern RTC_HandleTypeDef hrtc;

struct DateTime
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

    uint8_t weekday;
    uint8_t month;
    uint8_t date;
    uint8_t year;
};

//************** helper functions *****************
/*
 * BCD -> binary
 * 0x45 -> 69 decimal if read naively, but represents 45 in BCD
 * 0100 0101
 * high nibble (tens) = 4, low nibble (ones) = 5
 * int = (num >> 4) * 10 + (num & 15)
 *
 * binary -> BCD
 * 45 -> 0x45
 * tens = 45 / 10 = 4
 * ones = 45 % 10 = 5
 * result = (tens << 4) | ones
 */

uint8_t BCD_To_Binary(uint8_t bcd)
{
    return (bcd >> 4) * 10 + (bcd & 15);
}

uint8_t Binary_To_BCD(uint8_t bin)
{
    uint8_t tens = bin / 10;
    uint8_t ones = bin % 10;
    return (tens << 4) | ones;
}

//************** lifecycle *****************

DateTime* DateTime_Create(void)
{
    DateTime* dt = (DateTime*)malloc(sizeof(DateTime));
    if (dt == NULL)
    {
        // allocation failed
        return NULL;
    }
    return dt;
}

void DateTime_Destroy(DateTime* dt)
{
    if (dt == NULL)
    {
        return; // free(NULL) is safe per the C standard, but explicit guard is clearer intent
    }
    free(dt);
}

//************** RTC read/write *****************

void DateTime_ReadFromRTC(DateTime* dt)
{
    if (dt == NULL)
    {
        return;
    }

    RTC_TimeTypeDef t;
    RTC_DateTypeDef d;

    HAL_RTC_GetTime(&hrtc, &t, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &d, RTC_FORMAT_BCD); // must follow GetTime (shadow register rule)

    dt->hours   = BCD_To_Binary(t.Hours);
    dt->minutes = BCD_To_Binary(t.Minutes);
    dt->seconds = BCD_To_Binary(t.Seconds);

    dt->year    = BCD_To_Binary(d.Year);
    dt->month   = BCD_To_Binary(d.Month);
    dt->date    = BCD_To_Binary(d.Date);
    dt->weekday = d.WeekDay; // WeekDay is a plain 1-7 enum, not BCD-packed
}

void DateTime_WriteToRTC(const DateTime* dt)
{
    if (dt == NULL)
    {
        return;
    }

    RTC_TimeTypeDef t = {0};
    RTC_DateTypeDef d = {0};

    t.Hours   = Binary_To_BCD(dt->hours);
    t.Minutes = Binary_To_BCD(dt->minutes);
    t.Seconds = Binary_To_BCD(dt->seconds);
    HAL_RTC_SetTime(&hrtc, &t, RTC_FORMAT_BCD);

    d.Year    = Binary_To_BCD(dt->year);
    d.Month   = Binary_To_BCD(dt->month);
    d.Date    = Binary_To_BCD(dt->date);
    d.WeekDay = dt->weekday;
    HAL_RTC_SetDate(&hrtc, &d, RTC_FORMAT_BCD);
}

//************** setters *****************

void DateTime_SetTime(DateTime* dt, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    if (dt == NULL)
    {
        return;
    }
    dt->hours   = hours;
    dt->minutes = minutes;
    dt->seconds = seconds;
}

void DateTime_SetDate(DateTime* dt, uint8_t year, uint8_t month, uint8_t date, uint8_t weekday)
{
    if (dt == NULL)
    {
        return;
    }
    dt->year    = year;
    dt->month   = month;
    dt->date    = date;
    dt->weekday = weekday;
}

//************** print *****************

void DateTime_Print(const DateTime* dt)
{
    if (dt == NULL)
    {
        return;
    }

    printf("20%02u/%02u/%02u-%01u-%02u:%02u:%02u\r\n",
           dt->year, dt->month, dt->date, dt->weekday,
           dt->hours, dt->minutes, dt->seconds);
}



//
//DateTime* g_dt = NULL;
//g_dt = DateTime_Create();
//   if (g_dt == NULL)
//   {
//       printf("Failed to allocate DateTime\r\n");
//       Error_Handler();
//   }
//
//   // Set an initial time/date once (only needed the first time,
//   // or whenever the backup domain loses power / gets reset)
//   DateTime_SetTime(g_dt, 14,14, 0);        // 14:30:00
//   DateTime_SetDate(g_dt, 26, 7, 12, 1);      // 2026/07/09, weekday 4 (Thursday)
//   DateTime_WriteToRTC(g_dt);

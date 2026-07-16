/*
 * TOPIC: ADC Polling Mode — Reading Two Channels (Potentiometer + Light Sensor)
 *
 * Two ADC peripherals are read sequentially using polling (the simplest mode):
 *   ADC1 — potentiometer, configured 12-bit (0–4095 raw, 0–3300 mV)
 *   ADC2 — light sensor,  configured 8-bit  (0–255 raw)
 *
 * The polling sequence for each channel is always the same 4 steps:
 *   HAL_ADC_Start   → HAL_ADC_PollForConversion → HAL_ADC_GetValue → HAL_ADC_Stop
 *
 * Every 2nd loop iteration both channels are printed together; otherwise only
 * the rotation (potentiometer) value is printed.
 *
 * Voltage formula (integer math, no floats needed):
 *   voltage_mv = (raw_value * 3300) / 4095   [for 12-bit ADC]
 *
 * KEY APIS:
 *   HAL_ADC_Start(hadc)
 *   HAL_ADC_PollForConversion(hadc, timeout)
 *   HAL_ADC_GetValue(hadc)                   — returns uint32_t raw count
 *   HAL_ADC_Stop(hadc)
 *
 * In main(), before the while(1):
 *   MX_ADC1_Init();  MX_ADC2_Init();
 */

//#include "main.h"
//#include <stdio.h>
//
//extern ADC_HandleTypeDef hadc1;
//extern ADC_HandleTypeDef hadc2;
//
//  uint32_t raw_value_rotation;
//  uint32_t raw_value_light_sensor;
//  uint8_t second = 0;
//
//  while (1)
//  {
//	  second++;
//
//	  HAL_ADC_Start(&hadc2);
//	  HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
//	  raw_value_light_sensor = HAL_ADC_GetValue(&hadc2);
//	  HAL_ADC_Stop(&hadc2);
//	  uint32_t voltage_mv_light = (raw_value_light_sensor * 3300) / 4095;
//
//
//	 	HAL_ADC_Start(&hadc1);
//	 	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//		        raw_value_rotation = HAL_ADC_GetValue(&hadc1);
//		        HAL_ADC_Stop(&hadc1);
//
//		        uint32_t voltage_mv_rotation = (raw_value_rotation * 3300) / 4095;  // millivolts, as an integer // voltage = (raw_value / 4095.0f) * 3.3f;
//
//		        if (second == 2) {
//		        	printf("Raw rotation: %lu  Voltage rotation: %lu mV ********* Raw light: %lu  Voltage light: %lu mV\r\n", raw_value_rotation, voltage_mv_rotation,raw_value_light_sensor,voltage_mv_light);
//		        	second = 0;
//		        	continue;
//			}
//		        printf("Raw: %lu  Voltage: %lu mV\r\n", raw_value_rotation, voltage_mv_rotation);
//		        printf("*********************************************************************\r\n");
//
//		        HAL_Delay(1000);    // optional —
//
//
//  }

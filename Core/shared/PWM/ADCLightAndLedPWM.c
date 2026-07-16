/*
 * TOPIC: ADC Polling Mode + PWM Duty Cycle Control from Sensor Value
 *
 * *************************USING POLLING MODE*************************************
 *
 * Reads two ADC channels and uses the light sensor value to continuously
 * update the PWM duty cycle of an LED — the brighter the light, the brighter
 * the LED (or vice versa, depending on sensor polarity).
 *
 *   ADC1 — potentiometer, 12-bit (0–4095)
 *   ADC2 — light sensor,   8-bit  (0–255)
 *
 * Mapping:   duty_cycle = (raw_value_light * 999) / 255
 *   → scales 8-bit sensor range (0–255) to TIM3 ARR range (0–999)
 *   → written to TIM3 CH1 CCR via __HAL_TIM_SET_COMPARE
 *
 * In main(), before the while(1):
 *   HAL_TIM_Base_Start(&htim3);
 *   HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
 *   MX_ADC1_Init(); MX_ADC2_Init();
 *
 * KEY APIS:
 *   HAL_ADC_Start / PollForConversion / GetValue / Stop
 *   __HAL_TIM_SET_COMPARE(htim, channel, value)   — update CCR without restarting PWM
 */

//#include "main.h"
//#include <stdio.h>
//
//extern ADC_HandleTypeDef hadc1;
//extern ADC_HandleTypeDef hadc2;
//extern TIM_HandleTypeDef htim3;
//
// 	  uint32_t raw_value_light;
// 	  uint32_t duty_cycle;
// 	  uint32_t raw_value_rotation;
//
//  while (1)
//  {
//	      // --- Read potentiometer (ADC1, 12-bit) ---
//	      HAL_ADC_Start(&hadc1);
//	      HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//	      raw_value_rotation = HAL_ADC_GetValue(&hadc1);
//	      HAL_ADC_Stop(&hadc1);
//
//	      // --- Read light sensor (ADC2, 8-bit) ---
//	      HAL_ADC_Start(&hadc2);
//	      HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
//	      raw_value_light = HAL_ADC_GetValue(&hadc2);
//	      HAL_ADC_Stop(&hadc2);
//
//	      // --- Use light sensor to control PWM duty (0-255 -> 0-999) ---
//	      duty_cycle = (raw_value_light * 999) / 255;
//	      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty_cycle);
//
//
//  }

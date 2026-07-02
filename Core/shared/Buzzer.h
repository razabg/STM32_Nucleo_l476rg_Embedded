//#ifndef BUZZER_H
//#define BUZZER_H
//
//#include "main.h"
//#include <stdint.h>
//
//typedef struct Buzzer_Handle Buzzer_Handle;
//
//typedef enum {
//    NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1,
//    NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1,
//    NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2,
//    NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2
//} Note;
//
//Buzzer_Handle *Buzzer_Create(TIM_HandleTypeDef *pwm_timer, uint32_t channel,
//                              TIM_HandleTypeDef *duration_timer);
//void Buzzer_PlayNote(Buzzer_Handle *h, Note note, uint32_t duration_ms);
//void Buzzer_Stop(Buzzer_Handle *h);
//void Buzzer_DurationElapsed(Buzzer_Handle *h);  // called from TIM6 ISR
//
//
//// Return codes so caller knows if read succeeded
//typedef enum {
//	BUZZER_OK,
//	BUZZER_NO_RESPONSE,
//} BUZZER_Result;
//
//
//
//#endif

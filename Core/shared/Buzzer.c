//#include "buzzer.h"
//#include <stdlib.h>
//
//#define NOTE_COUNT 24
//
//typedef struct {
//    uint16_t period;
//    uint16_t pulse;
//} NoteValues;
//
//static const NoteValues note_table[NOTE_COUNT] = {
//    {3815, 1907}, {3609, 1804}, {3400, 1700}, {3214, 1607},
//    {3029, 1514}, {2864, 1432}, {2701, 1350}, {2550, 1275},
//    {2408, 1204}, {2271, 1135}, {2144, 1072}, {2023, 1011},
//    {1911,  955}, {1804,  902}, {1702,  851}, {1606,  803},
//    {1516,  758}, {1431,  715}, {1350,  675}, {1274,  637},
//    {1202,  601}, {1135,  567}, {1071,  535}, {1011,  505}
//};
//
//struct Buzzer_Handle {
//    TIM_HandleTypeDef *pwm_timer;
//    uint32_t channel;
//    TIM_HandleTypeDef *duration_timer;
//};
//
//Buzzer_Handle *Buzzer_Create(TIM_HandleTypeDef *pwm_timer, uint32_t channel,
//                              TIM_HandleTypeDef *duration_timer)
//{
//    Buzzer_Handle *h = malloc(sizeof(Buzzer_Handle));
//    if (!h) return NULL;
//    h->pwm_timer = pwm_timer;
//    h->channel = channel;
//    h->duration_timer = duration_timer;
//    return h;
//}
//
//void Buzzer_PlayNote(Buzzer_Handle *h, Note note, uint32_t duration_ms)
//{
//    if (note >= NOTE_COUNT) return;
//
//    // restart duration timer if a note is already sounding
//    HAL_TIM_Base_Stop_IT(h->duration_timer);
//    __HAL_TIM_SET_COUNTER(h->duration_timer, 0);
//
//    __HAL_TIM_SET_AUTORELOAD(h->pwm_timer, note_table[note].period);
//    __HAL_TIM_SET_COMPARE(h->pwm_timer, h->channel, note_table[note].pulse);
//
//    HAL_TIM_PWM_Start(h->pwm_timer, h->channel);
//    HAL_TIM_Base_Start_IT(h->duration_timer);
//}
//
//void Buzzer_Stop(Buzzer_Handle *h)
//{
//    HAL_TIM_PWM_Stop(h->pwm_timer, h->channel);
//    HAL_TIM_Base_Stop_IT(h->duration_timer);
//}
//
//void Buzzer_DurationElapsed(Buzzer_Handle *h)
//{
//    HAL_TIM_PWM_Stop(h->pwm_timer, h->channel);
//}
//

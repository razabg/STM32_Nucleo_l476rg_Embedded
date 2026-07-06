#include "buzzer.h"
#include <stdlib.h>

#define NOTE_COUNT 24
#define LITTLE_YONATAN_LENGTH (sizeof(little_yonatan) / sizeof(little_yonatan[0]))

typedef struct {
    uint16_t period;
    uint16_t pulse;
} NoteValues;

static const NoteValues note_table[NOTE_COUNT] = { //index 0 == note c etc...
    {3815, 1907}, {3609, 1804}, {3400, 1700}, {3214, 1607},
    {3029, 1514}, {2864, 1432}, {2701, 1350}, {2550, 1275},
    {2408, 1204}, {2271, 1135}, {2144, 1072}, {2023, 1011},
    {1911,  955}, {1804,  902}, {1702,  851}, {1606,  803},
    {1516,  758}, {1431,  715}, {1350,  675}, {1274,  637},
    {1202,  601}, {1135,  567}, {1071,  535}, {1011,  505}
};

struct Buzzer_Handle {
    TIM_HandleTypeDef *pwm_timer;
    uint32_t channel;
    TIM_HandleTypeDef *duration_timer;
};

Buzzer_Handle *Buzzer_Create(TIM_HandleTypeDef *pwm_timer, uint32_t channel,
                              TIM_HandleTypeDef *duration_timer)
{
    Buzzer_Handle *h = malloc(sizeof(Buzzer_Handle));
    if (!h) return NULL;
    h->pwm_timer = pwm_timer;
    h->channel = channel;
    h->duration_timer = duration_timer;
    return h;
}

void Buzzer_PlayNote(Buzzer_Handle *h, Note note, uint32_t duration_ms)
{
    HAL_TIM_Base_Stop_IT(h->duration_timer);
    __HAL_TIM_SET_COUNTER(h->duration_timer, 0);

    // convert duration_ms into ARR ticks for the duration timer (10kHz tick rate) each tick 0.1ms
    uint32_t arr_ticks = (duration_ms * 10) - 1;
    __HAL_TIM_SET_AUTORELOAD(h->duration_timer, arr_ticks); //control how long each long will last using the duration timer.

    __HAL_TIM_SET_AUTORELOAD(h->pwm_timer, note_table[note].period);
    __HAL_TIM_SET_COMPARE(h->pwm_timer, h->channel, note_table[note].pulse);

    HAL_TIM_PWM_Start(h->pwm_timer, h->channel);
    HAL_TIM_Base_Start_IT(h->duration_timer);
}
void Buzzer_Stop(Buzzer_Handle *h)
{
    HAL_TIM_PWM_Stop(h->pwm_timer, h->channel);
    HAL_TIM_Base_Stop_IT(h->duration_timer);
}

void Buzzer_DurationElapsed(Buzzer_Handle *h)
{
    HAL_TIM_PWM_Stop(h->pwm_timer, h->channel);
}



typedef struct {
    Note note;
    uint32_t duration_ms;
} MelodyNote;

static const MelodyNote little_yonatan[] = {
    {NOTE_G1, 200}, {NOTE_E1, 200}, {NOTE_E1, 400},
    {NOTE_F1, 200}, {NOTE_D1, 200}, {NOTE_D1, 400},
    {NOTE_C1, 200}, {NOTE_D1, 200}, {NOTE_E1, 200}, {NOTE_F1, 200},
    {NOTE_G1, 200}, {NOTE_G1, 200}, {NOTE_G1, 400},
    {NOTE_G1, 200}, {NOTE_E1, 200}, {NOTE_E1, 400},
    {NOTE_F1, 200}, {NOTE_D1, 200}, {NOTE_D1, 400},
    {NOTE_C1, 200}, {NOTE_E1, 200}, {NOTE_G1, 200}, {NOTE_G1, 200}, {NOTE_C1, 800},

    {NOTE_D1, 200}, {NOTE_D1, 200}, {NOTE_D1, 200}, {NOTE_D1, 200},
    {NOTE_D1, 200}, {NOTE_E1, 200}, {NOTE_F1, 400},
    {NOTE_E1, 200}, {NOTE_E1, 200}, {NOTE_E1, 200}, {NOTE_E1, 200},
    {NOTE_E1, 200}, {NOTE_F1, 200}, {NOTE_G1, 400},
    {NOTE_E1, 200}, {NOTE_E1, 200}, {NOTE_E1, 400},
    {NOTE_F1, 200}, {NOTE_D1, 200}, {NOTE_D1, 400},
    {NOTE_C1, 200}, {NOTE_E1, 200}, {NOTE_G1, 200}, {NOTE_G1, 200}, {NOTE_C1, 800},
};



void Buzzer_PlayLittleYonatan(Buzzer_Handle *h)
{
    for (uint32_t i = 0; i < LITTLE_YONATAN_LENGTH; i++)
    {
        Buzzer_PlayNote(h, little_yonatan[i].note, little_yonatan[i].duration_ms);
        HAL_Delay(little_yonatan[i].duration_ms + 100);
    }
}





static const MelodyNote wellerman[] = {
    {NOTE_A1,300},{NOTE_D1,300},{NOTE_D1,300},{NOTE_D1,300},
    {NOTE_D1,600},{NOTE_F1,300},{NOTE_A1,300},
    {NOTE_A1,300},{NOTE_A1,300},{NOTE_G1,300},{NOTE_F1,300},
    {NOTE_G1,300},{NOTE_A1,600},

    {NOTE_A1,300},{NOTE_D1,300},{NOTE_D1,300},{NOTE_D1,300},
    {NOTE_D1,600},{NOTE_F1,300},{NOTE_A1,300},
    {NOTE_A1,300},{NOTE_G1,300},{NOTE_F1,300},{NOTE_E1,300},
    {NOTE_D1,600},

    {NOTE_A1,600},{NOTE_A1,300},{NOTE_F1,300},
    {NOTE_G1,600},{NOTE_A1,300},{NOTE_A1,300},
    {NOTE_G1,300},{NOTE_F1,300},{NOTE_G1,300},{NOTE_A1,300},
    {NOTE_A1,600},

    {NOTE_A1,300},{NOTE_D1,300},{NOTE_D1,300},{NOTE_D1,300},
    {NOTE_D1,600},{NOTE_F1,300},{NOTE_A1,300},
    {NOTE_A1,300},{NOTE_G1,300},{NOTE_F1,300},{NOTE_E1,300},
    {NOTE_D1,600},
};


#define WELLERMAN_LENGTH (sizeof(wellerman) / sizeof(wellerman[0]))


void Buzzer_PlayWellerman(Buzzer_Handle *h)
{
    for (uint32_t i = 0; i < WELLERMAN_LENGTH; i++)
    {
        Buzzer_PlayNote(h, wellerman[i].note, wellerman[i].duration_ms);
        HAL_Delay(wellerman[i].duration_ms);
    }
}





//Implementation of UART interrupt

//uint8_t rx_byte;
//
//HAL_UART_Receive_IT(&huart2, &rx_byte, 1);   // start listening for 1 byte at a time
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart->Instance == USART2)
//    {
//        switch (rx_byte)
//        {
//            case '1': Buzzer_PlayNote(buzzer, NOTE_C1, 500); break;
//            case '2': Buzzer_PlayNote(buzzer, NOTE_D1, 500); break;
//            case '3': Buzzer_PlayNote(buzzer, NOTE_E1, 500); break;
//            case '4': Buzzer_PlayNote(buzzer, NOTE_F1, 500); break;
//            case '5': Buzzer_PlayNote(buzzer, NOTE_G1, 500); break;
//            case '6': Buzzer_PlayNote(buzzer, NOTE_A1, 500); break;
//            case '7': Buzzer_PlayNote(buzzer, NOTE_B1, 500); break;
//            case '8': Buzzer_PlayNote(buzzer, NOTE_C2, 500); break;
//            case '9': Buzzer_PlayNote(buzzer, NOTE_D2, 500); break;
//            default: break;
//        }
//
//        HAL_UART_Receive_IT(&huart2, &rx_byte, 1);  // re-arm for the NEXT byte
//    }
//}

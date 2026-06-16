#include "basics.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define GPIOA_MODER  (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOB_MODER  (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_IDR    (*(volatile uint32_t *)(GPIOB_BASE + 0x10))
#define GPIOA_BSRR   (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

#define BUT1_PIN     (1U << 3) //but1 == pin3
#define RED_LED_PIN  (1U << 6) //pin 6



void turn_red_light_with_button_reg(void) {

	 GPIOA_MODER &= ~(0x3U << 12);   // clear PA6 mode bits
	 GPIOA_MODER |=  (0x1U << 12);   // set PA6 as output

	 GPIOB_MODER &= ~(0x3U << 6);    // clear PB3 mode bits → input

	 if (!(GPIOB_IDR & BUT1_PIN))         // button pressed (active low)
		 GPIOA_BSRR = RED_LED_PIN;        // LED ON
	 else
		 GPIOA_BSRR = (RED_LED_PIN << 16); // LED OFF


}


void turn_red_light_with_button(void) {

    static uint8_t lastState = 1;  // 1 = not pressed (active low)
    uint8_t currentState = HAL_GPIO_ReadPin(but1_GPIO_Port, but1_Pin);

    if (currentState == GPIO_PIN_RESET && lastState == GPIO_PIN_SET)
    {
        // falling edge — button just pressed
        printf("but1 been pressed\r\n");
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
    }
    else if (currentState == GPIO_PIN_SET && lastState == GPIO_PIN_RESET)
    {
        // rising edge — button just released
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
    }

    lastState = currentState;  // save for next iteration
}

//turning the red light by clicking event interrupt
//turn_red_light_with_button callback
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    if (GPIO_Pin == but1_Pin)
//    {
//        if (HAL_GPIO_ReadPin(but1_GPIO_Port, but1_Pin) == GPIO_PIN_RESET)
//        {
//            // falling edge — button pressed
//            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
//            printf("but1 pressed\r\n");
//        }
//        else
//        {
//            // rising edge — button released
//            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
//            printf("but1 released\r\n");
//        }
//    }
//}



//******************************************************************************






extern UART_HandleTypeDef huart2; //ALREADY IN MAIN C LINE 45

// ─────────────────────────────────────────
// UART INIT
// ─────────────────────────────────────────
//void InitUart2(void) {
//    huart2.Instance        = USART2;
//    huart2.Init.BaudRate   = 115200;
//    huart2.Init.WordLength = UART_WORDLENGTH_8B;
//    huart2.Init.StopBits   = UART_STOPBITS_1;
//    huart2.Init.Parity     = UART_PARITY_NONE;
//    huart2.Init.Mode       = UART_MODE_TX_RX;
//    huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
//    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//    HAL_UART_Init(&huart2);
//}

// ─────────────────────────────────────────
// PRINTF REDIRECT TO UART
// ─────────────────────────────────────────
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int _write(int file, char *ptr, int len)
{
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}

void basic_uart_messege_without_printf(){
	char msg[] = "got your message";
	HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg)-1, HAL_MAX_DELAY);
}


void basic_uart_receive_message(void)
{
    uint8_t byte = 0;
    printf("waiting for message...\r\n");
    HAL_UART_Receive(&huart2, &byte, 1, HAL_MAX_DELAY);
    printf("received: %c\r\n", byte);  // %c for single character
}


void basic_uart_turn_lights_by_message_from_console(void)
{
    uint8_t buf[16];
    uint8_t byte = 0;
    uint8_t idx = 0;

    memset(buf, 0, sizeof(buf));

    printf("waiting for message...\r\n");

    while (idx < sizeof(buf) - 1)
    {
        HAL_UART_Receive(&huart2, &byte, 1, HAL_MAX_DELAY);

        if (byte == '\0' || byte == '\n' || byte == '\r')
            break;

        buf[idx++] = byte;
    }
    buf[idx] = '\0';

    printf("received: %s\r\n", buf);

    if (strcmp((char *)buf, "red on") == 0)
    {
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
        printf("red LED ON\r\n");
    }
    else if (strcmp((char *)buf, "red off") == 0)
    {
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
        printf("red LED OFF\r\n");
    }
    else if (strcmp((char *)buf, "blue on") == 0)
    {
        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);
        printf("blue LED ON\r\n");
    }
    else if (strcmp((char *)buf, "blue off") == 0)
    {
        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
        printf("blue LED OFF\r\n");
    }
    else
    {
        printf("unknown command: %s\r\n", buf);
    }
}


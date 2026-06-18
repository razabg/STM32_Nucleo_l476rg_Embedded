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

/* * FUNCTION: __io_putchar
 * PURPOSE: Low-level UART character transmission.
 * DETAILS: Sends a single byte over UART2 using HAL polling.
 * This is the fundamental link needed to connect printf() to the serial console.
 */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/* * FUNCTION: _write
 * PURPOSE: Standard C library override for stream output.
 * DETAILS: Automatically loops through a string buffer and passes it to __io_putchar.
 * This maps all standard printf() statements directly to the UART hardware.
 */
int _write(int file, char *ptr, int len)
{
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}

/* * FUNCTION: turn_red_light_with_button_reg
 * PURPOSE: Direct register-level GPIO control (No HAL).
 * DETAILS: Configures PA6 as output and PB3 as input via MODER registers.
 * Polls the IDR register to read the button (active low) and uses
 * the BSRR register to atomically turn the Red LED on or off.
 */
//void turn_red_light_with_button_reg(void) {
//
//	 GPIOA_MODER &= ~(0x3U << 12);   // clear PA6 mode bits
//	 GPIOA_MODER |=  (0x1U << 12);   // set PA6 as output
//
//	 GPIOB_MODER &= ~(0x3U << 6);    // clear PB3 mode bits → input
//
//	 if (!(GPIOB_IDR & BUT1_PIN))         // button pressed (active low)
//		 GPIOA_BSRR = RED_LED_PIN;        // LED ON
//	 else
//		 GPIOA_BSRR = (RED_LED_PIN << 16); // LED OFF
//}
//
///* * FUNCTION: turn_red_light_with_button
// * PURPOSE: Polled button edge-detection using HAL functions.
// * DETAILS: Regularly checks the button state. Compares current vs. last state
// * to detect a 'Falling Edge' (press event -> LED ON + print)
// * or 'Rising Edge' (release event -> LED OFF).
// */
//void turn_red_light_with_button(void) {
//
//    static uint8_t lastState = 1;  // 1 = not pressed (active low)
//    uint8_t currentState = HAL_GPIO_ReadPin(but1_GPIO_Port, but1_Pin);
//
//    if (currentState == GPIO_PIN_RESET && lastState == GPIO_PIN_SET)
//    {
//        // falling edge — button just pressed
//        printf("but1 been pressed\r\n");
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
//    }
//    else if (currentState == GPIO_PIN_SET && lastState == GPIO_PIN_RESET)
//    {
//        // rising edge — button just released
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
//    }
//
//    lastState = currentState;  // save for next iteration
//}
//
//
///* * FUNCTION: HAL_GPIO_EXTI_Callback
// * PURPOSE: Hardware-triggered Interrupt Service Routine (ISR) for GPIO state changes.
// * DETAILS:
// * 1. This function is automatically invoked by the NVIC (Nested Vectored Interrupt Controller)
// * whenever an configured External Interrupt (EXTI) line detects a voltage edge change.
// * 2. Checks `GPIO_Pin` to ensure the interrupt originated specifically from the button's hardware pin.
// * 3. Immediately reads the live pin voltage to determine if it was a Falling Edge (press)
// * or Rising Edge (release) event.
// * 4. Toggles the hardware LED state asynchronously without needing a main 'while(1)' loop block.
// * * NOTE: Avoid calling slow operations like `printf()` inside production ISR callbacks, as they block
// * the CPU from handling other time-critical system interrupts!
// */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    // Verify which pin caused the interrupt (since multiple EXTI pins share callbacks)
//    if (GPIO_Pin == but1_Pin)
//    {
//        // Read live pin state to distinguish between press and release
//        if (HAL_GPIO_ReadPin(but1_GPIO_Port, but1_Pin) == GPIO_PIN_RESET)
//        {
//            // Falling Edge event detected (Voltage fell from 3.3V to 0V / Pressed)
//            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
//            printf("but1 pressed\r\n");
//        }
//        else
//        {
//            // Rising Edge event detected (Voltage rose from 0V back to 3.3V / Released)
//            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
//            printf("but1 released\r\n");
//        }
//    }
//}
//
//
//
//
///* * FUNCTION: basic_uart_messege_without_printf
// * PURPOSE: Raw string transmission via UART.
// * DETAILS: Bypasses printf entirely, transmitting a hardcoded string
// * directly into the hardware buffer using a blocking HAL function.
// */
//void basic_uart_messege_without_printf(){
//	char msg[] = "got your message";
//	HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg)-1, HAL_MAX_DELAY);
//}
//
///* * FUNCTION: basic_uart_receive_message
// * PURPOSE: Blocking single-byte UART reception.
// * DETAILS: Halts execution (polls) until exactly 1 byte/character is received
// * over UART2, then echoes it back to the console using printf.
// */
//void basic_uart_receive_message(void)
//{
//    uint8_t byte = 0;
//    printf("waiting for message...\r\n");
//    HAL_UART_Receive(&huart2, &byte, 1, HAL_MAX_DELAY);
//    printf("received: %c\r\n", byte);  // %c for single character
//}
//
//
//
//
//
///* * FUNCTION: basic_uart_turn_lights_by_message_from_console
// * PURPOSE: Basic CLI (Command Line Interface) over UART to control LEDs.
// * DETAILS:
// * 1. Dynamically builds a string buffer by receiving characters one-by-one
// * in a blocking loop until a termination character ('\0', '\n', or '\r') is hit.
// * 2. Null-terminates the collected string buffer to prevent overflow or garbage reading.
// * 3. Uses standard `strcmp` to match the received text against hardcoded string commands.
// * 4. Toggles corresponding GPIO pins (RED_LED or BLUE_LED) using HAL functions based on the match.
// */
//void basic_uart_turn_lights_by_message_from_console(void)
//{
//    uint8_t buf[16];
//    uint8_t byte = 0;
//    uint8_t idx = 0;
//
//    memset(buf, 0, sizeof(buf));
//
//    printf("waiting for message...\r\n");
//
//    while (idx < sizeof(buf) - 1)
//    {
//        HAL_UART_Receive(&huart2, &byte, 1, HAL_MAX_DELAY);
//
//        if (byte == '\0' || byte == '\n' || byte == '\r')
//            break;
//
//        buf[idx++] = byte;
//    }
//    buf[idx] = '\0';
//
//    printf("received: %s\r\n", buf);
//
//    if (strcmp((char *)buf, "red on") == 0)
//    {
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
//        printf("red LED ON\r\n");
//    }
//    else if (strcmp((char *)buf, "red off") == 0)
//    {
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
//        printf("red LED OFF\r\n");
//    }
//    else if (strcmp((char *)buf, "blue on") == 0)
//    {
//        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);
//        printf("blue LED ON\r\n");
//    }
//    else if (strcmp((char *)buf, "blue off") == 0)
//    {
//        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
//        printf("blue LED OFF\r\n");
//    }
//    else
//    {
//        printf("unknown command: %s\r\n", buf);
//    }
//}
//
//
//
//
//
//
//
//
//#define BUFFER_SIZE 8000
//static uint32_t src[BUFFER_SIZE];
//static uint32_t dst[BUFFER_SIZE];
//
//extern DMA_HandleTypeDef hdma_memtomem_dma1_channel1;
//extern TIM_HandleTypeDef htim2;
//
///* * FUNCTION: fill_buffer
// * PURPOSE: Helper to populate an array with a specific test pattern.
// */
//static void fill_buffer(uint32_t *buf, uint32_t size, uint32_t value)
//{
//    for (uint32_t i = 0; i < size; i++)
//        buf[i] = value;
//}
//
///* * FUNCTION: compare_buffers
// * PURPOSE: Helper to verify data integrity after a memory copy operation.
// * RETURNS: 1 if identical, 0 if data corruption occurred.
// */
//static int compare_buffers(uint32_t *a, uint32_t *b, uint32_t size)
//{
//    for (uint32_t i = 0; i < size; i++)
//    {
//        if (a[i] != b[i])
//            return 0;
//    }
//    return 1;
//}
//
///* * FUNCTION: dma_task
// * PURPOSE: Benchmark DMA Memory-to-Memory hardware transfers vs. CPU software transfers (memcpy).
// * DETAILS: Uses TIM2 as a microsecond timer. It fills, transfers, and verifies a 32 KB block
// * twice using hardware DMA, then repeats using standard CPU `memcpy()`.
// * Finally, it prints the microsecond speeds to evaluate hardware offloading performance.
// */
//void dma_task(void)
//{
//    uint32_t elapsed_dma, elapsed_memcpy;
//
//    // ── Transfer 1 ───────────────────────────────────────
//    printf("=== Transfer 1 ===\r\n");
//    fill_buffer(src, BUFFER_SIZE, 0xAAAAAAAA);
//    fill_buffer(dst, BUFFER_SIZE, 0x00000000);
//
//    __HAL_TIM_SET_COUNTER(&htim2, 0);
//    HAL_TIM_Base_Start(&htim2);
//
//    HAL_DMA_Start(&hdma_memtomem_dma1_channel1,
//                  (uint32_t)src,
//                  (uint32_t)dst,
//                  BUFFER_SIZE);
//
//    HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,
//                             HAL_DMA_FULL_TRANSFER,
//                             HAL_MAX_DELAY);
//
//    elapsed_dma = __HAL_TIM_GET_COUNTER(&htim2);
//    HAL_TIM_Base_Stop(&htim2);
//
//    if (compare_buffers(src, dst, BUFFER_SIZE))
//        printf("Transfer 1: OK - buffers match\r\n");
//    else
//        printf("Transfer 1: FAIL - buffers differ\r\n");
//
//    printf("Transfer 1 DMA time: %lu us\r\n", elapsed_dma);
//
//
//    // ── Transfer 2 ───────────────────────────────────────
//    printf("=== Transfer 2 ===\r\n");
//    fill_buffer(src, BUFFER_SIZE, 0xBBBBBBBB);
//    fill_buffer(dst, BUFFER_SIZE, 0x00000000);
//
//    __HAL_TIM_SET_COUNTER(&htim2, 0);
//    HAL_TIM_Base_Start(&htim2);
//
//    HAL_DMA_Start(&hdma_memtomem_dma1_channel1,
//                  (uint32_t)src,
//                  (uint32_t)dst,
//                  BUFFER_SIZE);
//
//    HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,
//                             HAL_DMA_FULL_TRANSFER,
//                             HAL_MAX_DELAY);
//
//    elapsed_dma = __HAL_TIM_GET_COUNTER(&htim2);
//    HAL_TIM_Base_Stop(&htim2);
//
//    if (compare_buffers(src, dst, BUFFER_SIZE))
//        printf("Transfer 2: OK - buffers match\r\n");
//    else
//        printf("Transfer 2: FAIL - buffers differ\r\n");
//
//    printf("Transfer 2 DMA time: %lu us\r\n", elapsed_dma);
//
//
//    // ── memcpy for comparison ─────────────────────────────
//    printf("=== memcpy ===\r\n");
//    fill_buffer(src, BUFFER_SIZE, 0xCCCCCCCC);
//    fill_buffer(dst, BUFFER_SIZE, 0x00000000);
//
//    __HAL_TIM_SET_COUNTER(&htim2, 0);
//    HAL_TIM_Base_Start(&htim2);
//
//    memcpy(dst, src, BUFFER_SIZE * sizeof(uint32_t));
//
//    elapsed_memcpy = __HAL_TIM_GET_COUNTER(&htim2);
//    HAL_TIM_Base_Stop(&htim2);
//
//    printf("memcpy time: %lu us\r\n", elapsed_memcpy);
//
//    // ── Final comparison ─────────────────────────────────
//    printf("-----------------------------------\r\n");
//    printf("Last DMA transfer: %lu us\r\n", elapsed_dma);
//    printf("memcpy:            %lu us\r\n", elapsed_memcpy);
//    if (elapsed_dma < elapsed_memcpy)
//        printf("DMA was faster by %lu us\r\n", elapsed_memcpy - elapsed_dma);
//    else
//        printf("memcpy was faster by %lu us\r\n", elapsed_dma - elapsed_memcpy);
//}

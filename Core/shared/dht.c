#include "dht.h"
#include <stdio.h>
extern TIM_HandleTypeDef htim2;



// ── Private helpers (not visible outside this file) ──

//function that count the elapsed time by us (microseconds)
static void delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

//GPIO_InitStruct.Pin = DHT_Pin;
// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
// GPIO_InitStruct.Pull = GPIO_PULLUP;
// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
// HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);


static void set_pin_output(void)
{
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin   = DHT_Pin;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT_GPIO_Port, &gpio);
}

static void set_pin_input(void)
{
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin  = DHT_Pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT_GPIO_Port, &gpio);
}


static DHT_Result send_start_signal(void)
{
    // 1. make sure pin is output
    set_pin_output();

    // 2. pull LOW for 18ms
    HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET);
    delay_us(18000);                    // 18ms = 18000μs

    // 3. pull HIGH and wait 40μs
    HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_SET);
    delay_us(40);

    // 4. switch to input — DHT takes over the line
    set_pin_input();

    return DHT_OK;
}

//Loop 1 — waits until DHT actually pulls the line LOW
//Loop 2 — waits for that 80μs LOW to finish
//Loop 3 — waits for that 80μs HIGH to finish
//After loop 3 — first data bit is starting
static DHT_Result wait_for_response(void)
{
    // wait for DHT to pull LOW (with timeout)
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_SET)//still high
    {
        if (__HAL_TIM_GET_COUNTER(&htim2) > 100)
            return DHT_NO_RESPONSE;   // DHT never pulled low
    }

    // wait for DHT's 80μs LOW to finish
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_RESET)
    {
        if (__HAL_TIM_GET_COUNTER(&htim2) > 100)
            return DHT_NO_RESPONSE;
    }

    // wait for DHT's 80μs HIGH to finish
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_SET)
    {
        if (__HAL_TIM_GET_COUNTER(&htim2) > 100)
            return DHT_NO_RESPONSE;
    }

    return DHT_OK;
}


static uint8_t read_bit(void)
{
    // wait for the 50μs LOW to finish
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_RESET);

    // measure how long HIGH lasts
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_SET);

    // read how many μs passed
    uint32_t duration = __HAL_TIM_GET_COUNTER(&htim2);//we measured the vcc to know if its 1 or 0;

    return (duration > 50) ? 1 : 0;
}


static void read_40_bits(uint8_t *bytes)
{
    // clear all 5 bytes first
    for (int i = 0; i < 5; i++)
        bytes[i] = 0;

    // read 40 bits, MSB first
    for (int i = 0; i < 40; i++)
    {
        uint8_t bit = read_bit();

        // which byte does this bit belong to?
        int byte_index = i / 8;

        // shift existing bits left, add new bit on the right
        bytes[byte_index] = (bytes[byte_index] << 1) | bit;
    }
}






// ── Public function ───────────────────────────────────

DHT_Result DHT_Read(DHT_Data *out)
{
	uint8_t bytes[5];// the buffer to read the dht data
    // Step 1: send start signal
	send_start_signal();
    // Step 2: wait for DHT response
	    if (wait_for_response() != DHT_OK) { return DHT_NO_RESPONSE; }

    // Step 3: read 40 bits
	    read_40_bits(bytes);

	    // inside DHT11_Read() after read_40_bits():

	    uint8_t sum = bytes[0] + bytes[1] + bytes[2] + bytes[3];

	    if (sum != bytes[4])
	        return DHT_CHECKSUM_ERROR;   // data corrupted

	    // data is good — fill the result struct
	    out->humidity_int    = bytes[0];
	    out->humidity_dec    = bytes[1];
	    out->temperature_int = bytes[2];
	    out->temperature_dec = bytes[3];

	    return DHT_OK;
}


//important ! there is anther button interrupt callback in basics.c make sure its under comment to use this callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == but1_Pin)
    {
        if (HAL_GPIO_ReadPin(but1_GPIO_Port, but1_Pin) == GPIO_PIN_RESET)
        {
            // Falling Edge — button pressed → read DHT
            DHT_Data data;
            DHT_Result result = DHT_Read(&data);

            if (result == DHT_OK)
            {
                printf("Temperature: %d.%d C\r\n",
                       data.temperature_int,
                       data.temperature_dec);
                printf("Humidity:    %d.%d%%\r\n",
                       data.humidity_int,
                       data.humidity_dec);
            }
            else if (result == DHT_NO_RESPONSE)
            {
                printf("DHT11 not responding\r\n");
            }
            else
            {
                printf("Checksum error\r\n");
            }
        }
        // no need to handle release for this task
    }
}

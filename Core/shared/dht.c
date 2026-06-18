#include "dht.h"
extern TIM_HandleTypeDef htim2;



// ── Private helpers (not visible outside this file) ──
static DHT_Result send_start_signal(void)
{
    // 1. make sure pin is output
    set_pin_output();

    // 2. pull LOW for 18ms
    HAL_GPIO_WritePin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin, GPIO_PIN_RESET);
    delay_us(18000);                    // 18ms = 18000μs

    // 3. pull HIGH and wait 40μs
    HAL_GPIO_WritePin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin, GPIO_PIN_SET);
    delay_us(40);

    // 4. switch to input — DHT takes over the line
    set_pin_input();

    return DHT_OK;
}



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

// ── Public function ───────────────────────────────────

DHT11_Result DHT_Read(DHT_Data *out)
{
    // Step 1: send start signal
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState)

    // Step 2: wait for DHT response
    // Step 3: read 40 bits
    // Step 4: assemble 5 bytes
    // Step 5: verify checksum
    // Step 6: fill out struct and return
}

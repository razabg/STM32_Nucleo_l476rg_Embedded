/*
 * ============================================================================
 * PWM REFERENCE — Two Implementation Methods
 * STM32L476 Nucleo @ 80MHz
 * ============================================================================
 *
 * This file documents two different ways to generate a PWM signal on an
 * STM32 timer, with full explanations of WHY each works and WHEN to use it.
 *
 * Both solutions assume:
 *   - Target frequency: 10kHz (period = 100µs)
 *   - LED on PA6 (mapped to TIM3 Channel 1)
 *   - A second timer (TIM6) used to slowly vary the duty cycle every 100ms,
 *     creating a "breathing" LED effect
 *
 * ============================================================================
 * THE MATH — How PSC and ARR Determine Frequency/Period
 * ============================================================================
 *
 * Formula:
 *     Frequency = Clock / (PSC+1) / (ARR+1)
 *     Period    = 1 / Frequency
 *
 * Two-stage way to think about it:
 *     Stage 1 (PSC) — slows the 80MHz clock down into "ticks"
 *         tick_frequency = Clock / (PSC+1)
 *         tick_duration  = 1 / tick_frequency
 *
 *     Stage 2 (ARR) — sets how many ticks make one full period
 *         period = (ARR+1) * tick_duration
 *
 * Example used throughout this file — 10kHz / 100µs period:
 *     PSC = 7    -> tick_frequency = 80,000,000 / 8   = 10,000,000 Hz (10MHz)
 *                   tick_duration  = 0.1µs
 *     ARR = 999  -> period = 1000 ticks * 0.1µs        = 100µs
 *                   frequency = 1 / 100µs               = 10kHz  ✔
 *
 * Duty cycle (CCR) is just a fraction of ARR:
 *     CCR = (duty_percent / 100) * ARR
 *     1%  -> CCR = 9
 *     50% -> CCR = 499
 *     99% -> CCR = 989
 *
 * Larger ARR = finer duty cycle resolution (more possible CCR steps).
 * ARR=999 gives 1000 steps (0.1% resolution) -- ideal for smooth 1%-99% sweeps.
 *
 *
 * ============================================================================
 * TIM6 — Shared by Both Solutions (the "breathing" driver)
 * ============================================================================
 *
 * TIM6 fires an interrupt every 100ms. Each time it fires, it updates the
 * duty cycle value used by TIM3, stepping it up or down between 1% and 99%.
 *
 * CubeMX settings:
 *     Prescaler (PSC) = 7999   -> tick_frequency = 80MHz / 8000 = 10kHz
 *     Period    (ARR) = 999    -> period = 1000 ticks * 0.1ms   = 100ms
 *     NVIC: enable TIM6 global interrupt
 *
 * Verify:
 *     80,000,000 / (7999+1) / (999+1) = 10,000,000 / 1000 = 10Hz = 100ms ✔
 *
 *
 * ============================================================================
 * SOLUTION 1 — HARDWARE PWM (no interrupts, "direct output")
 * ============================================================================
 *
 * HOW IT WORKS:
 *   The timer's compare hardware is physically wired to the GPIO pin.
 *   The silicon automatically drives the pin HIGH when counter < CCR, and
 *   LOW when counter >= CCR, every single cycle, with ZERO CPU involvement.
 *   Your code only needs to write a new CCR value occasionally to change
 *   the duty cycle -- the waveform generation itself runs entirely in
 *   hardware, forever, in the background.
 *
 * WHEN TO USE:
 *   - Best choice for almost all real PWM use cases (LED dimming, motor
 *     control, etc.)
 *   - Supports very high frequencies (up to ~10MHz) because there's no
 *     interrupt overhead
 *   - Zero CPU cost while running
 *
 * CubeMX SETTINGS:
 *   TIM3 -> Channel1 -> "PWM Generation CH1"
 *   PSC = 7, ARR = 999  (10kHz, see math above)
 *   Pulse (initial CCR) = 9  (start at 1%)
 *   PA6 is automatically claimed as TIM3_CH1 alternate function -- the pin
 *   is now permanently hardware-controlled, you must NOT call
 *   HAL_GPIO_WritePin on it yourself anymore.
 *
 * STARTUP CODE:
 *   HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);   // note: NOT _IT
 *   HAL_TIM_Base_Start_IT(&htim6);              // still need TIM6 interrupt
 *                                                // for the breathing logic
 *
 * CALLBACK (only TIM6 needed -- TIM3 needs no callback at all):
 */

/*
int duty_hw = 9;
int direction_hw = 1;
int step_hw = 50;

void HAL_TIM_PeriodElapsedCallback_SOLUTION1(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        // simply hand the new duty value to the hardware -- it takes care
        // of the rest automatically, no manual pin toggling needed
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty_hw);

        duty_hw += direction_hw * step_hw;
        if (duty_hw >= 989) { duty_hw = 989; direction_hw = -1; }
        if (duty_hw <= 9)   { duty_hw = 9;   direction_hw =  1; }
    }
}
*/

/*
 * ============================================================================
 * SOLUTION 2 — PWM WITH INTERRUPTS (manual GPIO control)
 * ============================================================================
 *
 * HOW IT WORKS:
 *   The timer still runs the exact same internal counter-vs-CCR comparison
 *   logic as Solution 1, BUT it is configured to NOT physically drive any
 *   pin. Instead, it raises two separate interrupts per cycle, and YOUR
 *   CODE is responsible for manually setting the pin HIGH/LOW at the right
 *   moments using HAL_GPIO_WritePin.
 *
 *   Interrupt A -- "Period Elapsed" (counter wraps 999 -> 0, new cycle
 *   begins): this is your cue to turn the pin ON.
 *
 *   Interrupt B -- "PWM Pulse Finished" (counter reaches CCR, the compare
 *   match): this is your cue to turn the pin OFF.
 *
 *   Together, these two interrupts firing 10,000 times/sec each (20,000
 *   interrupts/sec total at 10kHz) recreate the exact same waveform that
 *   Solution 1's hardware produced automatically -- just with the CPU
 *   doing the work instead of dedicated silicon.
 *
 * WHEN TO USE:
 *   - Only when you specifically need software/CPU involvement on every
 *     PWM edge (e.g. an exercise requirement, or some custom logic that
 *     must run exactly at each edge)
 *   - Only practical at LOW frequencies (up to ~10kHz) -- beyond that the
 *     interrupt overhead overwhelms the CPU and starves other tasks
 *   - Higher flexibility but real CPU cost
 *
 * CubeMX SETTINGS:
 *   TIM3 -> Channel1 -> "PWM Generation No Output"   <-- KEY DIFFERENCE
 *       (NOT "PWM Generation CH1", NOT "Output Compare No Output")
 *       This keeps real PWM mode (PWM mode 1 or 2) and its associated
 *       interrupts/callbacks, but releases the pin -- PA6 is NOT claimed
 *       as alternate function, so it stays free for you to configure as
 *       plain GPIO_MODE_OUTPUT_PP and drive manually.
 *   PSC = 7, ARR = 999  (10kHz, identical math to Solution 1)
 *   Pulse (initial CCR) = 9
 *   NVIC: enable TIM3 global interrupt
 *   GPIO: configure PA6 / RED_LED_Pin as plain GPIO_MODE_OUTPUT_PP
 *
 * STARTUP CODE:
 *   HAL_TIM_Base_Start_IT(&htim3);              // for PeriodElapsed
 *   HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1); // for PulseFinished
 *   HAL_TIM_Base_Start_IT(&htim6);               // breathing logic, same
 *                                                 // as Solution 1
 *
 * CALLBACKS (both TIM3 events + TIM6, all needed):
 */
//
//int duty = 9;
//int direction = 1;
//int step = 50;
//
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3)
//    {
//        // new 100µs cycle is starting right now -- turn the LED on
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
//    }
//    else if (htim->Instance == TIM6)
//    {
//        // every 100ms: push current duty into TIM3's CCR register,
//        // then calculate what the NEXT duty value should be
//        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
//
//        duty += direction * step;
//        if (duty >= 989) { duty = 989; direction = -1; }
//        if (duty <= 9)   { duty = 9;   direction =  1; }
//    }
//}
//
//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3)
//    {
//        // counter has just reached CCR -- the "on" portion of this cycle
//        // is over, turn the LED off for the remainder of the period
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
//    }
//}

/*
 * ============================================================================
 * WHY THE LED LOOKS LIKE IT'S DIMMING (NOT JUST BLINKING)
 * ============================================================================
 *
 * The pin is ALWAYS either fully 3.3V or fully 0V -- there is no real
 * "in-between" voltage ever produced. The dimming effect is purely a trick
 * of human perception:
 *
 *   - The human eye cannot distinguish individual flickers faster than
 *     roughly 50-60 times per second; faster changes blur into an average.
 *   - At 10kHz, the LED is switching on/off 10,000 times per second --
 *     about 170x faster than the eye can resolve.
 *   - The eye perceives the AVERAGE proportion of time the LED spends ON
 *     vs OFF within each blurred-together window.
 *
 *   1%  duty  -> ON for 1µs,  OFF for 99µs  per 100µs cycle -> looks very dim
 *   50% duty  -> ON for 50µs, OFF for 50µs  per 100µs cycle -> looks medium
 *   99% duty  -> ON for 99µs, OFF for 1µs   per 100µs cycle -> looks very bright
 *
 * As TIM6 slowly changes `duty` every 100ms, the ON/OFF ratio within each
 * fast 100µs cycle shifts smoothly, which the eye perceives as a smooth
 * brightness fade -- the "breathing" effect -- even though every single
 * voltage transition is still a hard, instant 1-or-0 switch.
 *
 *
 * ============================================================================
 * SIDE-BY-SIDE SUMMARY
 * ============================================================================
 *
 *                          Solution 1 (HW)      Solution 2 (Interrupts)
 *   CubeMX Channel mode    PWM Generation CH1   PWM Generation No Output
 *   Start function          HAL_TIM_PWM_Start     HAL_TIM_PWM_Start_IT
 *   Base start needed?      No (not _IT)          Yes (HAL_TIM_Base_Start_IT)
 *   Pin driven by           Hardware (automatic)  Your code (HAL_GPIO_WritePin)
 *   Callbacks needed        None for TIM3         PeriodElapsed +
 *                                                  PWM_PulseFinished
 *   CPU cost while running  ~0                    20,000 interrupts/sec @10kHz
 *   Max practical frequency ~10MHz                ~10kHz
 *   Best for                Production use        Learning / special timing
 *                                                  requirements
 *
 * ============================================================================
 */

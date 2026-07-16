/*
 * TOPIC: DMA Memory-to-Memory Transfer vs. CPU memcpy — Benchmark
 *
 * DMA (Direct Memory Access) lets the hardware copy blocks of memory
 * independently, freeing the CPU to do other work (or sleep) during the
 * transfer. This example benchmarks two full DMA transfers against a standard
 * CPU memcpy() on an 8000-element uint32_t array (32 KB).
 *
 * TIM2 is used as a free-running microsecond counter for timing.
 * The result is printed: "DMA was faster by X us" (or vice versa).
 *
 * KEY APIS:
 *   HAL_DMA_Start(handle, src, dst, length)           — start transfer (blocking poll style)
 *   HAL_DMA_PollForTransfer(handle, type, timeout)    — wait for completion
 *   __HAL_TIM_SET_COUNTER / __HAL_TIM_GET_COUNTER     — reset / read TIM2 tick count
 *
 * In main(), before the while(1):
 *   HAL_TIM_Base_Start(&htim2);   — start microsecond counter
 *   dma_task();                   — run the benchmark
 */

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

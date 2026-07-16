//
//
//
//#define FLASH_USER_ADDR   0x080FF800U   // last 2KB page of 1MB flash on STM32L476RG
//
//char *str = "hello from flash";
//      Flash_WriteString(FLASH_USER_ADDR, str);
//
//      // Flash is memory-mapped — you can read it back just by
//      // treating the address as a pointer, no special read function needed.
//      char *readback = (char *)FLASH_USER_ADDR;
//      printf("Read from flash: %s\r\n", readback);
//
//
//void Flash_WriteString(uint32_t address, char *str)
//{
//    // Step 1: Unlock flash — it's write-protected by default.
//    // HAL_FLASH_Unlock() handles the internal key-sequence for you.
//    HAL_FLASH_Unlock();
//
//    // Step 2: Erase the page first.
//    // Flash can only flip bits 1->0 when programming. To get back to a
//    // clean slate (all 1s) you must erase the whole page beforehand.
//    FLASH_EraseInitTypeDef EraseInitStruct;
//    uint32_t PageError = 0;
//
//    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
//    EraseInitStruct.Banks     = FLASH_BANK_2;   // last page falls in bank 2 on a 1MB part
//    EraseInitStruct.Page      = 255;            // page index within that bank
//    EraseInitStruct.NbPages   = 1;
//
//    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
//    {
//        HAL_FLASH_Lock();
//        Error_Handler();   // PageError tells you which page failed, if nonzero
//    }
//
//    // Step 3: Program the string, 8 bytes (one "double word") at a time.
//    // The L4 flash peripheral only accepts writes in 8-byte chunks.
//    int len = strlen(str) + 1;   // +1 to include the null terminator '\0'
//    uint32_t addr = address;
//
//    // Outer loop: picks which group of 8 bytes we're on, and which
//    // flash address that group goes to.
//    for (int i = 0; i < len; i += 8)
//    {
//        // A union: u.bytes and u.dword are the SAME 8 bytes of memory,
//        // just viewed two different ways. Writing to u.bytes[j] also
//        // changes u.dword — no copying needed.
//        union {
//            uint64_t dword;
//            uint8_t  bytes[8];
//        } u;
//
//        // Reset to "erased" value (all 1 bits) before filling this group.
//        // Anything we don't overwrite below stays as 0xFF.
//        u.dword = 0xFFFFFFFFFFFFFFFF;
//
//        // Inner loop: fills this one group's 8 slots (or fewer, if the
//        // string runs out first).
//        for (int j = 0; j < 8 && (i + j) < len; j++)
//        {
//            u.bytes[j] = str[i + j];
//        }
//
//        // Ship this group of 8 bytes to flash as one 64-bit write.
//        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, u.dword);
//
//        // Move to the next 8-byte address for the next group.
//        addr += 8;
//    }
//
//    // Step 4: Re-lock flash so nothing else can accidentally write to it.
//    HAL_FLASH_Lock();
//}

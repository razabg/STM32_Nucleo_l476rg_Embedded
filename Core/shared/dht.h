#ifndef DHT_H
#define DHT_H

#include "main.h"
#include <stdint.h>

// What a successful read returns
typedef struct {
    uint8_t humidity_int;     // e.g. 45  → 45%
    uint8_t humidity_dec;     // usually 0 for DHT11
    uint8_t temperature_int;  // e.g. 23  → 23°C
    uint8_t temperature_dec;  // usually 0 for DHT11
} DHT_Data;

// Return codes so caller knows if read succeeded
typedef enum {
    DHT_OK,
    DHT_NO_RESPONSE,
    DHT_CHECKSUM_ERROR
} DHT_Result;

// The only public function
DHT_Result DHT_Read(DHT_Data *out);

#endif

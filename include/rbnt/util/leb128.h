#pragma once
#include <stdint.h>

uint8_t encode_uleb128(uint32_t number, uint8_t* buffer) {
    uint8_t i = 0;
    do {
        uint8_t seg = number & 0x7f;
        number >>= 7;
        if(number != 0) {
            seg |= 0x80;
        }
        buffer[i] = seg;
        i++;
    } while(number != 0);
    return i;
}

uint32_t decode_uleb128(uint8_t* buffer) {
    uint32_t out = 0;
    uint8_t i = 0;
    do {
        out |= (buffer[i] & 0x7f) << (7*i);
        i++;
    } while(buffer[i-1] & 0x80);
    return out;
}

// uint32_t encode_sleb128(int32_t number, uint8_t* buffer) {
//     number = ~number + 1;
//     uint8_t i = 0;
//     do {
//         uint8_t seg = number & 0x7f;
//         number >>= 7;
//         if(number != 0) {
//             seg |= 0x80;
//         }
//         buffer[i] = seg;
//         i++;
//     } while(number != 0);
//     return i;
// }
// 
// int32_t decode_sleb128(uint8_t* buffer) {
//     int32_t out = 0;
//     uint8_t i = 0;
//     do {
//         out |= (buffer[i] & 0x7f) << (7*i);
//         i++;
//     } while(buffer[i-1] & 0x80);
//     return ~(out - 1);
// }

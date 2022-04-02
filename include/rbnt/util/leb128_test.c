#include <stdint.h>
#include <stdio.h>
#include "leb128.h"
#include <stdlib.h>

void print_bytes(uint8_t* buffer, int n) {
    int i;
    for(i=0; i<n; i++) {
        printf("%02hhX ", buffer[i]);
    }
}

void print_output(int n, uint8_t* buffer, int count) {
    printf("%d:", n);
    print_bytes(buffer, count);
}

int main() {
    uint8_t* buf = malloc(5 * sizeof(*buf));

    uint8_t bytes;

    const int uinput_size = 5;
    uint32_t uinputs[uinput_size] = {0, 36, 127, 128, 65535};

    int i;
    uint32_t n;
    for(i=0; i<uinput_size; i++) {
        n = uinputs[i];
        bytes = encode_uleb128(n, buf);
        printf("%d: ", n);
        print_bytes(buf, bytes);
        printf(": %d\n", decode_uleb128(buf));
    }
    
    /* const int input_size = 5; */
    /* int32_t inputs[input_size] = {0, -1, -126, -128, -65535}; */
    /*  */
    /* int32_t m; */
    /* for(i=0; i<uinput_size; i++) { */
    /*     m = uinputs[i]; */
    /*     bytes = encode_sleb128(m, buf); */
    /*     printf("%d: ", m); */
    /*     print_bytes(buf, bytes); */
    /*     printf(": %d\n", decode_sleb128(buf)); */
    /* } */
}

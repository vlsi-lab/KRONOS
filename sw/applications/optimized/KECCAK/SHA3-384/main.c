#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fips202.h"

#ifdef PERF_CNT_CYCLES
    #include "core_v_mini_mcu.h"
    #include "csr.h"
#endif

static const uint8_t t_expected[32] = {
    0xB1, 0x3F, 0xEB, 0xB1, 0xB3, 0xC5, 0x4A, 0x7C,
    0x6B, 0x69, 0x36, 0x7F, 0x69, 0x3A, 0x1D, 0x1F,
    0x31, 0x45, 0x70, 0x9B, 0x6D, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int main(void) {
    int result = 0;
    uint8_t input[200];
    uint8_t t[48];
    #if PERF_CNT_CYCLES
        unsigned int cycles, cycles2;
    #endif

    for (int i = 0; i < 200; i++) {
        input[i] = i;
    }

    printf("Test started!\n");
    #ifdef PERF_CNT_CYCLES
        CSR_CLEAR_BITS(CSR_REG_MCOUNTINHIBIT, 0x1);
        CSR_WRITE(CSR_REG_MCYCLE, 0);
    #endif
    sha3_384(t, input, 200);
    #ifdef PERF_CNT_CYCLES
        CSR_READ(CSR_REG_MCYCLE, &cycles);
        printf("Number of clock cycles for test-1 : %d\n", cycles);
    #endif

    for (int i = 0; i < 32; i++) {
        if (t[i] != t_expected[i]) {
            printf("Expected t[%d] = 0x%02X, but got 0x%02X.\n", i, t_expected[i], t[i]);
        }
    }

    printf("Test: terminated\n");


    printf("Received: ");
    for (int i = 0; i < 48; i++) {
        printf("%02X", t[i]);
    }

    return result;
}
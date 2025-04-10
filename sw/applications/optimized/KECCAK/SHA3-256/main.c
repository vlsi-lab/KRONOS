#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fips202.h"

#ifdef PERF_CNT_CYCLES
    #include "core_v_mini_mcu.h"
    #include "csr.h"
#endif

static const uint8_t t_expected[32] = {  // Expected output values (you should replace this with the actual test vector)
    0x5F, 0x72, 0x8F, 0x63, 0xBF, 0x5E, 0xE4, 0x8C,
    0x77, 0xF4, 0x53, 0xC0, 0x49, 0x03, 0x98, 0xFA,
    0x64, 0x5B, 0x8D, 0x4C, 0x4E, 0x56, 0xBE, 0x9A,
    0x41, 0xCF, 0xEC, 0x34, 0x4D, 0x6C, 0xA8, 0x99
};

int main(void) {
    int result = 0;
    uint8_t input[200];
    uint8_t t[32] = {0};
    #if PERF_CNT_CYCLES
        unsigned int cycles;
    #endif

    for (int i = 0; i < 200; i++) {
        input[i] = i;
    }

    printf("Test started!\n");
    #ifdef PERF_CNT_CYCLES
        CSR_CLEAR_BITS(CSR_REG_MCOUNTINHIBIT, 0x1);
        CSR_WRITE(CSR_REG_MCYCLE, 0);
    #endif
    sha3_256(t, input, 200);
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
    for (int i = 0; i < 32; i++) {
        printf("%02X", t[i]);
    }

    return result;
}
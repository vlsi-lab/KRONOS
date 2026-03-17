#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "core_v_mini_mcu.h"
#include "kronos_x_heep.h"
#include "core_v_mini_mcu.h"
#include "csr.h"

#define SIZE 50


static void KeccakF1600_StatePermute(uint64_t *state) {

	uint32_t Abe_low, Abe_high;

    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[0] ), [rs2] "r" ((state[0] >> 32) ), [i] "r" (0): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[1] ), [rs2] "r" ((state[1] >> 32) ), [i] "r" (2): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[2] ), [rs2] "r" ((state[2] >> 32) ), [i] "r" (4): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[3] ), [rs2] "r" ((state[3] >> 32) ), [i] "r" (6): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[4] ), [rs2] "r" ((state[4] >> 32) ), [i] "r" (8): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[5] ), [rs2] "r" ((state[5] >> 32) ), [i] "r" (10): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[6] ), [rs2] "r" ((state[6] >> 32) ), [i] "r" (12): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[7] ), [rs2] "r" ((state[7] >> 32) ), [i] "r" (14): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[8] ), [rs2] "r" ((state[8] >> 32) ), [i] "r" (16): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[9] ), [rs2] "r" ((state[9] >> 32) ), [i] "r" (18): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[10]), [rs2] "r" ((state[10] >> 32)), [i] "r" (20): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[11]), [rs2] "r" ((state[11] >> 32)), [i] "r" (22): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[12]), [rs2] "r" ((state[12] >> 32)), [i] "r" (24): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[13]), [rs2] "r" ((state[13] >> 32)), [i] "r" (26): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[14]), [rs2] "r" ((state[14] >> 32)), [i] "r" (28): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[15]), [rs2] "r" ((state[15] >> 32)), [i] "r" (30): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[16]), [rs2] "r" ((state[16] >> 32)), [i] "r" (32): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[17]), [rs2] "r" ((state[17] >> 32)), [i] "r" (34): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[18]), [rs2] "r" ((state[18] >> 32)), [i] "r" (36): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[19]), [rs2] "r" ((state[19] >> 32)), [i] "r" (38): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[20]), [rs2] "r" ((state[20] >> 32)), [i] "r" (40): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[21]), [rs2] "r" ((state[21] >> 32)), [i] "r" (42): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[22]), [rs2] "r" ((state[22] >> 32)), [i] "r" (44): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[23]), [rs2] "r" ((state[23] >> 32)), [i] "r" (46): );
    asm volatile (".insn r 0x4b, 0x004, 0, x0, %[rs1], %[rs2], %[i]\r\n": : [rs1] "r" (state[24]), [rs2] "r" ((state[24] >> 32)), [i] "r" (48): );

    asm volatile (".insn r 0x4b, 0x004, 1, x0, %[rs1], x0, x0\n\t" : : [rs1] "r" (1) : );

    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
        
    for (int i=0; i<50; i+=2){
        asm volatile (".insn r 0x4b, 0x004, 2, %[rd_low], %[rs1], x0, x0\r\n": [rd_low] "=r" (Abe_low) : [rs1] "r" (i): );
        asm volatile (".insn r 0x4b, 0x004, 2, %[rd_high], %[rs1], x0, x0\r\n": [rd_high] "=r" (Abe_high): [rs1] "r" (i+1): );
        state[i/2] = ((uint64_t)Abe_high << 32) | Abe_low;
    }

}

int main(){

	static uint32_t Din[SIZE] __attribute__ ((aligned (4)));
	static uint32_t Dout[SIZE] __attribute__ ((aligned (4)));

	static uint32_t D_expected[SIZE];
	int i = 0;

	memset(Din, 0, sizeof(Din));
	memset(Dout, 0, sizeof(Dout));
	memset(D_expected, 0, sizeof(D_expected));

	unsigned int cycles;

	
 
	Din[0] = 0x7369C667;
	Din[1] = 0xEC4AFF51;
	Din[2] = 0xABBACD29;
	Din[3] = 0x00000010;
	Din[31] = 0x80000000;


	D_expected[1] = 0xE1ADB0E2;
	D_expected[0] = 0xE7CB8356;
	D_expected[3] = 0xBB3F5FB8;
	D_expected[2] = 0x573A5BD7;
	D_expected[5] = 0xF7CA02A1;
	D_expected[4] = 0xE9784CC5;
	D_expected[7] = 0x6E54F256;
	D_expected[6] = 0x60A4C685;
	D_expected[9] = 0x77051F83;
	D_expected[8] = 0x243FCBAA;
	D_expected[11] = 0x6459DB0B;
	D_expected[10] = 0x4C063DD5;
	D_expected[13] = 0xE046DE71;
	D_expected[12] = 0xCB4B81C6;
	D_expected[15] = 0x94051793;
	D_expected[14] = 0xDB31F24C;
	D_expected[17] = 0xA13FC86C;
	D_expected[16] = 0xF16E32DD;
	D_expected[19] = 0xB962FC91;
	D_expected[18] = 0xB7737708;
	D_expected[21] = 0xD3CA2E7A;
	D_expected[20] = 0xFA27C801;
	D_expected[23] = 0x53C85108;
	D_expected[22] = 0xF72A3CCA;
	D_expected[25] = 0x73E732CD;
	D_expected[24] = 0xADF0E783;
	D_expected[27] = 0x8470BD54;
	D_expected[26] = 0xC4BDD1BF; 
	D_expected[29] = 0xD10B916F;
	D_expected[28] = 0x7C8C1F77; 
	D_expected[31] = 0x51129474;
	D_expected[30] = 0x440A2670; 
	D_expected[33] = 0x3D77CB49;
	D_expected[32] = 0xE9960C44; 
	D_expected[35] = 0xEC5001EB;
	D_expected[34] = 0xE4251E39; 
	D_expected[37] = 0x77A0EEC5;
	D_expected[36] = 0xEA4FD653;
	D_expected[39] = 0xEBC86BD4;
	D_expected[38] = 0x7B6773E7; 
	D_expected[41] = 0xE77DF6B0;
	D_expected[40] = 0x128FDC4B; 
	D_expected[43] = 0x0DB0D48A;
	D_expected[42] = 0x02F1B12E; 
	D_expected[45] = 0x241B344D;
	D_expected[44] = 0x0DC38AE5;
	D_expected[47] = 0xC3EE4E27;
	D_expected[46] = 0x532483D8;
	D_expected[49] = 0x0271BFE2;
	D_expected[48] = 0x84B1B424;
	printf("Hello Keccak\n");

	CSR_CLEAR_BITS(CSR_REG_MCOUNTINHIBIT, 0x1);
    CSR_WRITE(CSR_REG_MCYCLE, 0);
	KeccakF1600_StatePermute(Din);	
	CSR_READ(CSR_REG_MCYCLE, &cycles);
    printf("Number of clock cycles for KeccakF1600_StatePermute: %d\n", cycles);

	for (i = 0; i< SIZE; i++ ){
		if (Din[i] != D_expected[i]){
			printf("ERROR keccak output did not match test vector. ");
        	printf("Expected D[%d]: %04X but obtained %04X \n", i, D_expected[i], Din[i]);
		}	
	}	
	printf("Keccak terminated!\n");
	return 0;

}
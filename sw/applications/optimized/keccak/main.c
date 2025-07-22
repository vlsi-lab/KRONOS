#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "core_v_mini_mcu.h"
#include "kronos_x_heep.h"
#include "core_v_mini_mcu.h"
#include "csr.h"

#define SIZE 50
 #define NROUNDS 24
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))

void rol32_asm(uint32_t a, uint32_t b, unsigned int offset, uint32_t *result1, uint32_t *result2) {
    
    asm volatile (
        ".insn r 0x6b, 0x01, 0x0, %[rd1],  %[rs2], %[rs1], %[i]\n\t"
        ".insn r 0x6b, 0x01, 0x1, %[rd2], x0, x0, x0\n\t"
        : [rd1] "=r" (*result1), [rd2] "=r" (*result2)
        : [rs1] "r" (a), [rs2] "r" (b), [i] "r" (offset)
        : "memory");
}
 
 
  static const uint32_t KeccakF1600RoundConstants32[2 * 24] = {
    0x00000000, 0x00000001,
    0x00000000, 0x00008082,
    0x80000000, 0x0000808a,
    0x80000000, 0x80008000,
    0x00000000, 0x0000808b,
    0x00000000, 0x80000001,
    0x80000000, 0x80008081,
    0x80000000, 0x00008009,
    0x00000000, 0x0000008a,
    0x00000000, 0x00000088,
    0x00000000, 0x80008009,
    0x00000000, 0x8000000a,
    0x00000000, 0x8000808b,
    0x80000000, 0x0000008b,
    0x80000000, 0x00008089,
    0x80000000, 0x00008003,
    0x80000000, 0x00008002,
    0x80000000, 0x00000080,
    0x00000000, 0x0000800a,
    0x80000000, 0x8000000a,
    0x80000000, 0x80008081,
    0x80000000, 0x00008080,
    0x00000000, 0x80000001,
    0x80000000, 0x80008008
};

void KeccakF1600_StatePermute(uint32_t *state){

    uint32_t Aba0, Abe0, Abi0, Abo0, Abu0;
    uint32_t Aba1, Abe1, Abi1, Abo1, Abu1;
    uint32_t Aga0, Age0, Agi0, Ago0, Agu0;
    uint32_t Aga1, Age1, Agi1, Ago1, Agu1;
    uint32_t Aka0, Ake0, Aki0, Ako0, Aku0;
    uint32_t Aka1, Ake1, Aki1, Ako1, Aku1;
    uint32_t Ama0, Ame0, Ami0, Amo0, Amu0;
    uint32_t Ama1, Ame1, Ami1, Amo1, Amu1;
    uint32_t Asa0, Ase0, Asi0, Aso0, Asu0;
    uint32_t Asa1, Ase1, Asi1, Aso1, Asu1;
    uint32_t BCa0, BCe0, BCi0, BCo0, BCu0;
    uint32_t BCa1, BCe1, BCi1, BCo1, BCu1;
    uint32_t Da0, De0, Di0, Do0, Du0;
    uint32_t Da1, De1, Di1, Do1, Du1;
    uint32_t Eba0, Ebe0, Ebi0, Ebo0, Ebu0;
    uint32_t Eba1, Ebe1, Ebi1, Ebo1, Ebu1;
    uint32_t Ega0, Ege0, Egi0, Ego0, Egu0;
    uint32_t Ega1, Ege1, Egi1, Ego1, Egu1;
    uint32_t Eka0, Eke0, Eki0, Eko0, Eku0;
    uint32_t Eka1, Eke1, Eki1, Eko1, Eku1;
    uint32_t Ema0, Eme0, Emi0, Emo0, Emu0;
    uint32_t Ema1, Eme1, Emi1, Emo1, Emu1;
    uint32_t Esa0, Ese0, Esi0, Eso0, Esu0;
    uint32_t Esa1, Ese1, Esi1, Eso1, Esu1;

    //copyFromState(A, state)
        Aba0 = state[ 0];
        Aba1 = state[ 1];
        Abe0 = state[ 2];
        Abe1 = state[ 3];
        Abi0 = state[ 4];
        Abi1 = state[ 5];
        Abo0 = state[ 6];
        Abo1 = state[ 7];
        Abu0 = state[ 8];
        Abu1 = state[ 9];
        Aga0 = state[10];
        Aga1 = state[11];
        Age0 = state[12];
        Age1 = state[13];
        Agi0 = state[14];
        Agi1 = state[15];
        Ago0 = state[16];
        Ago1 = state[17];
        Agu0 = state[18];
        Agu1 = state[19];
        Aka0 = state[20];
        Aka1 = state[21];
        Ake0 = state[22];
        Ake1 = state[23];
        Aki0 = state[24];
        Aki1 = state[25];
        Ako0 = state[26];
        Ako1 = state[27];
        Aku0 = state[28];
        Aku1 = state[29];
        Ama0 = state[30];
        Ama1 = state[31];
        Ame0 = state[32];
        Ame1 = state[33];
        Ami0 = state[34];
        Ami1 = state[35];
        Amo0 = state[36];
        Amo1 = state[37];
        Amu0 = state[38];
        Amu1 = state[39];
        Asa0 = state[40];
        Asa1 = state[41];
        Ase0 = state[42];
        Ase1 = state[43];
        Asi0 = state[44];
        Asi1 = state[45];
        Aso0 = state[46];
        Aso1 = state[47];
        Asu0 = state[48];
        Asu1 = state[49];

        uint32_t Da0a, Da0b;
        uint32_t De0a, De0b;
        uint32_t Di0a, Di0b;
        uint32_t Do0a, Do0b;
        uint32_t Du0a, Du0b;
        uint32_t BCa0a, BCa0b;
        uint32_t BCe0a, BCe0b;
        uint32_t BCi0a, BCi0b;
        uint32_t BCo0a, BCo0b;
        uint32_t BCu0a, BCu0b;


        for(int round = 0; round < 24; round += 2 )
        {
            //printf("\n\n\n\nround=%d\n\n\n\n\n\n", round);

            //    prepareTheta
            BCa0 = Aba0^Aga0^Aka0^Ama0^Asa0;
            BCa1 = Aba1^Aga1^Aka1^Ama1^Asa1;
            
            BCe0 = Abe0^Age0^Ake0^Ame0^Ase0;
            BCe1 = Abe1^Age1^Ake1^Ame1^Ase1;

            BCi0 = Abi0^Agi0^Aki0^Ami0^Asi0;
            BCi1 = Abi1^Agi1^Aki1^Ami1^Asi1;

            BCo0 = Abo0^Ago0^Ako0^Amo0^Aso0;
            BCo1 = Abo1^Ago1^Ako1^Amo1^Aso1;

            BCu0 = Abu0^Agu0^Aku0^Amu0^Asu0;
            BCu1 = Abu1^Agu1^Aku1^Amu1^Asu1;

            //printf("BCa: %04X-%04X\n", BCa1, BCa0);
            //printf("BCe: %04X-%04X\n", BCe1, BCe0);
            //printf("BCi: %04X-%04X\n", BCi1, BCi0);
            //printf("BCo: %04X-%04X\n", BCo1, BCo0);
            //printf("BCu: %04X-%04X\n\n", BCu1, BCu0);

            //thetaRhoPiChiIota(round  , A, E)
            rol32_asm(BCe1, BCe0, 1, &Da0a, &Da0b);
            Da0 = BCu0^Da0b;
            Da1 = BCu1^Da0a;

            rol32_asm(BCi1, BCi0, 1, &De0a, &De0b);
            De0 = BCa0^De0b;
            De1 = BCa1^De0a;

            rol32_asm(BCo1, BCo0, 1, &Di0a, &Di0b);
            Di0 = BCe0^Di0b;
            Di1 = BCe1^Di0a;

            rol32_asm(BCu1, BCu0, 1, &Do0a, &Do0b);
            Do0 = BCi0^Do0b;
            Do1 = BCi1^Do0a;

            rol32_asm(BCa1, BCa0, 1, &Du0a, &Du0b);
            Du0 = BCo0^Du0b;
            Du1 = BCo1^Du0a;

            //printf("Da: %04X-%04X\n", Da1, Da0);
            //printf("De: %04X-%04X\n", De1, De0);
            //printf("Di: %04X-%04X\n", Di1, Di0);
            //printf("Do: %04X-%04X\n", Do1, Do0);
            //printf("Du: %04X-%04X\n", Du1, Du0);

            Aba0 ^= Da0;
            Aba1 ^= Da1;
            BCa0 = Aba0;
            BCa1 = Aba1;
            Age0 ^= De0;
            Age1 ^= De1;

            ////printf("Aba: %04X-%04X\n", Aba1, Aba0);
            ////printf("BCa: %04X-%04X\n", BCa1, BCa0);
            //printf("\nAge: %04X-%04X\n", Age1, Age0);

            rol32_asm(Age1, Age0, 44, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;

            Aki1 ^= Di1;
            Aki0 ^= Di0;
            //printf("\nAki: %04X-%04X\n", Aki1, Aki0);


            rol32_asm(Aki1, Aki0, 43, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a;

            Amo1 ^= Do1;
            Amo0 ^= Do0;

            //printf("\nAmo: %04X-%04X\n", Amo1, Amo0);
            rol32_asm(Amo1, Amo0, 21, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Asu0 ^= Du0;
            Asu1 ^= Du1;
            //printf("\nAsu: %04X-%04X\n", Asu1, Asu0);


            rol32_asm(Asu1, Asu0, 14, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;

            //printf("BCe: %04X-%04X\n", BCe1, BCe0);
            //printf("BCi: %04X-%04X\n", BCi1, BCi0);
            //printf("BCo: %04X-%04X\n", BCo1, BCo0);
            //printf("BCu: %04X-%04X\n", BCu1, BCu0);


            Eba0 =   BCa0 ^((~BCe0)&  BCi0 );
            Eba0 ^= KeccakF1600RoundConstants32[round*2+1];

            Eba1 =   BCa1 ^((~BCe1)&  BCi1 );
            Eba1 ^= KeccakF1600RoundConstants32[round*2+0];
            ////printf("\n\n\n*******Eba0: %04X-%04X\n", Eba1, Eba0);
            ////printf("keccakconst: %04X-%04X\n", KeccakF1600RoundConstants32[round*2+0], KeccakF1600RoundConstants32[round*2+1]);

            Ebe0 =   BCe0 ^((~BCi0)&  BCo0 );
            Ebe1 =   BCe1 ^((~BCi1)&  BCo1 );

            Ebi0 =   BCi0 ^((~BCo0)&  BCu0 );
            Ebi1 =   BCi1 ^((~BCo1)&  BCu1 );

            Ebo0 =   BCo0 ^((~BCu0)&  BCa0 );
            Ebo1 =   BCo1 ^((~BCu1)&  BCa1 );

            Ebu0 =   BCu0 ^((~BCa0)&  BCe0 );
            Ebu1 =   BCu1 ^((~BCa1)&  BCe1 );

            ////printf("Ebe0: %04X-%04X\n", Ebe1, Ebe0);
            ////printf("Ebi0: %04X-%04X\n", Ebi1, Ebi0);
            ////printf("Ebo0: %04X-%04X\n", Ebo1, Ebo0);
            ////printf("Ebu0: %04X-%04X\n", Ebu1, Ebu0);
       
            Abo0 ^= Do0;
            Abo1 ^= Do1;

            rol32_asm(Abo1, Abo0, 28, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a;

            Agu0 ^= Du0;
            Agu1 ^= Du1;

            rol32_asm(Agu1, Agu0, 20, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;

            Aka1 ^= Da1;
            Aka0 ^= Da0;

            rol32_asm(Aka1, Aka0, 3, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a;

            Ame1 ^= De1;
            Ame0 ^= De0;

            rol32_asm(Ame1, Ame0, 45, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Asi1 ^= Di1;
            Asi0 ^= Di0;


            rol32_asm(Asi1, Asi0, 61, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;

            //printf("\n\nAbo: %04X-%04X\n", Abo1, Abo0);
            //printf("Agu: %04X-%04X\n", Agu1, Agu0);
            //printf("Aka: %04X-%04X\n", Aka1, Aka0);
            //printf("Ame: %04X-%04X\n", Ame1, Ame0);
            //printf("Asi: %04X-%04X\n", Asi1, Asi0);

            //printf("\n\nBCa: %04X-%04X\n", BCa1, BCa0);
            //printf("BCe: %04X-%04X\n", BCe1, BCe0);
            //printf("BCi: %04X-%04X\n", BCi1, BCi0);
            //printf("BCo: %04X-%04X\n", BCo1, BCo0);
            //printf("BCu: %04X-%04X\n", BCu1, BCu0);

            Ega0 =   BCa0 ^((~BCe0)&  BCi0 );
            Ega1 =   BCa1 ^((~BCe1)&  BCi1 );

            Ege0 =   BCe0 ^((~BCi0)&  BCo0 );
            Ege1 =   BCe1 ^((~BCi1)&  BCo1 );
            
            Egi0 =   BCi0 ^((~BCo0)&  BCu0 );
            Egi1 =   BCi1 ^((~BCo1)&  BCu1 );

            Ego0 =   BCo0 ^((~BCu0)&  BCa0 );
            Ego1 =   BCo1 ^((~BCu1)&  BCa1 );

            Egu0 =   BCu0 ^((~BCa0)&  BCe0 );
            Egu1 =   BCu1 ^((~BCa1)&  BCe1 );

            //printf("\n\nEga: %04X-%04X\n", Ega1, Ega0);
            //printf("Ege: %04X-%04X\n", Ege1, Ege0);
            //printf("Egi: %04X-%04X\n", Egi1, Egi0);
            //printf("Ego: %04X-%04X\n", Ego1, Ego0);
            //printf("Egu: %04X-%04X\n", Egu1, Egu0);

            //printf("\n\nAbe before: %04X-%04X\n", Abe1, Abe0);
            //printf("De: %04X-%04X\n", De1, De0);
            Abe1 ^= De1;
            Abe0 ^= De0;
            //printf("Abe: %04X-%04X\n", Abe1, Abe0);

            rol32_asm(Abe1, Abe0, 1, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a;
            //printf("BCa: %04X-%04X\n", BCa1, BCa0);

            //printf("\n\nAgi before: %04X-%04X\n", Agi1, Agi0);
            //printf("Di: %04X-%04X\n", Di1, Di0);
            Agi0 ^= Di0;
            Agi1 ^= Di1;
            //printf("Agi: %04X-%04X\n", Agi1, Agi0);

            rol32_asm(Agi1, Agi0, 6, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;

            Ako1 ^= Do1;
            Ako0 ^= Do0;
            
            //printf("\n\nAko before: %04X-%04X\n", Ako1, Ako0);
            //printf("Do: %04X-%04X\n", Do1, Do0);
            rol32_asm(Ako1, Ako0, 25, &BCi0a, &BCi0b);
            //printf("\n\nAko: %04X-%04X\n", Ako1, Ako0);
            
            BCi0 = BCi0b;
            BCi1 = BCi0a;

            Amu0 ^= Du0;
            Amu1 ^= Du1;

            rol32_asm(Amu1, Amu0, 8, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Asa0 ^= Da0;
            Asa1 ^= Da1;

            rol32_asm(Asa1, Asa0, 18, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;

            //printf("\n\nBCa: %04X-%04X\n", BCa1, BCa0);
            //printf("BCe: %04X-%04X\n", BCe1, BCe0);
            //printf("BCi: %04X-%04X\n", BCi1, BCi0);
            //printf("BCo: %04X-%04X\n", BCo1, BCo0);
            //printf("BCu: %04X-%04X\n", BCu1, BCu0);

            Eka0 =   BCa0 ^((~BCe0)&  BCi0 );
            Eka1 =   BCa1 ^((~BCe1)&  BCi1 );

            Eke0 =   BCe0 ^((~BCi0)&  BCo0 );
            Eke1 =   BCe1 ^((~BCi1)&  BCo1 );

            Eki0 =   BCi0 ^((~BCo0)&  BCu0 );
            Eki1 =   BCi1 ^((~BCo1)&  BCu1 );

            Eko0 =   BCo0 ^((~BCu0)&  BCa0 );
            Eko1 =   BCo1 ^((~BCu1)&  BCa1 );

            Eku0 =   BCu0 ^((~BCa0)&  BCe0 );
            Eku1 =   BCu1 ^((~BCa1)&  BCe1 );

            ////printf("Eka: %04X-%04X\n", Eka1, Eka0);
            ////printf("Eke: %04X-%04X\n", Eke1, Eke0);
            ////printf("Eki: %04X-%04X\n", Eki1, Eki0);
            ////printf("Eko: %04X-%04X\n", Eko1, Eko0);
            ////printf("Eku: %04X-%04X\n", Eku1, Eku0);
       
            Abu1 ^= Du1;
            Abu0 ^= Du0;

            rol32_asm(Abu1, Abu0, 27, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a;

            Aga0 ^= Da0;
            Aga1 ^= Da1;

            rol32_asm(Aga1, Aga0, 36, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;

            Ake0 ^= De0;
            Ake1 ^= De1;

            rol32_asm(Ake1, Ake0, 10, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a;

            Ami1 ^= Di1;
            Ami0 ^= Di0;

            rol32_asm(Ami1, Ami0, 15, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Aso0 ^= Do0;
            Aso1 ^= Do1;

            rol32_asm(Aso1, Aso0, 56, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;

            //printf("\n\n\nBCa: %04X-%04X\n", BCa1, BCa0);
            //printf("BCe: %04X-%04X\n", BCe1, BCe0);
            //printf("BCi: %04X-%04X\n", BCi1, BCi0);
            //printf("BCo: %04X-%04X\n", BCo1, BCo0);
            //printf("BCu: %04X-%04X\n", BCu1, BCu0);

            Ema0 =   BCa0 ^((~BCe0)&  BCi0 );
            Ema1 =   BCa1 ^((~BCe1)&  BCi1 );

            Eme0 =   BCe0 ^((~BCi0)&  BCo0 );
            Eme1 =   BCe1 ^((~BCi1)&  BCo1 );

            Emi0 =   BCi0 ^((~BCo0)&  BCu0 );
            Emi1 =   BCi1 ^((~BCo1)&  BCu1 );

            Emo0 =   BCo0 ^((~BCu0)&  BCa0 );
            Emo1 =   BCo1 ^((~BCu1)&  BCa1 );

            Emu0 =   BCu0 ^((~BCa0)&  BCe0 );       
            Emu1 =   BCu1 ^((~BCa1)&  BCe1 );

            //printf("\n\nEma: %04X-%04X\n", Ema1, Ema0);
            //printf("Emi: %04X-%04X\n", Eme1, Eme0);
            //printf("Eme: %04X-%04X\n", Emi1, Emi0);
            //printf("Emo: %04X-%04X\n", Emo1, Emo0);
            //printf("Emu: %04X-%04X\n", Emu1, Emu0);
       
            Abi0 ^= Di0;
            Abi1 ^= Di1;

            rol32_asm(Abi1, Abi0, 62, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a;

            Ago1 ^= Do1;
            Ago0 ^= Do0;

            rol32_asm(Ago1, Ago0, 55, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;

            Aku1 ^= Du1;
            Aku0 ^= Du0;

            rol32_asm(Aku1, Aku0, 39, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a;

            Ama1 ^= Da1;
            Ama0 ^= Da0;

            rol32_asm(Ama1, Ama0, 41, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Ase0 ^= De0;
            Ase1 ^= De1;

            rol32_asm(Ase1, Ase0, 2, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;

            //printf("\n\nBca: %04X-%04X\n", BCa1, BCa0);
            //printf("BCi: %04X-%04X\n", BCe1, BCe0);
            //printf("BCe: %04X-%04X\n", BCi1, BCi0);
            //printf("BCo: %04X-%04X\n", BCo1, BCo0);
            //printf("BCu: %04X-%04X\n", BCu1, BCu0);

            Esa0 =   BCa0 ^((~BCe0)&  BCi0 );
            Esa1 =   BCa1 ^((~BCe1)&  BCi1 );

            Ese0 =   BCe0 ^((~BCi0)&  BCo0 );
            Ese1 =   BCe1 ^((~BCi1)&  BCo1 );

            Esi0 =   BCi0 ^((~BCo0)&  BCu0 );
            Esi1 =   BCi1 ^((~BCo1)&  BCu1 );

            Eso0 =   BCo0 ^((~BCu0)&  BCa0 );
            Eso1 =   BCo1 ^((~BCu1)&  BCa1 );

            Esu0 =   BCu0 ^((~BCa0)&  BCe0 );       
            Esu1 =   BCu1 ^((~BCa1)&  BCe1 );

            //printf("Esa: %04X-%04X\n", Esa1, Esa0);
            //printf("Ese: %04X-%04X\n", Ese1, Ese0);
            //printf("Esi: %04X-%04X\n", Esi1, Esi0);
            //printf("Eso: %04X-%04X\n", Eso1, Eso0);
            //printf("Esu: %04X-%04X\n", Esu1, Esu0);

            //    prepareTheta
            BCa0 = Eba0^Ega0^Eka0^Ema0^Esa0;
            BCa1 = Eba1^Ega1^Eka1^Ema1^Esa1;
            BCe0 = Ebe0^Ege0^Eke0^Eme0^Ese0;
            BCe1 = Ebe1^Ege1^Eke1^Eme1^Ese1;
            BCi0 = Ebi0^Egi0^Eki0^Emi0^Esi0;
            BCi1 = Ebi1^Egi1^Eki1^Emi1^Esi1;
            BCo0 = Ebo0^Ego0^Eko0^Emo0^Eso0;
            BCo1 = Ebo1^Ego1^Eko1^Emo1^Eso1;
            BCu0 = Ebu0^Egu0^Eku0^Emu0^Esu0;
            BCu1 = Ebu1^Egu1^Eku1^Emu1^Esu1;

            //printf("\n\nBCa: %04X-%04X\n", BCa1, BCa0);
            //printf("BCe: %04X-%04X\n", BCe1, BCe0);
            //printf("BCi: %04X-%04X\n", BCi1, BCi0);
            //printf("BCo: %04X-%04X\n", BCo1, BCo0);
            //printf("BCu: %04X-%04X\n", BCu1, BCu0);

            //thetaRhoPiChiIota(round+1, E, A)
            rol32_asm(BCe1, BCe0, 1, &BCe0a, &BCe0b);
            Da0 = BCe0b ^ BCu0;
            Da1 = BCe0a ^ BCu1;

            rol32_asm(BCi1, BCi0, 1, &BCi0a, &BCi0b);
            De0 = BCi0b ^ BCa0;
            De1 = BCi0a ^ BCa1;

            rol32_asm(BCo1, BCo0, 1, &BCo0a, &BCo0b);
            Di0 = BCo0b ^ BCe0;
            Di1 = BCo0a ^ BCe1;

            rol32_asm(BCu1, BCu0, 1, &BCu0a, &BCu0b);
            Do0 = BCu0b ^ BCi0;
            Do1 = BCu0a ^ BCi1;

            rol32_asm(BCa1, BCa0, 1, &BCa0a, &BCa0b);
            Du0 = BCa0b ^ BCo0;
            Du1 = BCa0a ^ BCo1;

            //printf("\n\nDa: %04X-%04X\n", Da1, Da0);
            //printf("De: %04X-%04X\n", De1, De0);
            //printf("Di: %04X-%04X\n", Di1, Di0);
            //printf("Do: %04X-%04X\n", Do1, Do0);
            //printf("Du: %04X-%04X\n", Du1, Du0);

            Eba0 ^= Da0;
            Eba1 ^= Da1;

            BCa0 = Eba0;
            BCa1 = Eba1;

            Ege0 ^= De0;
            Ege1 ^= De1;

            rol32_asm(Ege1, Ege0, 44, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;

            Eki1 ^= Di1;
            Eki0 ^= Di0;

            rol32_asm(Eki1, Eki0, 43, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a;

            Emo1 ^= Do1;
            Emo0 ^= Do0;

            rol32_asm(Emo1, Emo0, 21, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Esu0 ^= Du0;
            Esu1 ^= Du1;

            rol32_asm(Esu1, Esu0, 14, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;

            //printf("BCe: %04X-%04X\n", BCe1, BCe0);

            Aba0 =   BCa0 ^((~BCe0)&  BCi0 );
            Aba1 =   BCa1 ^((~BCe1)&  BCi1 );
            //printf("\n\nAba: %04X-%04X\n", Aba1, Aba0);

            Aba0 ^= KeccakF1600RoundConstants32[round*2+3];
            Aba1 ^= KeccakF1600RoundConstants32[round*2+2];
            //printf("\n\nAba: %04X-%04X\n", Aba1, Aba0);
            //printf("keccakconst: %04X-%04X\n", KeccakF1600RoundConstants32[round*2+2], KeccakF1600RoundConstants32[round*2+3]);

            Abe0 =   BCe0 ^((~BCi0)&  BCo0 );
            Abe1 =   BCe1 ^((~BCi1)&  BCo1 );

            Abi0 =   BCi0 ^((~BCo0)&  BCu0 );
            Abi1 =   BCi1 ^((~BCo1)&  BCu1 );

            Abo0 =   BCo0 ^((~BCu0)&  BCa0 );
            Abo1 =   BCo1 ^((~BCu1)&  BCa1 );

            Abu0 =   BCu0 ^((~BCa0)&  BCe0 );
            Abu1 =   BCu1 ^((~BCa1)&  BCe1 );
       
            Ebo0 ^= Do0;
            Ebo1 ^= Do1;
            
            rol32_asm(Ebo1, Ebo0, 28, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a;

            Egu0 ^= Du0;
            Egu1 ^= Du1;

            rol32_asm(Egu1, Egu0, 20, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;


            Eka1 ^= Da1;
            Eka0 ^= Da0;

            rol32_asm(Eka1, Eka0, 3, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a;

            Eme1 ^= De1;
            Eme0 ^= De0;

            rol32_asm(Eme1, Eme0, 45, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Esi1 ^= Di1;
            Esi0 ^= Di0;

            rol32_asm(Esi1, Esi0, 61, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;

            //printf("\n\nBca: %04X-%04X\n", BCa1, BCa0);
            //printf("Bce: %04X-%04X\n", BCe1, BCe0);
            //printf("Bci: %04X-%04X\n", BCi1, BCi0);
            //printf("Bco: %04X-%04X\n", BCo1, BCo0);
            //printf("Bcu: %04X-%04X\n", BCu1, BCu0);


            Aga0 =   BCa0 ^((~BCe0)&  BCi0 );
            Aga1 =   BCa1 ^((~BCe1)&  BCi1 );

            Age0 =   BCe0 ^((~BCi0)&  BCo0 );
            Age1 =   BCe1 ^((~BCi1)&  BCo1 );

            Agi0 =   BCi0 ^((~BCo0)&  BCu0 );
            Agi1 =   BCi1 ^((~BCo1)&  BCu1 );

            Ago0 =   BCo0 ^((~BCu0)&  BCa0 );
            Ago1 =   BCo1 ^((~BCu1)&  BCa1 );

            Agu0 =   BCu0 ^((~BCa0)&  BCe0 );       
            Agu1 =   BCu1 ^((~BCa1)&  BCe1 );
       
            //printf("\n\nAga: %04X-%04X\n", Aga1, Aga0);
            //printf("age: %04X-%04X\n", Age1, Age0);
            //printf("agi: %04X-%04X\n", Agi1, Agi0);
            //printf("ago: %04X-%04X\n", Ago1, Ago0);
            //printf("agu: %04X-%04X\n", Agu1, Agu0);

            Ebe1 ^= De1;
            Ebe0 ^= De0;

            rol32_asm(Ebe1, Ebe0, 1, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a;

            Egi0 ^= Di0;
            Egi1 ^= Di1;
            
            rol32_asm(Egi1, Egi0, 6, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a;
            
            Eko1 ^= Do1;
            Eko0 ^= Do0;
            
            rol32_asm(Eko1, Eko0, 25, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a;
            
            Emu0 ^= Du0;
            Emu1 ^= Du1;

            rol32_asm(Emu1, Emu0, 8, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a;

            Esa0 ^= Da0;
            Esa1 ^= Da1;

            rol32_asm(Esa1, Esa0, 18, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a;         
            
            Aka0 =   BCa0 ^((~BCe0)&  BCi0 );
            Aka1 =   BCa1 ^((~BCe1)&  BCi1 );

            Ake0 =   BCe0 ^((~BCi0)&  BCo0 );
            Ake1 =   BCe1 ^((~BCi1)&  BCo1 );

            Aki0 =   BCi0 ^((~BCo0)&  BCu0 );
            Aki1 =   BCi1 ^((~BCo1)&  BCu1 );

            Ako0 =   BCo0 ^((~BCu0)&  BCa0 );
            Ako1 =   BCo1 ^((~BCu1)&  BCa1 );

            Aku0 =   BCu0 ^((~BCa0)&  BCe0 );
            Aku1 =   BCu1 ^((~BCa1)&  BCe1 );

            //printf("\n\nAka: %04X-%04X\n", Aka1, Aka0);
            //printf("ake: %04X-%04X\n", Ake1, Ake0);
            //printf("aki: %04X-%04X\n", Aki1, Aki0);
            //printf("ako: %04X-%04X\n", Ako1, Ako0);
            //printf("aku: %04X-%04X\n", Aku1, Aku0);

            Ebu1 ^= Du1;
            Ebu0 ^= Du0;

            rol32_asm(Ebu1, Ebu0, 27, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a;   
            
            Ega0 ^= Da0;
            Ega1 ^= Da1;
            
            rol32_asm(Ega1, Ega0, 36, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a; 
            
            Eke0 ^= De0;
            Eke1 ^= De1;

            rol32_asm(Eke1, Eke0, 10, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a; 
            
            Emi1 ^= Di1;
            Emi0 ^= Di0;
            
            rol32_asm(Emi1, Emi0, 15, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a; 
            
            Eso0 ^= Do0;
            Eso1 ^= Do1;

            rol32_asm(Eso1, Eso0, 56, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a; 
            
            Ama0 =   BCa0 ^((~BCe0)&  BCi0 );
            Ama1 =   BCa1 ^((~BCe1)&  BCi1 );
            
            Ame0 =   BCe0 ^((~BCi0)&  BCo0 );
            Ame1 =   BCe1 ^((~BCi1)&  BCo1 );
            
            Ami0 =   BCi0 ^((~BCo0)&  BCu0 );
            Ami1 =   BCi1 ^((~BCo1)&  BCu1 );
            
            Amo0 =   BCo0 ^((~BCu0)&  BCa0 );
            Amo1 =   BCo1 ^((~BCu1)&  BCa1 );
            
            Amu0 =   BCu0 ^((~BCa0)&  BCe0 );       
            Amu1 =   BCu1 ^((~BCa1)&  BCe1 );

            //printf("\n\nAma: %04X-%04X\n", Ama1, Ama0);
            //printf("ame: %04X-%04X\n", Ame1, Ame0);
            //printf("ami: %04X-%04X\n", Ami1, Ami0);
            //printf("amo: %04X-%04X\n", Amo1, Amo0);
            //printf("amu: %04X-%04X\n", Amu1, Amu0);

            Ebi0 ^= Di0;
            Ebi1 ^= Di1;

            rol32_asm(Ebi1, Ebi0, 62, &BCa0a, &BCa0b);
            BCa0 = BCa0b;
            BCa1 = BCa0a; 

            Ego1 ^= Do1;
            Ego0 ^= Do0;

            rol32_asm(Ego1, Ego0, 55, &BCe0a, &BCe0b);
            BCe0 = BCe0b;
            BCe1 = BCe0a; 

            Eku1 ^= Du1;
            Eku0 ^= Du0;

            rol32_asm(Eku1, Eku0, 39, &BCi0a, &BCi0b);
            BCi0 = BCi0b;
            BCi1 = BCi0a; 

            Ema1 ^= Da1;
            Ema0 ^= Da0;

            rol32_asm(Ema1, Ema0, 41, &BCo0a, &BCo0b);
            BCo0 = BCo0b;
            BCo1 = BCo0a; 

            Ese0 ^= De0;
            Ese1 ^= De1;

            rol32_asm(Ese1, Ese0, 2, &BCu0a, &BCu0b);
            BCu0 = BCu0b;
            BCu1 = BCu0a; 

            Asa0 =   BCa0 ^((~BCe0)&  BCi0 );
            Asa1 =   BCa1 ^((~BCe1)&  BCi1 );

            Ase0 =   BCe0 ^((~BCi0)&  BCo0 );
            Ase1 =   BCe1 ^((~BCi1)&  BCo1 );

            Asi0 =   BCi0 ^((~BCo0)&  BCu0 );
            Asi1 =   BCi1 ^((~BCo1)&  BCu1 );

            Aso0 =   BCo0 ^((~BCu0)&  BCa0 );
            Aso1 =   BCo1 ^((~BCu1)&  BCa1 );

            Asu0 =   BCu0 ^((~BCa0)&  BCe0 );       
            Asu1 =   BCu1 ^((~BCa1)&  BCe1 );

            //printf("\n\nAsa: %04X-%04X\n", Asa1, Asa0);
            //printf("ase: %04X-%04X\n", Ase1, Ase0);
            //printf("asi: %04X-%04X\n", Asi1, Asi0);
            //printf("aso: %04X-%04X\n", Aso1, Aso0);
            //printf("asu: %04X-%04X\n", Asu1, Asu0);
            
        }

        //copyToState(state, A)
        state[ 0] = Aba0;
        state[ 1] = Aba1;
        state[ 2] = Abe0;
        state[ 3] = Abe1;
        state[ 4] = Abi0;
        state[ 5] = Abi1;
        state[ 6] = Abo0;
        state[ 7] = Abo1;
        state[ 8] = Abu0;
        state[ 9] = Abu1;
        state[10] = Aga0;
        state[11] = Aga1;
        state[12] = Age0;
        state[13] = Age1;
        state[14] = Agi0;
        state[15] = Agi1;
        state[16] = Ago0;
        state[17] = Ago1;
        state[18] = Agu0;
        state[19] = Agu1;
        state[20] = Aka0;
        state[21] = Aka1;
        state[22] = Ake0;
        state[23] = Ake1;
        state[24] = Aki0;
        state[25] = Aki1;
        state[26] = Ako0;
        state[27] = Ako1;
        state[28] = Aku0;
        state[29] = Aku1;
        state[30] = Ama0;
        state[31] = Ama1;
        state[32] = Ame0;
        state[33] = Ame1;
        state[34] = Ami0;
        state[35] = Ami1;
        state[36] = Amo0;
        state[37] = Amo1;
        state[38] = Amu0;
        state[39] = Amu1;
        state[40] = Asa0;
        state[41] = Asa1;
        state[42] = Ase0;
        state[43] = Ase1;
        state[44] = Asi0;
        state[45] = Asi1;
        state[46] = Aso0;
        state[47] = Aso1;
        state[48] = Asu0;
        state[49] = Asu1;

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
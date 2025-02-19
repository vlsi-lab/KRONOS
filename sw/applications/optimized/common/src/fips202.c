/* Based on the public domain implementation in
 * crypto_hash/keccakc512/simple/ from http://bench.cr.yp.to/supercop.html
 * by Ronny Van Keer
 * and the public domain "TweetFips202" implementation
 * from https://twitter.com/tweetfips202
 * by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe */

 #include <stddef.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdio.h>
 
 #include "fips202.h"
 
 #define NROUNDS 24
 #define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))
 
 /*************************************************
  * Name:        load64
  *
  * Description: Load 8 bytes into uint64_t in little-endian order
  *
  * Arguments:   - const uint8_t *x: pointer to input byte array
  *
  * Returns the loaded 64-bit unsigned integer
  **************************************************/
 static uint64_t load64(const uint8_t *x) {
     uint64_t r = 0;
     for (size_t i = 0; i < 8; ++i) {
         r |= (uint64_t)x[i] << 8 * i;
     }
 
     return r;
 }
 
 /*************************************************
  * Name:        store64
  *
  * Description: Store a 64-bit integer to a byte array in little-endian order
  *
  * Arguments:   - uint8_t *x: pointer to the output byte array
  *              - uint64_t u: input 64-bit unsigned integer
  **************************************************/
 static void store64(uint8_t *x, uint64_t u) {
     for (size_t i = 0; i < 8; ++i) {
         x[i] = (uint8_t) (u >> 8 * i);
     }
 }
 
 static void KeccakF1600_StatePermute(uint64_t *state) {
 
     uint32_t Abe_low;
     uint32_t Abe_high;
 
     //unsigned int cycles;
     //printf("Hiiii");
 
     //CSR_CLEAR_BITS(CSR_REG_MCOUNTINHIBIT, 0x1);
     //CSR_WRITE(CSR_REG_MCYCLE, 0);
 
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
 
     //CSR_READ(CSR_REG_MCYCLE, &cycles);
     //printf("Number of clock cycles : %d\n", cycles);
 
 }
 
 /*************************************************
  * Name:        keccak_absorb
  *
  * Description: Absorb step of Keccak;
  *              non-incremental, starts by zeroeing the state.
  *
  * Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
  *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
  *              - const uint8_t *m: pointer to input to be absorbed into s
  *              - size_t mlen: length of input in bytes
  *              - uint8_t p: domain-separation byte for different
  *                                 Keccak-derived functions
  **************************************************/
 static void keccak_absorb(uint64_t *s, uint32_t r, const uint8_t *m,
                           size_t mlen, uint8_t p) {
     size_t i;
     uint8_t t[200];
 
     /* Zero state */
     for (i = 0; i < 25; ++i) {
         s[i] = 0;
     }
 
     while (mlen >= r) {
         for (i = 0; i < r / 8; ++i) {
             s[i] ^= load64(m + 8 * i);
         }
 
         KeccakF1600_StatePermute(s);
         mlen -= r;
         m += r;
     }
 
     for (i = 0; i < r; ++i) {
         t[i] = 0;
     }
     for (i = 0; i < mlen; ++i) {
         t[i] = m[i];
     }
     t[i] = p;
     t[r - 1] |= 128;
     for (i = 0; i < r / 8; ++i) {
         s[i] ^= load64(t + 8 * i);
     }
 }
 
 /*************************************************
  * Name:        keccak_squeezeblocks
  *
  * Description: Squeeze step of Keccak. Squeezes full blocks of r bytes each.
  *              Modifies the state. Can be called multiple times to keep
  *              squeezing, i.e., is incremental.
  *
  * Arguments:   - uint8_t *h: pointer to output blocks
  *              - size_t nblocks: number of blocks to be
  *                                                squeezed (written to h)
  *              - uint64_t *s: pointer to input/output Keccak state
  *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
  **************************************************/
 static void keccak_squeezeblocks(uint8_t *h, size_t nblocks,
                                  uint64_t *s, uint32_t r) {
     while (nblocks > 0) {
         KeccakF1600_StatePermute(s);
         for (size_t i = 0; i < (r >> 3); i++) {
             store64(h + 8 * i, s[i]);
         }
         h += r;
         nblocks--;
     }
 }
 
 /*************************************************
  * Name:        keccak_inc_init
  *
  * Description: Initializes the incremental Keccak state to zero.
  *
  * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
  *                First 25 values represent Keccak state.
  *                26th value represents either the number of absorbed bytes
  *                that have not been permuted, or not-yet-squeezed bytes.
  **************************************************/
 static void keccak_inc_init(uint64_t *s_inc) {
     size_t i;
 
     for (i = 0; i < 25; ++i) {
         s_inc[i] = 0;
     }
     s_inc[25] = 0;
 }
 
 /*************************************************
  * Name:        keccak_inc_absorb
  *
  * Description: Incremental keccak absorb
  *              Preceded by keccak_inc_init, succeeded by keccak_inc_finalize
  *
  * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
  *                First 25 values represent Keccak state.
  *                26th value represents either the number of absorbed bytes
  *                that have not been permuted, or not-yet-squeezed bytes.
  *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
  *              - const uint8_t *m: pointer to input to be absorbed into s
  *              - size_t mlen: length of input in bytes
  **************************************************/
 static void keccak_inc_absorb(uint64_t *s_inc, uint32_t r, const uint8_t *m,
                               size_t mlen) {
     size_t i;
 
     /* Recall that s_inc[25] is the non-absorbed bytes xored into the state */
     while (mlen + s_inc[25] >= r) {
         for (i = 0; i < r - (uint32_t)s_inc[25]; i++) {
             /* Take the i'th byte from message
                xor with the s_inc[25] + i'th byte of the state; little-endian */
             s_inc[(s_inc[25] + i) >> 3] ^= (uint64_t)m[i] << (8 * ((s_inc[25] + i) & 0x07));
         }
         mlen -= (size_t)(r - s_inc[25]);
         m += r - s_inc[25];
         s_inc[25] = 0;
 
         KeccakF1600_StatePermute(s_inc);
     }
 
     for (i = 0; i < mlen; i++) {
         s_inc[(s_inc[25] + i) >> 3] ^= (uint64_t)m[i] << (8 * ((s_inc[25] + i) & 0x07));
     }
     s_inc[25] += mlen;
 }
 
 /*************************************************
  * Name:        keccak_inc_finalize
  *
  * Description: Finalizes Keccak absorb phase, prepares for squeezing
  *
  * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
  *                First 25 values represent Keccak state.
  *                26th value represents either the number of absorbed bytes
  *                that have not been permuted, or not-yet-squeezed bytes.
  *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
  *              - uint8_t p: domain-separation byte for different
  *                                 Keccak-derived functions
  **************************************************/
 static void keccak_inc_finalize(uint64_t *s_inc, uint32_t r, uint8_t p) {
     /* After keccak_inc_absorb, we are guaranteed that s_inc[25] < r,
        so we can always use one more byte for p in the current state. */
     s_inc[s_inc[25] >> 3] ^= (uint64_t)p << (8 * (s_inc[25] & 0x07));
     s_inc[(r - 1) >> 3] ^= (uint64_t)128 << (8 * ((r - 1) & 0x07));
     s_inc[25] = 0;
 }
 
 /*************************************************
  * Name:        keccak_inc_squeeze
  *
  * Description: Incremental Keccak squeeze; can be called on byte-level
  *
  * Arguments:   - uint8_t *h: pointer to output bytes
  *              - size_t outlen: number of bytes to be squeezed
  *              - uint64_t *s_inc: pointer to input/output incremental state
  *                First 25 values represent Keccak state.
  *                26th value represents either the number of absorbed bytes
  *                that have not been permuted, or not-yet-squeezed bytes.
  *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
  **************************************************/
 static void keccak_inc_squeeze(uint8_t *h, size_t outlen,
                                uint64_t *s_inc, uint32_t r) {
     size_t i;
 
     /* First consume any bytes we still have sitting around */
     for (i = 0; i < outlen && i < s_inc[25]; i++) {
         /* There are s_inc[25] bytes left, so r - s_inc[25] is the first
            available byte. We consume from there, i.e., up to r. */
         h[i] = (uint8_t)(s_inc[(r - s_inc[25] + i) >> 3] >> (8 * ((r - s_inc[25] + i) & 0x07)));
     }
     h += i;
     outlen -= i;
     s_inc[25] -= i;
 
     /* Then squeeze the remaining necessary blocks */
     while (outlen > 0) {
         KeccakF1600_StatePermute(s_inc);
 
         for (i = 0; i < outlen && i < r; i++) {
             h[i] = (uint8_t)(s_inc[i >> 3] >> (8 * (i & 0x07)));
         }
         h += i;
         outlen -= i;
         s_inc[25] = r - i;
     }
 }
 
 void shake128_inc_init(shake128incctx *state) {
     state->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (state->ctx == NULL) {
         exit(111);
     }
     keccak_inc_init(state->ctx);
 }
 
 void shake128_inc_absorb(shake128incctx *state, const uint8_t *input, size_t inlen) {
     keccak_inc_absorb(state->ctx, SHAKE128_RATE, input, inlen);
 }
 
 void shake128_inc_finalize(shake128incctx *state) {
     keccak_inc_finalize(state->ctx, SHAKE128_RATE, 0x1F);
 }
 
 void shake128_inc_squeeze(uint8_t *output, size_t outlen, shake128incctx *state) {
     keccak_inc_squeeze(output, outlen, state->ctx, SHAKE128_RATE);
 }
 
 void shake128_inc_ctx_clone(shake128incctx *dest, const shake128incctx *src) {
     dest->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (dest->ctx == NULL) {
         exit(111);
     }
     memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
 }
 
 void shake128_inc_ctx_release(shake128incctx *state) {
     free(state->ctx);
 }
 
 void shake256_inc_init(shake256incctx *state) {
     state->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (state->ctx == NULL) {
         exit(111);
     }
     keccak_inc_init(state->ctx);
 }
 
 void shake256_inc_absorb(shake256incctx *state, const uint8_t *input, size_t inlen) {
     keccak_inc_absorb(state->ctx, SHAKE256_RATE, input, inlen);
 }
 
 void shake256_inc_finalize(shake256incctx *state) {
     keccak_inc_finalize(state->ctx, SHAKE256_RATE, 0x1F);
 }
 
 void shake256_inc_squeeze(uint8_t *output, size_t outlen, shake256incctx *state) {
     keccak_inc_squeeze(output, outlen, state->ctx, SHAKE256_RATE);
 }
 
 void shake256_inc_ctx_clone(shake256incctx *dest, const shake256incctx *src) {
     dest->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (dest->ctx == NULL) {
         exit(111);
     }
     memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
 }
 
 void shake256_inc_ctx_release(shake256incctx *state) {
     free(state->ctx);
 }
 
 /*************************************************
  * Name:        shake128_absorb
  *
  * Description: Absorb step of the SHAKE128 XOF.
  *              non-incremental, starts by zeroeing the state.
  *
  * Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
  *              - const uint8_t *input: pointer to input to be absorbed
  *                                            into s
  *              - size_t inlen: length of input in bytes
  **************************************************/
 void shake128_absorb(shake128ctx *state, const uint8_t *input, size_t inlen) {
     state->ctx = malloc(PQC_SHAKECTX_BYTES);
     if (state->ctx == NULL) {
         exit(111);
     }
     keccak_absorb(state->ctx, SHAKE128_RATE, input, inlen, 0x1F);
 }
 
 /*************************************************
  * Name:        shake128_squeezeblocks
  *
  * Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of
  *              SHAKE128_RATE bytes each. Modifies the state. Can be called
  *              multiple times to keep squeezing, i.e., is incremental.
  *
  * Arguments:   - uint8_t *output: pointer to output blocks
  *              - size_t nblocks: number of blocks to be squeezed
  *                                            (written to output)
  *              - shake128ctx *state: pointer to input/output Keccak state
  **************************************************/
 void shake128_squeezeblocks(uint8_t *output, size_t nblocks, shake128ctx *state) {
     keccak_squeezeblocks(output, nblocks, state->ctx, SHAKE128_RATE);
 }
 
 void shake128_ctx_clone(shake128ctx *dest, const shake128ctx *src) {
     dest->ctx = malloc(PQC_SHAKECTX_BYTES);
     if (dest->ctx == NULL) {
         exit(111);
     }
     memcpy(dest->ctx, src->ctx, PQC_SHAKECTX_BYTES);
 }
 
 /** Release the allocated state. Call only once. */
 void shake128_ctx_release(shake128ctx *state) {
     free(state->ctx);
 }
 
 /*************************************************
  * Name:        shake256_absorb
  *
  * Description: Absorb step of the SHAKE256 XOF.
  *              non-incremental, starts by zeroeing the state.
  *
  * Arguments:   - shake256ctx *state: pointer to (uninitialized) output Keccak state
  *              - const uint8_t *input: pointer to input to be absorbed
  *                                            into s
  *              - size_t inlen: length of input in bytes
  **************************************************/
 void shake256_absorb(shake256ctx *state, const uint8_t *input, size_t inlen) {
     state->ctx = malloc(PQC_SHAKECTX_BYTES);
     if (state->ctx == NULL) {
         exit(111);
     }
     keccak_absorb(state->ctx, SHAKE256_RATE, input, inlen, 0x1F);
 }
 
 /*************************************************
  * Name:        shake256_squeezeblocks
  *
  * Description: Squeeze step of SHAKE256 XOF. Squeezes full blocks of
  *              SHAKE256_RATE bytes each. Modifies the state. Can be called
  *              multiple times to keep squeezing, i.e., is incremental.
  *
  * Arguments:   - uint8_t *output: pointer to output blocks
  *              - size_t nblocks: number of blocks to be squeezed
  *                                (written to output)
  *              - shake256ctx *state: pointer to input/output Keccak state
  **************************************************/
 void shake256_squeezeblocks(uint8_t *output, size_t nblocks, shake256ctx *state) {
     keccak_squeezeblocks(output, nblocks, state->ctx, SHAKE256_RATE);
 }
 
 void shake256_ctx_clone(shake256ctx *dest, const shake256ctx *src) {
     dest->ctx = malloc(PQC_SHAKECTX_BYTES);
     if (dest->ctx == NULL) {
         exit(111);
     }
     memcpy(dest->ctx, src->ctx, PQC_SHAKECTX_BYTES);
 }
 
 /** Release the allocated state. Call only once. */
 void shake256_ctx_release(shake256ctx *state) {
     free(state->ctx);
 }
 
 /*************************************************
  * Name:        shake128
  *
  * Description: SHAKE128 XOF with non-incremental API
  *
  * Arguments:   - uint8_t *output: pointer to output
  *              - size_t outlen: requested output length in bytes
  *              - const uint8_t *input: pointer to input
  *              - size_t inlen: length of input in bytes
  **************************************************/
 void shake128(uint8_t *output, size_t outlen,
               const uint8_t *input, size_t inlen) {
     size_t nblocks = outlen / SHAKE128_RATE;
     uint8_t t[SHAKE128_RATE];
     shake128ctx s;
 
     shake128_absorb(&s, input, inlen);
     shake128_squeezeblocks(output, nblocks, &s);
 
     output += nblocks * SHAKE128_RATE;
     outlen -= nblocks * SHAKE128_RATE;
 
     if (outlen) {
         shake128_squeezeblocks(t, 1, &s);
         for (size_t i = 0; i < outlen; ++i) {
             output[i] = t[i];
         }
     }
     shake128_ctx_release(&s);
 }
 
 /*************************************************
  * Name:        shake256
  *
  * Description: SHAKE256 XOF with non-incremental API
  *
  * Arguments:   - uint8_t *output: pointer to output
  *              - size_t outlen: requested output length in bytes
  *              - const uint8_t *input: pointer to input
  *              - size_t inlen: length of input in bytes
  **************************************************/
 void shake256(uint8_t *output, size_t outlen,
               const uint8_t *input, size_t inlen) {
     size_t nblocks = outlen / SHAKE256_RATE;
     uint8_t t[SHAKE256_RATE];
     shake256ctx s;
 
     shake256_absorb(&s, input, inlen);
     shake256_squeezeblocks(output, nblocks, &s);
 
     output += nblocks * SHAKE256_RATE;
     outlen -= nblocks * SHAKE256_RATE;
 
     if (outlen) {
         shake256_squeezeblocks(t, 1, &s);
         for (size_t i = 0; i < outlen; ++i) {
             output[i] = t[i];
         }
     }
     shake256_ctx_release(&s);
 }
 
 void sha3_256_inc_init(sha3_256incctx *state) {
     state->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (state->ctx == NULL) {
         exit(111);
     }
     keccak_inc_init(state->ctx);
 }
 
 void sha3_256_inc_ctx_clone(sha3_256incctx *dest, const sha3_256incctx *src) {
     dest->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (dest->ctx == NULL) {
         exit(111);
     }
     memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
 }
 
 void sha3_256_inc_ctx_release(sha3_256incctx *state) {
     free(state->ctx);
 }
 
 void sha3_256_inc_absorb(sha3_256incctx *state, const uint8_t *input, size_t inlen) {
     keccak_inc_absorb(state->ctx, SHA3_256_RATE, input, inlen);
 }
 
 void sha3_256_inc_finalize(uint8_t *output, sha3_256incctx *state) {
     uint8_t t[SHA3_256_RATE];
     keccak_inc_finalize(state->ctx, SHA3_256_RATE, 0x06);
 
     keccak_squeezeblocks(t, 1, state->ctx, SHA3_256_RATE);
 
     sha3_256_inc_ctx_release(state);
 
     for (size_t i = 0; i < 32; i++) {
         output[i] = t[i];
     }
 }
 
 /*************************************************
  * Name:        sha3_256
  *
  * Description: SHA3-256 with non-incremental API
  *
  * Arguments:   - uint8_t *output:      pointer to output
  *              - const uint8_t *input: pointer to input
  *              - size_t inlen:   length of input in bytes
  **************************************************/
 void sha3_256(uint8_t *output, const uint8_t *input, size_t inlen) {
     uint64_t s[25];
     uint8_t t[SHA3_256_RATE];
 
     /* Absorb input */
     keccak_absorb(s, SHA3_256_RATE, input, inlen, 0x06);
 
     /* Squeeze output */
     keccak_squeezeblocks(t, 1, s, SHA3_256_RATE);
 
     for (size_t i = 0; i < 32; i++) {
         output[i] = t[i];
     }
 }
 
 void sha3_384_inc_init(sha3_384incctx *state) {
     state->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (state->ctx == NULL) {
         exit(111);
     }
     keccak_inc_init(state->ctx);
 }
 
 void sha3_384_inc_ctx_clone(sha3_384incctx *dest, const sha3_384incctx *src) {
     dest->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (dest->ctx == NULL) {
         exit(111);
     }
     memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
 }
 
 void sha3_384_inc_absorb(sha3_384incctx *state, const uint8_t *input, size_t inlen) {
     keccak_inc_absorb(state->ctx, SHA3_384_RATE, input, inlen);
 }
 
 void sha3_384_inc_ctx_release(sha3_384incctx *state) {
     free(state->ctx);
 }
 
 void sha3_384_inc_finalize(uint8_t *output, sha3_384incctx *state) {
     uint8_t t[SHA3_384_RATE];
     keccak_inc_finalize(state->ctx, SHA3_384_RATE, 0x06);
 
     keccak_squeezeblocks(t, 1, state->ctx, SHA3_384_RATE);
 
     sha3_384_inc_ctx_release(state);
 
     for (size_t i = 0; i < 48; i++) {
         output[i] = t[i];
     }
 }
 
 /*************************************************
  * Name:        sha3_384
  *
  * Description: SHA3-256 with non-incremental API
  *
  * Arguments:   - uint8_t *output:      pointer to output
  *              - const uint8_t *input: pointer to input
  *              - size_t inlen:   length of input in bytes
  **************************************************/
 void sha3_384(uint8_t *output, const uint8_t *input, size_t inlen) {
     uint64_t s[25];
     uint8_t t[SHA3_384_RATE];
 
     /* Absorb input */
     keccak_absorb(s, SHA3_384_RATE, input, inlen, 0x06);
 
     /* Squeeze output */
     keccak_squeezeblocks(t, 1, s, SHA3_384_RATE);
 
     for (size_t i = 0; i < 48; i++) {
         output[i] = t[i];
     }
 }
 
 void sha3_512_inc_init(sha3_512incctx *state) {
     state->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (state->ctx == NULL) {
         exit(111);
     }
     keccak_inc_init(state->ctx);
 }
 
 void sha3_512_inc_ctx_clone(sha3_512incctx *dest, const sha3_512incctx *src) {
     dest->ctx = malloc(PQC_SHAKEINCCTX_BYTES);
     if (dest->ctx == NULL) {
         exit(111);
     }
     memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
 }
 
 void sha3_512_inc_absorb(sha3_512incctx *state, const uint8_t *input, size_t inlen) {
     keccak_inc_absorb(state->ctx, SHA3_512_RATE, input, inlen);
 }
 
 void sha3_512_inc_ctx_release(sha3_512incctx *state) {
     free(state->ctx);
 }
 
 void sha3_512_inc_finalize(uint8_t *output, sha3_512incctx *state) {
     uint8_t t[SHA3_512_RATE];
     keccak_inc_finalize(state->ctx, SHA3_512_RATE, 0x06);
 
     keccak_squeezeblocks(t, 1, state->ctx, SHA3_512_RATE);
 
     sha3_512_inc_ctx_release(state);
 
     for (size_t i = 0; i < 64; i++) {
         output[i] = t[i];
     }
 }
 
 /*************************************************
  * Name:        sha3_512
  *
  * Description: SHA3-512 with non-incremental API
  *
  * Arguments:   - uint8_t *output:      pointer to output
  *              - const uint8_t *input: pointer to input
  *              - size_t inlen:   length of input in bytes
  **************************************************/
 void sha3_512(uint8_t *output, const uint8_t *input, size_t inlen) {
     uint64_t s[25];
     uint8_t t[SHA3_512_RATE];
 
     /* Absorb input */
     keccak_absorb(s, SHA3_512_RATE, input, inlen, 0x06);
 
     /* Squeeze output */
     keccak_squeezeblocks(t, 1, s, SHA3_512_RATE);
 
     for (size_t i = 0; i < 64; i++) {
         output[i] = t[i];
     }
 }
 
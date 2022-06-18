/**
 * Yong Da Li
 * Saturday, June 4, 2022
 * 
 * section 5.4.3.2.2 Length-Limited Golomb-Power-of-2 Codeword of CCSDS123 standard
 * encoder
 */

#ifndef ENCODER_H
#define ENCODER_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

  
// Yong Da's naive implementation
unsigned int encode_sample(unsigned int sample, unsigned int k);

/**
 * way more optimized implementation of encoder
 * https://michaeldipperstein.github.io/rice.html
 * @param  sample        [thing to be compressed]
 * @param  k             [divisor = M = 2^k, to control size of divisor]
 * @param  num_bits_used [how many bits of 32 bit integer is actually used in the compressed sample]
 * @return               [compressed value, stored as 32-bit unsigned integer]
 */
uint32_t encode_sample_optimized(uint32_t sample, unsigned int k, unsigned int* num_bits_used);

/**
 * decoder assumes input is 32 bit unsigned integer
 * and it'll find the leading 1, to figure out when does the compressed sample actually start within the 32 bits
 * @param  code [encoded sample]
 * @param  k    [control parameter for size of divisor, must know the k value used for encoding]
 * @return      [decoded 32-bit unsigned sample]
 */
uint32_t decode_sample(uint32_t code, unsigned int k);

#endif // ENCODER_H
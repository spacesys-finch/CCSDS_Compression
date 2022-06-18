/**
 * Yong Da Li
 * Saturday, June 4, 2022
 * 
 * section 5.4.3.2.2 Length-Limited Golomb-Power-of-2 Codeword of CCSDS123 standard
 * encoder
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "main.h"
#include "encoder.h"

void check_single_encode(void){
    unsigned int sample = 1234;
    unsigned int k = 6;
    unsigned int num_bits_used;
    // unsigned int encoded = encode_sample(sample, k);
    unsigned int encoded = encode_sample_optimized(sample, k, &num_bits_used);

    printf("==== encode ====\n");
    printf("---- parameters ----\n");
    printf("Golomb power of 2 [k]: %u\n", k);
    printf("divisor [M]: %u\n", (unsigned int) pow(2,k));


    printf("---- encoding ----\n");
    printf("sample: dec = %u, \tbin = "\
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN \
        ", \thex = %x\n", sample, \
        BYTE_TO_BINARY(sample>>24), \
        BYTE_TO_BINARY(sample>>16), \
        BYTE_TO_BINARY(sample>>8), \
        BYTE_TO_BINARY(sample), sample);

    printf("encoded: \t\tbin = "\
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN \
        ", \thex = %x\n", \
        BYTE_TO_BINARY(encoded>>24), \
        BYTE_TO_BINARY(encoded>>16), \
        BYTE_TO_BINARY(encoded>>8), \
        BYTE_TO_BINARY(encoded), encoded);
    printf("input sample uses %d bits\n", (int)sizeof(sample)*8);
    printf("encoded sample uses %d bits\n", num_bits_used);
}

// TO DO
void check_single_decode(void){
    /**
     * ===== parameters =====
     * Golomb power of 2 [k]: 4
     * divisor [M]: 16
     * ===== encoding =====
     * sample: decimal = 18,   bin = 00000000 00000000 00000000 00010010,      hex = 12
     * encoded:                bin = 00100010,         hex = 22
     */
    uint32_t code = 0x3ffff92;
    unsigned int k = 6;

    printf("===== decode =====\n");
    printf("code: hex = %X, \tbin = "\
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN " " \
        BYTE_TO_BINARY_PATTERN \
        ", \thex = %x\n", code, \
        BYTE_TO_BINARY(code>>24), \
        BYTE_TO_BINARY(code>>16), \
        BYTE_TO_BINARY(code>>8), \
        BYTE_TO_BINARY(code), code);   

    printf("decoded: %d\n", decode_sample(code, k));
}

int main(void){
    check_single_encode();
    check_single_decode();
    
    return 0;
}
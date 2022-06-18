/**
 * Yong Da Li
 * Saturday, June 4, 2022
 * 
 * section 5.4.3.2.2 Length-Limited Golomb-Power-of-2 Codeword of CCSDS123 standard
 * encoder
 */

#include <stdint.h>
#include "encoder.h"


/**
 * returns the GOP2 code word for an unsigned integer as a char array 
 * @param  sample 	- input integer
 * @param  k		- sample/2**k parameter
 * @return 			- char array of GOP2 code word
 */
unsigned int encode_sample(unsigned int sample, unsigned int k){
	unsigned int ret;
	unsigned int unary = 0;	// unary portion, pad with 1's so initialize to 0
	unsigned int binary;	// binary portion

	// compute portions needed
	unsigned int divisor = (int) pow(2,k);
	unsigned int quotient = sample / divisor;
	unsigned int remainder = sample % divisor;

	/* debug messages
	printf("sample: %u\n", sample);
	printf("k: %u\n", k);
	printf("divisor: %u\n", divisor);
	printf("quotient: %u\n", quotient);
	printf("remainder: %u\n", remainder);
	*/

	// unary portion --> pad with 1's and stop character 0, since it makes debugging easier
	// 					can easily find start of unary portion in integer, since it's leading 1's
	while (quotient > 0){
		unary = (unary | 0x1) << 1;	// pad with 1's
		quotient--;
	}
	// don't need to add stop character since unary was initialized with 0's


	// binary portion
	unsigned int binary_bitmask = 0;
	int k_copy = k;
	while (k_copy>0){	// create bit mask
		binary_bitmask = (binary_bitmask << 1 | 0x1);
		k_copy--;
	}
	binary = remainder & binary_bitmask; // create binary portion


	// combine
	ret = (unary<<k) | binary; // length of bitmask is k

	/* debug messages
	printf("unary: \t\t"BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(unary));
	printf("binary: \t"BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(binary));
	*/

	return ret;
}

uint32_t encode_sample_optimized(uint32_t sample, unsigned int k, unsigned int* num_bits_used){
	uint32_t ret;
	unsigned int M = (int) pow(2,k);
	unsigned int quotient;
	unsigned int remainder;
	unsigned int unary = 0;

	// calculate using bitwise operations
	quotient = sample >> k;
	remainder = sample & (M-1);

	// number of bits needed
	// quotient in unary + 1 for unary stop character + k bits for remainder
	// 		- actually don't need this since the output is like: 00100010
	// 		- go by "first 1", that's the start of the sample, stored in 32 bit integer
	*num_bits_used = quotient + 1 + k;

	// create unary encoding of quotient
	while (quotient > 0){
		unary = (unary | 0x1) << 1;	// pad with 1's
		quotient--;
	}	

	// combine
	ret = (unary << k) | remainder;

	return ret;
}


uint32_t decode_sample(uint32_t code, unsigned int k){
	uint32_t ret;
	int i = 32; // indexing variable, start at bit 32 (left edge) and work backwards
	uint32_t quotient = 0; 
	uint32_t remainder = 0;

	// structure: 0000001111011
	// 			  ^^^^^^ 		not encoded values, pading
	// 			        ^^^^^	unary value + stop character 0
	// 			             ^^ last k bits are remainder 

	// find the leading 1
	while( (code & (1<<i)) >> i != 1 ){
		i--;
	}

	// count number of 1's in unary, until stop character 0
	while ( (code & (1<<i)) >> i == 1 ){
		i--;
		quotient++;
	}

	// want to extract last k bits
	// shift all to left, then shift right to pull out leading 0's
	remainder = code << (32-k);
	remainder = remainder >> (32-k);

	// combine
	ret = (int) pow(2,k) * quotient + remainder;

	return ret;
}
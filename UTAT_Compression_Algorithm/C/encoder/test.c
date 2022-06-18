#include <assert.h>
#include "encoder.h"

/**
 * running make target `make test` should have no output
 * program will fail out if any of the asserts fail
 */
int main(void){
	assert(encode_sample(14u,2u) == 0x3a);
	assert(encode_sample(14u,3u) == 0x16);
	assert(encode_sample(7u,2u) == 0xb);
	return 0;
}
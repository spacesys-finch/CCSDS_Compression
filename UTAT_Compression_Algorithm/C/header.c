#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "header.h"

/*
Adds provided 'value' to bitstream of size 'size', taking up 'width' number of bits.
Inputs: 
    value:      Value to be encoded in bitstream
    bitstream:  Arrray of 64 bit values of encoded values
    size:       Size of array
    width:      Size in bits of value
Output:
    N/A
*/
void add_to_bitstream(uint16_t value, uint64_t* bitstream, uint16_t size, uint8_t width)
{
    uint64_t temp = 0;
    for(int i = 0; i < size; ++i)
    {
        temp = UINT64_MAX << (64 - width); //Create bitmask
        temp = temp & bitstream[i];        //Grab the MSBs that overflow
        bitstream[i] = bitstream[i] << width;
        bitstream[i] = bitstream[i] | value;
        value = temp >> (64 - width);
    }
}

/*
Decodes and removes the most recently added value from bitstream
Inputs:
    bitstream:  Arrray of 64 bit values of encoded values
    size:       Size of array
    width:      Size in bits of value to be decoded
Outputs:
    16 bit decoded value
*/
uint16_t decode_bitstream(uint64_t* bitstream, uint16_t size, uint8_t width)
{
    uint64_t temp = 0;
    uint64_t value = 0;
    for(int i = size - 1; i >=0 ; --i)
    {  
        temp = UINT64_MAX >> (64 - width); //Create bitmask
        temp = temp & bitstream[i];        //Grab the LSBs that overflow
        bitstream[i] = bitstream[i] >> width;
        bitstream[i] = bitstream[i] | value; 
        value = temp << (64 - width);
    }

    return (uint16_t) (value >> (64 - width));
}

/*
Encodes Image Metadata into bitstream and writes to bin file. Requires the total number of bytes to be known. 
Inputs:
    params:     Pointer to struct holding all necessary Image Metadata values to be encoded
    size:       Total number of 64 bit variables (Number of bytes / 4) needed to encode Image Metadata values
Output:
    Pointer to encoded bitstream
*/
uint64_t* encodeimageMetadata(imageMetadata* params, uint16_t size)
{
    uint16_t* cursor = (uint16_t*) params;
    uint64_t* bitstream = calloc(size + 1, sizeof(uint64_t));
    for(int i = 0; i <= 12; ++i)
    {
        //printf("%d\n", *(cursor + i));
        add_to_bitstream(*(cursor + i), bitstream, 3, imageMetadataWidths[i]);
        //printf("%llx %llx %llx\n",bitstream[2], bitstream[1], bitstream[0]);
    }
    printf("%016llx %016llx %016llx\n",bitstream[2], bitstream[1], bitstream[0]);
    
    //Write bitstream to bin file
    FILE *fptr;
    fptr = fopen("output/encoded.bin","wb");  // w for write, b for binary
    fwrite(bitstream, size, 8, fptr);
    fclose(fptr);
    
    return bitstream;
}

/*
Reads bitstream from bin file and populates empty Image Metadata struct.
Inputs:
    params:     Emtpy Image Metadata struct
    size:       otal number of 64 bit variables (Number of bytes / 4) used to encode Image Metadata values
Output:
    N/A
*/
void decodeImageMetadata(imageMetadata* params, uint16_t size)
{
    //Read bitstream from bin file
    uint64_t* bitstream = calloc(size + 1, sizeof(uint64_t));
    FILE *fptr;
    fptr = fopen("output/encoded.bin", "rb");
    fread(bitstream, size, 8, fptr);
    fclose(fptr);

    printf("%016llx %016llx %016llx\n",bitstream[2], bitstream[1], bitstream[0]);
    
    uint16_t* cursor = (uint16_t*) params;
    
    for(int i = 12; i >= 0; --i)
    {
        //printf("%d\n", *(cursor + i));
        uint16_t value = decode_bitstream(bitstream, 3, imageMetadataWidths[i]);
        //printf("Grabbed: %hu\n", value);
        *(cursor + i) = value;
    }
}

int main(void)
{
    //Create test imageMetadata
    printf("==== Image Metadata Test Unit ====\n");
    imageMetadata* params = calloc(sizeof(imageMetadata), 1);

    params->userDefined = 1;
    params->xSize = 434;
    params->ySize= 325;
    params->zSize= 3459;

    params->sampledType= 1;           //Binary value 0/1
    params->largeDynamicRangeFlag= 0; //Binary value 0/1
    params->dynamicRange = 1;          //Only 4 bits used

    params->sampledEncodingOrder = 0;     //Binary value 0/1
    params->subframeInterleavingDepth = 5;

    params->outputWordSize = 2;                 //Only 3 bits used
    params->entropyCoderType = 3;               //Only 2 bits used
    params->quantizerFidelityControlMethod = 2; //Only 2 bits used

    params->supplementaryInformationTableCount = 15; //Only 4 bits used

    uint64_t* data = encodeimageMetadata(params, 3);
    //printf("%016llx %016llx %016llx\n",data[2], data[1], data[0]);
    imageMetadata* params_empty = calloc(sizeof(imageMetadata), 1);
    decodeImageMetadata(params_empty, 3);
    if(memcmp(params_empty, params, 26) == 0)
        printf("Passed!\n");
    else
        printf("Failed!\n");

    return 0;
}
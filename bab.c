#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>


typedef struct BMP_HEADER {
    uint16_t id;
    uint32_t size;
    uint32_t reserved;
    uint32_t offset;
} __attribute__ ((packed)) BMP_HEADER;

typedef struct BMP {
    BMP_HEADER *bmp_header;
    void *image_data;
} BMP;

int main()
{
    char *in_file_name = "baboon.bmp";

    FILE *in_file;
    in_file = fopen(in_file_name, "rb");

    if(!in_file){
        printf("Error reading file: %s\n",in_file_name);
    }

    BMP_HEADER bmp_header;


    /* Read in the BMP header */
    fread(&bmp_header, sizeof(BMP_HEADER), 1, in_file);

    /* Check 'magic' */
    if(bmp_header.id != 0x4d42){
        printf("Unsupported file type: %x\n",bmp_header.id);
    }



    // TODO
    fseek(in_file, bmp_header.offset, SEEK_SET);
    BMP bmp = malloc(bmp_header.size);



    fclose(in_file);
    in_file = NULL;

    return 0;
}

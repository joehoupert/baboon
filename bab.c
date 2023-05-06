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

/* Pixel array, lives at BMP_HEADER.offset */
char *bitmap_image = NULL;

int main()
{

/* 
 *
 * Read in the bmp
 *
 */
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


    // Move pointer to start of pixel array
    fseek(in_file, bmp_header.offset, SEEK_SET);
    bitmap_image = (char *)malloc(bmp_header.size);

    /* NEVER TRUST A MALLOC */
    if(!bitmap_image){
        free(bitmap_image);
        bitmap_image = NULL;
        fclose(in_file);
        printf("Error malloc-ing\n");
        exit(-1);
    }

    fread(bitmap_image, bmp_header.size, 1, in_file);

    if(!bitmap_image){
        fclose(in_file);
        exit(-1);
    }

    fclose(in_file);
    in_file = NULL;


/*
 *
 * Write out a bitmap
 *
 */

    char *out_file_name = "baboon_out.bmp";

    FILE *out_file;
    out_file = fopen(out_file_name, "wb");

    if(!out_file){
        printf("Error getting file handle: %s\n",out_file_name);
    }


    /* Write the header back */
    fwrite(&bmp_header, sizeof(BMP_HEADER), 1, out_file);
    /* Adjust write head */
    fseek(out_file, bmp_header.offset, SEEK_SET);
    /* Write the image array */
    fwrite(bitmap_image, bmp_header.size - sizeof(BMP_HEADER), 1, out_file);


    return 0;
}

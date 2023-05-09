#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#define R_WT 0.2989
#define G_WT 0.5870
#define B_WT 0.1140


typedef struct BMP_HEADER {
    uint16_t id;
    uint32_t size;
    uint32_t reserved;
    uint32_t offset;
} __attribute__ ((packed)) BMP_HEADER;

typedef struct DIB {
    uint32_t size;
    uint32_t bmp_width_px;
    uint32_t bmp_height_px;
    uint16_t color_planes; //must be 1
    uint16_t color_depth; //bits per pixel
    uint32_t compression_type;
    uint32_t image_size;
    uint32_t horizontal_res;
    uint32_t vertical_res;
    uint32_t num_colors;
    uint32_t num_important_colors; //generally unused
} __attribute__ ((packed)) DIB;


void display_dib(DIB dib){
    printf("Bitmap Information Header \n");
    printf("Header size: %d\n", dib.size);
    printf("Width (px): %d\n", dib.bmp_width_px);
    printf("Height (px): %d\n", dib.bmp_height_px);
    printf("Color planes: %d\n", dib.color_planes);
    printf("Color depth: %d\n", dib.color_depth);
    printf("Compression: %d\n", dib.compression_type);
    printf("Image size: %d\n", dib.image_size);
    printf("Horizontal: %d\n", dib.horizontal_res);
    printf("Vertical: %d\n", dib.vertical_res);
    printf("Number of colors: %d\n", dib.num_colors);
    printf("Important colors: %d\n", dib.num_important_colors);

}

/* Pixel array, lives at BMP_HEADER.offset */
unsigned char *bitmap_image = NULL;

int main()
{

/* 
 * Read in the bmp header
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


/* 
 * Read in the DIB
 */

    DIB dib;

    fseek(in_file, sizeof(BMP_HEADER), SEEK_SET);
    fread(&dib, sizeof(DIB), 1, in_file);

    display_dib(dib);

    /* TODO Error checking? */



/* 
 * Read in the pixel array
 */
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
 * We know that baboon.bmp has compression type 0 (R,G,B)
 * We're also little endian, so every 3 bytes are B,G,R
 */
    int px = 0;
    int tmp;
    for(px = 0; px < dib.image_size; px+=3){
        tmp = (bitmap_image[px]*B_WT + 
               bitmap_image[px+1]*G_WT +
               bitmap_image[px+2]*R_WT) / 3;
        
        bitmap_image[px] = tmp;
        bitmap_image[px+1] = tmp;
        bitmap_image[px+2] = tmp;
    }

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
    fseek(out_file, sizeof(BMP_HEADER), SEEK_SET);

    /* Write the DIB back */
    fwrite(&dib, sizeof(DIB), 1, out_file);
    /* Adjust write head */
    fseek(out_file, bmp_header.offset, SEEK_SET);

    /* Write the image array */
    fwrite(bitmap_image, dib.image_size, 1, out_file);


    return 0;
}

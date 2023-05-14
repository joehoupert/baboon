#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdlib.h>
#include "bmp.h"

#define R_WT 0.2989
#define G_WT 0.5870
#define B_WT 0.1140


int main()
{
    bmp_image_t *b = NULL;

    /* 
     * Read in the bmp header
     */
    char *in_file_name = "baboon.bmp";
    char *err = NULL;

    FILE *in_file;
    in_file = fopen(in_file_name, "rb");

    if(!in_file){
        printf("Error reading file: %s\n",in_file_name);
    }

    b = read_bmp(in_file, &err);
    fclose(in_file);

    display_header(b->header);

    if(!b){
        printf("%s\n", err);
    }


/* 
 * We know that baboon.bmp has compression type 0 (R,G,B)
 * We're also little endian, so every 3 bytes are B,G,R
 */
    //int px = 0;
    //int tmp;
    //for(px = 0; px < b->header.image_size; px+=3){
    //    tmp = (b->px_array[px]*B_WT + 
    //           b->px_array[px+1]*G_WT +
    //           b->px_array[px+2]*R_WT) / 3;
    //    
    //    b->px_array[px] = tmp;
    //    b->px_array[px+1] = tmp;
    //    b->px_array[px+2] = tmp;
    //}

/*
 *
 * Write out a bitmap
 *
 */
    char *out_file_name = "baboon_grey.bmp";

    FILE *out_file;
    out_file = fopen(out_file_name, "wb");

    if(!out_file){
        printf("Error getting file handle: %s\n",out_file_name);
    }

    b = bmp_to_greyscale(b, &err);

    bool write_ok = write_bmp(out_file, b, &err);

    if(!write_ok)
    {
        /* TODO */
        printf("TODO: handle\n");
        fclose(out_file);
    }
    fclose(out_file);

    free(b);

    return 0;
}

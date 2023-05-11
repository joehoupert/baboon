#include <stdio.h>
#include <string.h>
#include "bmp.h"

bool check_bmp_header(bmp_header_t *bmp_header, FILE *file)
{
    if(bmp_header->id != 0x4d42) return false;
    if(bmp_header->offset != BMP_HEADER_SIZE) return false;
    if(bmp_header->dib_size != BMP_HEADER_SIZE) return false;
    if(bmp_header->color_planes != 1) return false;
    if(bmp_header->compression_type != 0) return false;
    if(bmp_header->num_colors != 0) return false;
    if(bmp_header->num_important_colors != 0) return false;
    if(bmp_header->color_depth != 24) return false;

    /* Check image size.  Padding ensures width is on a 4 byte boundary */
    int padding = (4 - (bmp_header->width_px * bmp_header->color_depth / 8) % 4) % 4;
    int width_bytes = bmp_header->width_px * bmp_header->color_depth / 8 + padding;
    int size = width_bytes * bmp_header->height_px;
    if(size != bmp_header->image_size) return false;


    /* Check size of input file */
    int input_size;
    fseek(file, 0, SEEK_END);
    input_size = ftell(file);
    fseek(file, 0, SEEK_SET);  //return things how you found them
    if(input_size != bmp_header-> size) return false;

    return true;
}

/*
 * malloc the bmp header
 * fread in the header
 * verify it using our check func
 * if ok, read in the pixel array
 * else clean up
 */
bmp_image_t *read_bmp(FILE *file, char **err)
{
    bmp_image_t *b;

    /*
     *
     * Read the header in, vet it
     * If its ok, read the rest of the pixel array in
     * 
     */
    size_t read = 0;
    read = fread(&b->header, sizeof(bmp_header_t), 1, file);
    /* TODO: check return val */
    printf("TODO: read %zi\n",read);

    bool header_ok = false;
    header_ok = check_bmp_header(&b->header, file);

    if(!header_ok){
        if(*err == NULL){
            char *msg = "ERROR: \"check_bmp_header\" failed";
            *err == malloc( (strlen(msg) + 1) * sizeof(**err) );
            strncpy(*err, msg, sizeof(*err));
        }
        b = NULL;
        return b;
    }


    size_t img_read = 0;
    b->px_array = (unsigned char *)malloc( b->header.image_size);

    /* NEVER TRUST A MALLOC */
    if(!b->px_array){
        free(b->px_array);
        b->px_array = NULL;

        if(*err == NULL){
            char *msg = "ERROR: \"malloc\" failed";
            *err == malloc( (strlen(msg) + 1) * sizeof(**err) );
            strncpy(*err, msg, sizeof(*err));
        }
        b = NULL;
        return b;
    }

    img_read = fread(&b->px_array, b->header.image_size, 1, file);


}


bool
write_bmp(FILE *file, bmp_image_t *bmp_image, char **err);

void
free_bmp(bmp_image_t *bmp_image);

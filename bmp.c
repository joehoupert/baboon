#include <stdio.h>
#include <string.h>
#include "bmp.h"


void display_header(bmp_header_t h){
    printf("Bitmap Information Header \n");
    printf("Header size: %d\n", h.dib_size);
    printf("Offset: %d\n", h.offset);
    printf("Width (px): %d\n", h.width_px);
    printf("Height (px): %d\n", h.height_px);
    printf("Color planes: %d\n", h.color_planes);
    printf("Color depth: %d\n", h.color_depth);
    printf("Compression: %d\n", h.compression_type);
    printf("Image size: %d\n", h.image_size);
    printf("Horizontal: %d\n", h.horizontal_res);
    printf("Vertical: %d\n", h.vertical_res);
    printf("Number of colors: %d\n", h.num_colors);
    printf("Important colors: %d\n", h.num_important_colors);
}


bool check_bmp_header(bmp_header_t *bmp_header, FILE *file)
{
    if(bmp_header->id != 0x4d42) return false;
    if(bmp_header->offset != BMP_HEADER_SIZE) return false;
    if(bmp_header->dib_size != DIB_HEADER_SIZE) return false;
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
 * Read in the header
 * Vet the header
 * if ok, malloc space for the pixel array
 * Set the read head to the px array and read it in
 * if ok, malloc space for the whole bmp_image_t
 * Assign yr pointers
 * return
 */
bmp_image_t *read_bmp(FILE *file, char **err)
{
    bmp_image_t *b = NULL;
    bmp_header_t h;
    bool header_ok = false;
    size_t img_read = 0;

    size_t header_read = 0;
    rewind(file);
    header_read = fread(&h, sizeof(bmp_header_t), 1, file);

    if(header_read != 1){
        if(*err == NULL)
        {
            char *msg = "ERROR: \"fread\" of header failed";
            *err = malloc( (strlen(msg) + 1) * sizeof(**err) );
            strncpy(*err, msg, sizeof(*err));
        }
        b = NULL;
        return b;
    }


    header_ok = check_bmp_header(&h, file);

    if(!header_ok)
    {
        if(*err == NULL)
        {
            char *msg = "ERROR: \"check_bmp_header\" failed";
            *err = malloc( (strlen(msg) + 1) * sizeof(**err) );
            strncpy(*err, msg, sizeof(*err));
        }
        b = NULL;
        return b;
    }

    unsigned char *px_array = (unsigned char *)malloc( h.image_size);

    /* Check that the malloc worked */
    if(!px_array){
        free(px_array);
        px_array = NULL;

        if(*err == NULL){
            char *msg = "ERROR: \"malloc\" failed";
            *err = malloc( (strlen(msg) + 1) * sizeof(**err) );
            strncpy(*err, msg, sizeof(*err));
        }
        b = NULL;
        return b;
    }

    /* Does fseek fail? */
    fseek(file, h.offset, SEEK_SET);

    img_read = fread(px_array, h.image_size, 1, file);

    if(img_read != 1)
    {
        if(*err == NULL)
        {
            char *msg = "ERROR: \"fread\" of pixel array failed";
            *err = malloc( (strlen(msg) + 1) * sizeof(**err) );
            strncpy(*err, msg, sizeof(*err));
        }
        b = NULL;
        return b;
    }

    /* Everything looks good */
    b = malloc(sizeof(*b));
    b->header = h;
    b->px_array = px_array;

    return b;
}


bool
write_bmp(FILE *file, bmp_image_t *bmp_image, char **err)
{
    rewind(file);


    /* 
     * We're going to write out the header first, then the pixel array
     * Why?
     * Because the whole bmp_image_t isn't in contiguous memory
     * See here:
     *printf("bmp_image: %p  bmp_header: %p\n",bmp_image, bmp_image->header);
     */

    /* Write out header */
    size_t header_written = 0;
    header_written = fwrite(&bmp_image->header, BMP_HEADER_SIZE, 1, file);

    /* TODO */
    printf("TODO: header_written = %zi\n",header_written);


    size_t img_written = 0;
    img_written = fwrite(bmp_image->px_array, bmp_image->header.image_size, 1, file);

    if(img_written != 1)
    {
        if(*err == NULL)
        {
            char *msg = "ERROR: \"fwrite\" failed";
            *err = malloc( (strlen(msg) + 1) * sizeof(**err) );
            strncpy(*err, msg, sizeof(*err));
        }
        return false;
    }

    return true;
}



void
free_bmp(bmp_image_t *bmp_image)
{
    free(bmp_image->px_array);
    free(bmp_image);
    return;
}

#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define R_WT 0.2989
#define G_WT 0.5870
#define B_WT 0.1140

#define BMP_HEADER_SIZE 54
#define DIB_HEADER_SIZE 40

typedef struct {
    uint16_t id;                /* Magic, must be 0x4d42 */
    uint32_t size;              /* Total file size, bytes */
    uint16_t reserved1;          /* Reserved */
    uint16_t reserved2;          /* Reserved */
    uint32_t offset;            /* Offset where the pixel array lives */
    /* Start of the DIB */
    uint32_t dib_size;          /* Its...the DIB size */
    uint32_t width_px;
    uint32_t height_px;
    uint16_t color_planes;      /* Must be 1 */
    uint16_t color_depth;       /* Bits per pixel, only support 24bit */
    uint32_t compression_type;  /* Should be 0 */
    uint32_t image_size;        /* Size in bytes of the pixel array */
    uint32_t horizontal_res;    /* In pixels per meter */
    uint32_t vertical_res;
    uint32_t num_colors;
    uint32_t num_important_colors;
} __attribute__ ((packed)) bmp_header_t;


/* Stores an entire BMP image */
typedef struct {
    bmp_header_t header;
    unsigned char *px_array;
} bmp_image_t;

/* Function Prototypes */

/*
 *  * Given an open file handle, read in a bmp image
 *   */
bmp_image_t*
read_bmp(FILE *file, char **err);

/* Given an open file handle, write out a bmp image */
bool
write_bmp(FILE *file, bmp_image_t *bmp_image, char **err);

/* Given an open file handle, validate the bmp_header_t (using our criteria) */
bool
check_bmp_header(bmp_header_t *bmp_header, FILE *file);

/* Free memory allocated by bmp_image_t */
void
free_bmp(bmp_image_t *bmp_image);

bmp_image_t *
bmp_to_greyscale(bmp_image_t *original, char **err);

void display_header(bmp_header_t h);


#endif /* bmp.h */

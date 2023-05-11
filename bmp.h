#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>

#define R_WT 0.2989
#define G_WT 0.5870
#define B_WT 0.1140

typedef struct BMP_HEADER {
    uint16_t id;                /* Magic, must be 0x4d42 */
    uint32_t size;              /* Total file size, bytes */
    uint16_t reserved;          /* Reserved */
    uint16_t reserved;          /* Reserved */
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
} __attribute__ ((packed)) BMP_HEADER;


/* Stores an entire BMP image */
typedef struct {
    BMP_HEADER header;
    unsigned char *px_array;
} BMP_IMAGE;

/* Function Prototypes */

/*
 *  * Given an open file handle, read in a bmp image
 *   */
BMP_IMAGE
read_file(FILE *file, char **err);

/* Given an open file handle, write out a bmp image */
bool
write_bmp(FILE *file, BMP_IMAGE *bmp_image, char **err);

/* Given an open file handle, validate the BMP_HEADER (using our criteria) */
bool
check_bmp_header(BMP_HEADER *bmp_header, FILE *file);

/* Free memory allocated by BMP_IMAGE */
void
free_bmp(BMP_IMAGE *bmp_image);


#endif /* bmp.h */

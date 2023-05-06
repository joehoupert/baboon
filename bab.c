#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


typedef struct BMP {
  uint16_t header;
  uint32_t size;
  uint32_t reserved;
  uint32_t offset;
} __attribute__ ((packed)) BMP;

typedef struct PIXEL {
  BMP *bmp;
  void *pixel_field;
} PIXEL;

int main()
{
  char *in_file_name = "baboon.bmp";

  FILE *in_file;
  in_file = fopen(in_file_name, "rb");

  if(!in_file){
    printf("Error reading file: %s\n",in_file_name);
  }

  BMP bmp;
  

  //size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  fread(&bmp, sizeof(BMP), 1, in_file);

  if(bmp.header != 0x4d42){
    printf("Unsupported file type: %x\n",bmp.header);
  }



// TODO
  fseek(in_file, sizeof(BMP), SEEK_SET);
  PIXEL px = malloc(bmp.size);



  fclose(in_file);
  in_file = NULL;

	return 0;
}

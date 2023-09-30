#include <stdio.h>
#include "DoctorByte.h"

int main() {
  printf("\n");

  DoctorByte docHead; 
  FILE * src = fopen("2.bmp", "rb");
  fread(&docHead, sizeof(docHead), 1, src);
  printf("Sig: %x \n", docHead.signature);
  printf("File size: %x \n", docHead.fileSize);
  printf("Res 1: %x \n", docHead.reserved1);
  printf("Res 2: %x \n", docHead.reserved2);
  printf("Offset: %x \n", docHead.dataOffset);

  fclose(src);
  printf("\n");
  return 0;
}

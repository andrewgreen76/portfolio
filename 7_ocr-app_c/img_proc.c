#include "img_proc.h"

void get_img_props(ProcList * pm, BmpHeader * imgh54)
{
  hmem_diag(pm->fname);
  
  printf("Caching image data from %s ...\n", pm->fname);
  FILE *srcbmp = fopen(pm->fname, "rb");
  fread(imgh54, sizeof(imgh54), 1, srcbmp);
  
  char loSigByte = (char)(imgh54->signature & 0xFF);
  char hiSigByte = (char)((imgh54->signature & 0xFF00) >>8);
  printf("Signature: %c%c\n", loSigByte, hiSigByte ); 
  printf("Total size of header: %d\n", imgh54->fileSize);
  printf("Reserved field 1: %04x\n", imgh54->reserved1);
  printf("Reserved field 2: %04x\n", imgh54->reserved2);
  printf("Data offset: %08x\n", imgh54->dataOffset);
  printf("Info header size: %08x\n", imgh54->headerSize);
  printf("Width: %d\n", imgh54->width);
  printf("Height: %d\n", imgh54->height);
  
  fclose(srcbmp);
  return;
}

void byte_check(char * fname, int bytei)
{
  FILE *fbyte = fopen(fname, "rb");
  unsigned char c;
  for(int i=0; i<bytei; i++) fread(&c, 1, 1, fbyte);
  printf("Byte check @ %d: %08x \n", bytei, c );
  fclose(fbyte);
  return;
}

void hmem_diag(char * fname)
{
  printf("Performing a memory diagnostic ...\n");
  FILE *scanObj = fopen(fname, "rb");
  unsigned char ch;

  for(int i=0 ; i<54 ; i++){
    if(i%16==0 && i!=0) printf("\n");
    fread(&ch, 1, 1, scanObj);
    printf("%02x ", (unsigned int)ch);
  }
  
  printf("\n");
  fclose(scanObj);
  return;
}

void proc_img()
{
    printf("Processing image ...\n");

    return;
}


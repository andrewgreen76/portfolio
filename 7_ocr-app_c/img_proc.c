#include "img_proc.h"
void get_img_props(ProcList * pm, BmpHeader * imgHdr)
{
  //hmem_diag(pm->name);
  
  printf("Caching image data from %s ...\n", pm->fname);

  FILE * src = fopen(pm->fname, "rb");
  fread(imgHdr, sizeof(imgHdr), 1, src);
  printf("Sig: %x \n", imgHdr->signature);
  printf("File size: %x \n", imgHdr->fileSize);
  printf("Res 1: %x \n", imgHdr->reserved1);
  printf("Res 2: %x \n", imgHdr->reserved2);
  printf("Offset: %x \n", imgHdr->dataOffset);
  fclose(src);
  
  hmem_diag(pm->fname);
  /*
  FILE *srcBmp = fopen(pm->fname, "rb");
  fread(imgHdr, sizeof(imgHdr), 1, srcBmp);
  printf("Signature: %x\n", imgHdr->signature ); 
  printf("Total size of header: %x\n", imgHdr->fileSize);
  printf("Reserved field 1: %x\n", imgHdr->reserved1);
  printf("Reserved field 2: %x\n", imgHdr->reserved2);
  printf("Data offset: %x\n", imgHdr->dataOffset);
  */
  /*
  char loSigByte = (char)(imgHdr->signature & 0xFF);
  char hiSigByte = (char)((imgHdr->signature & 0xFF00) >>8);
  printf("Signature: %c%c\n", loSigByte, hiSigByte ); 
  printf("Total size of header: %d\n", imgHdr->fileSize);
  printf("Reserved field 1: %x\n", imgHdr->reserved1);
  printf("Reserved field 2: %x\n", imgHdr->reserved2);
  printf("Data offset: %x\n", imgHdr->dataOffset);
  printf("Info header size: %x\n", imgHdr->headerSize);
  printf("Width: %d\n", imgHdr->width);
  printf("Height: %d\n", imgHdr->height);
  */
  //fclose(srcBmp);
  return;
}

unsigned char byte_check(char * fname, int readBytes) //readBytes: index -> numBytes ..
{
  readBytes++;                                        // .. here
  FILE *fbyte = fopen(fname, "rb");
  unsigned char c;
  for(int i=0; i<readBytes; i++) fread(&c, 1, 1, fbyte);
  printf("Byte check @ %d: %08x \n", readBytes, c );
  fclose(fbyte);
  return c;
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


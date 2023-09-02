#include "ocr_core.h"

void run_core_funcs(void)
{
    printf("Running core OCR and I/O functions ...\n");

    ProcList pm;
    BmpHeader bh;    
    get_fname(pm.fname);
    
    FILE * src = fopen(pm.fname, "rb");
    fread(&bh, sizeof(bh), 1, src);
    printf("Sig: %x \n", bh.signature);
    printf("File size: %x \n", bh.fileSize);
    printf("Res 1: %x \n", bh.reserved1);
    printf("Res 2: %x \n", bh.reserved2);
    printf("Offset: %x \n", bh.dataOffset);
    fclose(src);
    
    
    get_img_props(&pm, &bh);
    //get_bounded_vals();
    //proc_img();

    return;
}

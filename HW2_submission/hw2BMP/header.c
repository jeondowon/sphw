#include <stdio.h>
#include "bmp.h"

// 과제 1-1: BMP 파일의 헤더 내용을 화면에 출력한다.
int print_header(char *filename)
{
    FILE *fp;
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return -1;
    }

    if (read_header(fp, &fh, &ih) != 0) {
        printf("BMP 파일이 아닙니다: %s\n", filename);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    printf("===== BITMAP FILE HEADER =====\n");
    printf("bfType          : %c%c\n", fh.bfType & 0xFF, fh.bfType >> 8);
    printf("bfSize          : %u\n", fh.bfSize);
    printf("bfReserved1     : %u\n", fh.bfReserved1);
    printf("bfReserved2     : %u\n", fh.bfReserved2);
    printf("bfOffBits       : %u\n", fh.bfOffBits);

    printf("\n===== BITMAP INFO HEADER =====\n");
    printf("biSize          : %u\n", ih.biSize);
    printf("biWidth         : %d\n", ih.biWidth);
    printf("biHeight        : %d\n", ih.biHeight);
    printf("biPlanes        : %u\n", ih.biPlanes);
    printf("biBitCount      : %u\n", ih.biBitCount);
    printf("biCompression   : %u\n", ih.biCompression);
    printf("biSizeImage     : %u\n", ih.biSizeImage);
    printf("biXPelsPerMeter : %d\n", ih.biXPelsPerMeter);
    printf("biYPelsPerMeter : %d\n", ih.biYPelsPerMeter);
    printf("biClrUsed       : %u\n", ih.biClrUsed);
    printf("biClrImportant  : %u\n", ih.biClrImportant);

    return 0;
}

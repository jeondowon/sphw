#ifndef BMP_H
#define BMP_H

#include <stdio.h>

// 구조체 멤버 사이에 빈 공간이 생기지 않도록 1바이트 단위로 맞춘다.
// 이렇게 안 하면 BITMAPFILEHEADER가 14바이트가 아니라 16바이트가 되어서
// fread로 읽을 때 값이 밀린다.
#pragma pack(1)

// BMP 파일 헤더 (14바이트)
typedef struct{
    unsigned short bfType;      // 파일 종류, BM이어야 함
    unsigned int   bfSize;      // 파일 전체 크기
    unsigned short bfReserved1; // 사용 안 함 (0)
    unsigned short bfReserved2; // 사용 안 함 (0)
    unsigned int   bfOffBits;   // 픽셀 데이터(body)가 시작하는 위치
} BITMAPFILEHEADER;

// BMP 정보 헤더 (40바이트)
typedef struct{
    unsigned int   biSize;          // 정보 헤더 크기 (40)
    int            biWidth;         // 가로 픽셀 수
    int            biHeight;        // 세로 픽셀 수
    unsigned short biPlanes;        // 항상 1
    unsigned short biBitCount;      // 픽셀 하나의 비트 수 (24, 32 등)
    unsigned int   biCompression;   // 압축 방식 (0이면 압축 안 함)
    unsigned int   biSizeImage;     // 픽셀 데이터 크기 (0일 수도 있음)
    int            biXPelsPerMeter; // 가로 해상도
    int            biYPelsPerMeter; // 세로 해상도
    unsigned int   biClrUsed;       // 사용한 색상 수
    unsigned int   biClrImportant;  // 중요한 색상 수
} BITMAPINFOHEADER;

#pragma pack() // 원래 정렬 방식으로 되돌림

// bmp.c
int read_header(FILE* fp, BITMAPFILEHEADER* fh, BITMAPINFOHEADER* ih);

// header.c (과제 1-1)
int print_header(char* filename);

// hexdump.c (과제 1-2)
int hex_dump(char* in_name, char* out_name);

// grayscale.c (과제 1-3)
int make_gray(char* in_name, char* out_name);

#endif

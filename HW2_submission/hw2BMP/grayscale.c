#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

// 과제 1-3: BMP 이미지를 grayscale(흑백)로 바꿔서 out_name 파일에 저장한다.
// 24비트, 32비트 BMP만 지원한다.
int make_gray(char *in_name, char *out_name)
{
    FILE *in;
    FILE *out;
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    unsigned char *data;  // 파일 전체 내용
    unsigned char *pixel; // 지금 바꾸고 있는 픽셀
    unsigned char gray;
    long file_size;
    int width, height;
    int bytes_per_pixel;    // 픽셀 하나의 바이트 수
    int row_size; // 한 줄의 바이트 수 (패딩 포함)
    int x, y;

    in = fopen(in_name, "rb");
    if (in == NULL) {
        printf("파일을 열 수 없습니다: %s\n", in_name);
        return -1;
    }

    if (read_header(in, &fh, &ih) != 0) {
        printf("BMP 파일이 아닙니다: %s\n", in_name);
        fclose(in);
        return -1;
    }

    if (ih.biBitCount != 24 && ih.biBitCount != 32) {
        printf("24비트, 32비트 BMP만 변환할 수 있습니다.\n");
        fclose(in);
        return -1;
    }

    // 파일 크기를 구해서 파일 전체를 메모리로 읽는다
    fseek(in, 0, SEEK_END);
    file_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    data = (unsigned char *)malloc(file_size);
    if (data == NULL) {
        printf("메모리가 부족합니다.\n");
        fclose(in);
        return -1;
    }
    fread(data, 1, file_size, in);
    fclose(in);

    width = ih.biWidth;
    height = ih.biHeight;
    if (height < 0) // 높이가 음수면 위에서부터 저장된 이미지
        height = -height;

    bytes_per_pixel = ih.biBitCount / 8; // 24비트면 3, 32비트면 4

    // BMP는 한 줄의 크기가 4의 배수가 되도록 뒤에 0을 채운다 (패딩)
    row_size = (width * bytes_per_pixel + 3) / 4 * 4;

    // 픽셀 데이터가 파일 크기보다 크면 잘못된 파일
    if (fh.bfOffBits + (long)row_size * height > file_size) {
        printf("BMP 파일이 손상되었습니다.\n");
        free(data);
        return -1;
    }

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            pixel = data + fh.bfOffBits + y * row_size + x * bytes_per_pixel;

            // BMP는 B, G, R 순서로 저장되어 있다.
            // 밝기 = 0.299 R + 0.587 G + 0.114 B
            // 실수 대신 1000을 곱해서 정수로 계산하고, +500 은 반올림용
            gray = (299 * pixel[2] + 587 * pixel[1] + 114 * pixel[0] + 500) / 1000;

            pixel[0] = gray;
            pixel[1] = gray;
            pixel[2] = gray;
            // 32비트의 4번째 바이트(알파)는 그대로 둔다
        }
    }

    out = fopen(out_name, "wb");
    if (out == NULL) {
        printf("파일을 만들 수 없습니다: %s\n", out_name);
        free(data);
        return -1;
    }
    fwrite(data, 1, file_size, out);
    fclose(out);
    free(data);

    printf("grayscale 이미지를 %s 에 저장했습니다.\n", out_name);
    return 0;
}

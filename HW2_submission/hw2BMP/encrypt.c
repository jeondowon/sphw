#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

// 과제 2-1: 메시지 파일의 내용을 BMP 이미지 안에 숨겨서 새 파일로 저장한다.
// body의 바이트마다 맨 마지막 비트(LSB) 하나를 메시지의 비트로 바꾼다.
// 글자 하나(8비트)를 숨기는 데 body 8바이트가 필요하다.
// 메시지가 끝나면 끝 표시로 0을 하나 더 숨긴다.
int encrypt_msg(char *in_name, char *msg_name, char *out_name)
{
    FILE *in;
    FILE *msg;
    FILE *out;
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    unsigned char *data; // 파일 전체 내용
    long file_size;
    long pos;            // 다음에 비트를 숨길 위치
    int c;               // 메시지에서 읽은 글자
    int bit;
    int i;

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

    msg = fopen(msg_name, "r");
    if (msg == NULL) {
        printf("파일을 열 수 없습니다: %s\n", msg_name);
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
        fclose(msg);
        return -1;
    }
    fread(data, 1, file_size, in);
    fclose(in);

    // body 첫 바이트부터 숨기기 시작
    pos = fh.bfOffBits;

    while (1) {
        c = fgetc(msg);
        if (c == EOF)
            c = 0; // 메시지가 끝났으면 끝 표시(0)를 숨긴다

        // 글자 하나를 숨길 자리(8바이트)가 남아 있는지 확인
        if (pos + 8 > file_size) {
            printf("메시지가 너무 길어서 이미지에 다 넣을 수 없습니다.\n");
            free(data);
            fclose(msg);
            return -1;
        }

        // 글자의 비트를 앞쪽(7번 비트)부터 하나씩 숨긴다
        for (i = 7; i >= 0; i--) {
            bit = (c >> i) & 1;               // i번째 비트 꺼내기
            data[pos] = (data[pos] & 0xFE) | bit; // 마지막 비트만 bit로 바꾸기
            pos++;
        }

        if (c == 0)
            break;
    }
    fclose(msg);

    out = fopen(out_name, "wb");
    if (out == NULL) {
        printf("파일을 만들 수 없습니다: %s\n", out_name);
        free(data);
        return -1;
    }
    fwrite(data, 1, file_size, out);
    fclose(out);
    free(data);

    printf("메시지를 숨긴 이미지를 %s 에 저장했습니다.\n", out_name);
    return 0;
}

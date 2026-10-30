#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

// 과제 2-2: encrypt_msg로 숨긴 메시지를 꺼내서 화면에 출력한다.
// body의 바이트 8개에서 마지막 비트를 하나씩 모으면 글자 하나가 된다.
// 끝 표시인 0이 나올 때까지 반복한다.
int decrypt_msg(char *in_name)
{
    FILE *in;
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    unsigned char *data; // 파일 전체 내용
    long file_size;
    long pos;            // 다음에 비트를 꺼낼 위치
    int c;               // 모으고 있는 글자
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

    // body 첫 바이트부터 꺼내기 시작
    pos = fh.bfOffBits;

    printf("===== 해독한 메시지 =====\n");
    while (pos + 8 <= file_size) {
        // 8바이트의 마지막 비트를 모아서 글자 하나를 만든다
        c = 0;
        for (i = 0; i < 8; i++) {
            c = (c << 1) | (data[pos] & 1); // 왼쪽으로 한 칸 밀고 새 비트를 붙인다
            pos++;
        }

        if (c == 0) // 끝 표시가 나오면 멈춘다
            break;

        printf("%c", c);
    }
    printf("\n");

    free(data);
    return 0;
}

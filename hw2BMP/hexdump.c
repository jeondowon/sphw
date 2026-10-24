#include <stdio.h>
#include "bmp.h"

// 과제 1-2: BMP body(픽셀 데이터)를 16진수로 out_name 파일에 출력한다.
// 한 줄에 8바이트씩 출력하고, 줄 앞에는 body 첫 바이트를 0번지로 하는
// 상대 주소를 16진수로 붙인다.
//   예) 00000008: 00 00 00 FF 03 03 03 FF
int hex_dump(char *in_name, char *out_name)
{
    FILE *in;
    FILE *out;
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    int c;
    int count = 0; // 지금까지 출력한 바이트 수 (= 다음 바이트의 상대 주소)

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

    out = fopen(out_name, "w");
    if (out == NULL) {
        printf("파일을 만들 수 없습니다: %s\n", out_name);
        fclose(in);
        return -1;
    }

    // 픽셀 데이터가 시작하는 위치로 이동
    fseek(in, fh.bfOffBits, SEEK_SET);

    // 파일 끝까지 한 바이트씩 읽어서 출력
    while ((c = fgetc(in)) != EOF) {
        // 줄의 시작이면 주소를 먼저 출력
        if (count % 8 == 0)
            fprintf(out, "%08X:", count);

        fprintf(out, " %02X", c);
        count++;

        // 8개를 출력했으면 줄바꿈
        if (count % 8 == 0)
            fprintf(out, "\n");
    }

    // 마지막 줄이 8개가 안 되면 줄바꿈을 따로 해준다
    if (count % 8 != 0)
        fprintf(out, "\n");

    fclose(in);
    fclose(out);

    printf("BMP body %d 바이트를 %s 에 저장했습니다.\n", count, out_name);
    return 0;
}

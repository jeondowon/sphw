#include <stdio.h>
#include <string.h>
#include "bmp.h"

// 사용법 출력
void print_usage(char *name)
{
    printf("사용법:\n");
    printf("  %s -h <input_bmp_file>\n", name);
    printf("  %s -o <input_bmp_file> <out_file>\n", name);
    printf("  %s -g <input_bmp_file> <output_bmp_file>\n", name);
}

int main(int argc, char *argv[])
{
    int result;

    // 옵션과 인자 개수를 보고 알맞은 기능을 실행한다
    if (argc == 3 && strcmp(argv[1], "-h") == 0) {
        result = print_header(argv[2]);
    } else if (argc == 4 && strcmp(argv[1], "-o") == 0) {
        result = hex_dump(argv[2], argv[3]);
    } else if (argc == 4 && strcmp(argv[1], "-g") == 0) {
        result = make_gray(argv[2], argv[3]);
    } else {
        print_usage(argv[0]);
        return 1;
    }

    if (result != 0)
        return 1;
    return 0;
}

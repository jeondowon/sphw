#include <stdio.h>
#include <sys/stat.h>
#include "bmp.h"

// 파일에서 파일 헤더와 정보 헤더를 읽고 BMP 파일이 맞는지 확인한다.
// 성공하면 0, 실패하면 -1 을 돌려준다.
int read_header(FILE *fp, BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih)
{
    if (fread(fh, sizeof(BITMAPFILEHEADER), 1, fp) != 1)
        return -1;
    if (fread(ih, sizeof(BITMAPINFOHEADER), 1, fp) != 1)
        return -1;

    // 파일 맨 앞 두 글자가 'B'(0x42), 'M'(0x4D) 인지 확인
    // 리틀엔디언이라서 unsigned short 로 읽으면 0x4D42 가 된다.
    if (fh->bfType != 0x4D42)
        return -1;

    return 0;
}

// Compare identities, including hard links and symbolic links.
int same_file(const char *first, const char *second)
{
    struct stat a, b;
    return stat(first, &a) == 0 && stat(second, &b) == 0 &&
           a.st_dev == b.st_dev && a.st_ino == b.st_ino;
}

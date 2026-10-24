#!/bin/sh
# khj.bmp 로 hw2BMP 의 -h, -o, -g 기능을 테스트한다.
# 실행: make test  또는  sh test.sh
#
# 정답은 od 명령으로 khj.bmp 를 직접 읽어서 만들고, 프로그램 결과와 비교한다.

IN=khj.bmp
TMP=test_out
PASS=0
FAIL=0

# check "설명" 명령 : 명령이 성공하면 PASS, 실패하면 FAIL
check() {
    name=$1
    shift
    if "$@" > /dev/null 2>&1; then
        echo "  [PASS] $name"
        PASS=$((PASS + 1))
    else
        echo "  [FAIL] $name"
        FAIL=$((FAIL + 1))
    fi
}

if [ ! -f hw2BMP ]; then
    echo "hw2BMP 가 없습니다. make 를 먼저 실행하세요."
    exit 1
fi

rm -rf $TMP
mkdir $TMP

# khj.bmp 에서 직접 읽은 값들
SIZE=$(wc -c < $IN | tr -d ' ')
OFF=$(od -An -tu4 -j 10 -N 4 $IN | tr -d ' ')     # bfOffBits
WIDTH=$(od -An -tu4 -j 18 -N 4 $IN | tr -d ' ')   # biWidth
HEIGHT=$(od -An -tu4 -j 22 -N 4 $IN | tr -d ' ')  # biHeight
BIT=$(od -An -tu2 -j 28 -N 2 $IN | tr -d ' ')     # biBitCount
BODY=$((SIZE - OFF))

echo "테스트 파일: $IN (${WIDTH}x${HEIGHT}, ${BIT}비트, body $BODY 바이트)"

echo
echo "== 1-1) -h 헤더 출력 =="
./hw2BMP -h $IN > $TMP/header.txt
check "실행 성공"            ./hw2BMP -h $IN
check "bfType = BM"          grep -q "bfType *: BM$" $TMP/header.txt
check "bfSize = $SIZE"       grep -q "bfSize *: $SIZE$" $TMP/header.txt
check "bfOffBits = $OFF"     grep -q "bfOffBits *: $OFF$" $TMP/header.txt
check "biWidth = $WIDTH"     grep -q "biWidth *: $WIDTH$" $TMP/header.txt
check "biHeight = $HEIGHT"   grep -q "biHeight *: $HEIGHT$" $TMP/header.txt
check "biBitCount = $BIT"    grep -q "biBitCount *: $BIT$" $TMP/header.txt

echo
echo "== 1-2) -o 16진수 출력 =="
./hw2BMP -o $IN $TMP/body.txt > /dev/null
check "실행 성공"            ./hw2BMP -o $IN $TMP/body.txt

# 정답 파일 만들기: body 바이트를 한 줄에 하나씩 뽑은 뒤 8개씩 묶는다
od -An -tx1 -v -j $OFF $IN | tr -s ' ' '\n' | grep -v '^$' > $TMP/bytes.txt
awk '{
    if ((NR - 1) % 8 == 0) printf("%08X:", NR - 1)
    printf(" %s", toupper($1))
    if (NR % 8 == 0) printf("\n")
} END { if (NR % 8 != 0) printf("\n") }' $TMP/bytes.txt > $TMP/answer.txt

LINES=$(wc -l < $TMP/body.txt | tr -d ' ')
ANSWER_LINES=$(( (BODY + 7) / 8 ))
check "줄 수 = $ANSWER_LINES"        [ "$LINES" -eq "$ANSWER_LINES" ]
check "첫 줄 주소가 00000000"        grep -q "^00000000:" $TMP/body.txt
check "전체 내용이 정답과 같음"      cmp $TMP/body.txt $TMP/answer.txt

echo
echo "== 1-3) -g grayscale 변환 =="
./hw2BMP -g $IN $TMP/gray.bmp > /dev/null
check "실행 성공"                    ./hw2BMP -g $IN $TMP/gray.bmp
check "파일 크기가 원본과 같음"      [ "$(wc -c < $TMP/gray.bmp | tr -d ' ')" -eq "$SIZE" ]
check "헤더가 원본과 같음"           cmp -n $OFF $IN $TMP/gray.bmp

# 원본과 결과의 픽셀 바이트를 한 줄에 "원본 결과" 로 나란히 놓는다
od -An -tu1 -v -j $OFF $IN | tr -s ' ' '\n' | grep -v '^$' > $TMP/org.txt
od -An -tu1 -v -j $OFF $TMP/gray.bmp | tr -s ' ' '\n' | grep -v '^$' > $TMP/new.txt
paste $TMP/org.txt $TMP/new.txt > $TMP/pair.txt

# 픽셀마다 B, G, R 이 모두 밝기 공식 값과 같은지 확인 (khj.bmp 는 32비트라 4바이트씩)
check "모든 픽셀이 밝기 공식대로 흑백 변환됨" awk -v n=$((BIT / 8)) '{
    i = (NR - 1) % n
    org[i] = $1; new[i] = $2
    if (i == n - 1) {
        gray = int((299 * org[2] + 587 * org[1] + 114 * org[0] + 500) / 1000)
        if (new[0] != gray || new[1] != gray || new[2] != gray) exit 1
        if (n == 4 && new[3] != org[3]) exit 1
    }
}' $TMP/pair.txt

echo
echo "== 잘못된 사용 =="
check "인자 없음 -> 실패"        sh -c "! ./hw2BMP"
check "없는 옵션 -> 실패"        sh -c "! ./hw2BMP -x $IN"
check "인자 부족 -> 실패"        sh -c "! ./hw2BMP -o $IN"
check "없는 파일 -> 실패"        sh -c "! ./hw2BMP -h nofile.bmp"
check "BMP 아닌 파일 -> 실패"    sh -c "! ./hw2BMP -h Makefile"

rm -rf $TMP
echo
echo "결과: PASS $PASS / FAIL $FAIL"
[ $FAIL -eq 0 ]

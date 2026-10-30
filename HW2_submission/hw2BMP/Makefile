# hw2BMP Makefile

# make build : 컴파일해서 hw2BMP 실행 파일을 만든다 (make 만 쳐도 같음)
# make demo1 : -h, -o, -g 옵션을 차례로 실행하고 결과를 rep1.txt 에 저장
# make demo2 : -e, -d 옵션을 차례로 실행하고 결과를 rep2.txt 에 저장
# make clean : 만들어진 파일들을 지움

CC = gcc
CFLAGS = -Wall

OBJS = main.o bmp.o header.o hexdump.o grayscale.o encrypt.o decrypt.o

# demo에서 쓰는 파일 이름
IN_BMP   = khj.bmp
OUT_BODY = body.txt
OUT_GRAY = khj_gray.bmp
MSG      = message.txt
OUT_ENC  = khj_enc.bmp

build: hw2BMP

# 실행 파일 만들기 (.o 파일들을 링크)
hw2BMP: $(OBJS)
	$(CC) $(CFLAGS) -o hw2BMP $(OBJS)

# 각 소스 파일 컴파일 (bmp.h 가 바뀌어도 다시 컴파일하도록 같이 적음)
main.o: main.c bmp.h
	$(CC) $(CFLAGS) -c main.c

bmp.o: bmp.c bmp.h
	$(CC) $(CFLAGS) -c bmp.c

header.o: header.c bmp.h
	$(CC) $(CFLAGS) -c header.c

hexdump.o: hexdump.c bmp.h
	$(CC) $(CFLAGS) -c hexdump.c

grayscale.o: grayscale.c bmp.h
	$(CC) $(CFLAGS) -c grayscale.c

encrypt.o: encrypt.c bmp.h
	$(CC) $(CFLAGS) -c encrypt.c

decrypt.o: decrypt.c bmp.h
	$(CC) $(CFLAGS) -c decrypt.c

# 과제 1-1, 1-2, 1-3 테스트 (결과는 rep1.txt)
demo1: hw2BMP
	echo "== demo1: Test for -h, -o, -g options ==" > rep1.txt
	./hw2BMP -h $(IN_BMP) >> rep1.txt
	./hw2BMP -o $(IN_BMP) $(OUT_BODY) >> rep1.txt
	./hw2BMP -g $(IN_BMP) $(OUT_GRAY) >> rep1.txt
	@echo "demo1 is finished. Results are in rep1.txt"

# 과제 2-1, 2-2 테스트 (결과는 rep2.txt)
demo2: hw2BMP
	echo "== demo2: Test for -e, -d options ==" > rep2.txt
	./hw2BMP -e $(IN_BMP) $(MSG) $(OUT_ENC) >> rep2.txt
	./hw2BMP -d $(OUT_ENC) >> rep2.txt
	@echo "demo2 is finished. Results are in rep2.txt"

# 만들어진 파일 지우기
clean:
	rm -f $(OBJS) hw2BMP $(OUT_BODY) $(OUT_GRAY) $(OUT_ENC) rep1.txt rep2.txt

.PHONY: build demo1 demo2 clean

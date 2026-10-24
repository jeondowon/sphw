# hw2BMP Makefile

# make       : 컴파일해서 hw2BMP 실행 파일을 만든다
# make clean : 만들어진 파일들을 지움

CC = gcc
CFLAGS = -Wall

# 실행 파일 만들기 (.o 파일들을 링크)
hw2BMP: main.o bmp.o header.o hexdump.o grayscale.o
	$(CC) $(CFLAGS) -o hw2BMP main.o bmp.o header.o hexdump.o grayscale.o

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

# 만들어진 파일 지우기
clean:
	rm -f *.o hw2BMP

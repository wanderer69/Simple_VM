SRC6 = Unit1
SRC7 = extmod1

CC = ../../../../../tools/ucfront-gcc arm-elf-gcc
GCC = gcc

OPT = -Os -Wall -c -D__LINUX -DEXTMOD
# -D_DEBUG
# -D_DEBUG

all: src6 
# src7

src7:
	$(CC) -I./include -c $(OPT) -o $(SRC7).o $(SRC7).c -D_LOAD_LIB
	$(CC) -I./include -c $(OPT) -o comcode.o COMCODE.c
	$(CC) -I./include -c $(OPT) -o command.o command.c
	$(CC) -I./include -c $(OPT) -o funcload.o funcload.c
	$(CC) -I./include -c $(OPT) -o memman.o memman.c
	$(CC) -I./include -c $(OPT) -o objects.o Objects.c
	$(CC) -I./include -c $(OPT) -o oper.o OPER.c
	$(CC) -I./include -c $(OPT) -o print.o PRINT.c
	$(CC) -I./include -c $(OPT) -o read.o READ.c
	$(CC) -I./include -c $(OPT) -o vm.o vm.c
	$(CC) -I./include -c $(OPT) -o low_lev_func.o low_lev_func.c
	$(CC) -I./include -c $(OPT) -o gc.o gc.c
	$(CC) -I./include -c $(OPT) -o win_printf.o win_printf.c

src6:
	$(GCC) -I./include -c $(OPT) -o comcode.o COMCODE.c
	$(GCC) -I./include -c $(OPT) -o command.o command.c
	$(GCC) -I./include -c $(OPT) -o funcload.o funcload.c
	$(GCC) -I./include -c $(OPT) -o memman.o memman.c
	$(GCC) -I./include -c $(OPT) -o objects.o Objects.c
	$(GCC) -I./include -c $(OPT) -o oper.o OPER.c
	$(GCC) -I./include -c $(OPT) -o print.o PRINT.c
	$(GCC) -I./include -c $(OPT) -o read.o READ.c
	$(GCC) -I./include -c $(OPT) -o vm.o vm.c
	$(GCC) -I./include -c $(OPT) -o low_lev_func.o low_lev_func.c
	$(GCC) -I./include -c $(OPT) -o gc.o gc.c
	$(GCC) -I./include -c $(OPT) -o win_printf.o win_printf.c
	$(GCC) -I./include -c $(OPT) -o main.o main.c
	$(GCC) -I./include -c $(OPT) -o $(SRC6).o $(SRC6).c -D_LOAD_LIB
	$(GCC) -o $(SRC6) $(SRC6).o comcode.o command.o funcload.o memman.o objects.o oper.o print.o read.o vm.o low_lev_func.o gc.o win_printf.o main.o

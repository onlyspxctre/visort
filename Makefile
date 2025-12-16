all: build

build:
	clang -Wall -Wextra -std=gnu11 -o visort visort.c -lraylib -lpthread

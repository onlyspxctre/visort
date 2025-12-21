all: build

build:
	gcc -Wall -Wextra -std=gnu11 -fuse-ld=mold -o ./build/visort visort.c -lraylib -lpthread

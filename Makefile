stencil: stencil.c
	gcc -std=c99 -Wall $^ -o $@
#	icc -std=c99 -Wall -O0 $^ -o $@

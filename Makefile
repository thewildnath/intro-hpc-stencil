stencil: stencil.c
#	gcc -std=c99 -Wall -O3 -march=native -ffast-math -msse4 $^ -o $@
#	icc -std=c99 -Wall -O2 -march=native -no-prove-div -fp-model fast -qopt-report $^ -o $@
#	icc -std=c99 -Wall -fast -falias -xSSE3 -qopt-report $^ -o $@
	icc -std=c99 -Wall -O2 -qopt-report $^ -o $@

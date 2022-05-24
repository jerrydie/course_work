flags = -O3 -march=native -fopenmp

all: fea

lib:
	gcc -c -std=c99 -O3 -march=native common/FEA_Library/block_enc/*.c
	gcc -c -std=c99 -O3 -march=native common/FEA_Library/transform_naive/*.c
	ar crv libfea.a *.o
	rm fea_enc.o
	rm fea_interface.o
	rm math_simple.o
	rm transform_naive.o

fea: lib
	g++ $(flags) main.cpp libfea.a -o FEA1_Linear


clean:
	rm -f FEA1_Linear
	rm -f libfea.a

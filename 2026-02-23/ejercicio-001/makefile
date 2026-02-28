all: clean compilar
	
clean:
	rm -rf cache_demo a.out cachegrind.out.*

compilar:
	gcc -O2 -std=c11 cache_demo.c -o cache_demo

valgrindf: compilar
	valgrind --tool=cachegrind --cache-sim=yes --branch-sim=yes ./cache_demo f

valgrindc: compilar
	valgrind --tool=cachegrind --cache-sim=yes --branch-sim=yes ./cache_demo c

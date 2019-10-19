GPP = g++ -O3 -Wall -std=c++11

M = main.o

Aprog: $(M)
	$(GPP) -o Aprog $(M)

main.o: main.cc
	$(GPP) -o main.o -c main.cc

clean:
	rm Aprog
	clean


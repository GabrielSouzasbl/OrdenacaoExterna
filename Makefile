all: 
	g++ gerador.cpp -Wall -o gerador
	g++ -c registro.cpp
	g++ -c area.cpp
	g++ -c fita.cpp
	g++ -c quicksort.cpp
	g++ -c analise.cpp
	g++ -c main.cpp
	g++ registro.o area.o fita.o quicksort.o analise.o main.o -o tp
clean:
	rm -f *.o 
	rm gerador
	rm tp

run:
	./gerador
	./tp


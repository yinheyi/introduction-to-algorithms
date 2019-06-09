cc=g++
a.out: RBTree.o main.o
	$(cc) -o a.out RBTree.o main.o
RBTree.o: RBTree.cpp RBTree.h
	$(cc) -c RBTree.cpp
main.o: main.cpp RBTree.h
	$(cc) -c main.cpp
clean:
	rm RBTree.o main.o a.out

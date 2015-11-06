target : knn.o main.o
	 gcc main.c knn.c -Wall -o main -lm
main.o : main.c
	 gcc main.c header.h -c -g
knn.o : knn.c
	 gcc knn.c header.h -c -lm
clean:
	rm -rf *.o target

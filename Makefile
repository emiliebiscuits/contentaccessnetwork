EXEC=out
MPI=mpicc
CC=gcc

all: $(EXEC)

out: main.o espace.o
	$(MPI) `pkg-config --cflags --libs glib-2.0` -o $@ $^

espace.o: espace.c
	$(MPI) `pkg-config --cflags --libs glib-2.0` -o $@ -c $<
	
main.o:	main.c espace.h
	$(MPI)  `pkg-config --cflags --libs glib-2.0` -o $@ -c $<
		
clean:
	rm -rf *.o
	
mrproper: clean
	rm -rf $(EXEC)
	
exec:
	mpirun -np 6 $(EXEC)


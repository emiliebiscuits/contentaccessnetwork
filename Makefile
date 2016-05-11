EXEC=out
MPI=mpicc
CC=gcc

all: $(EXEC)

out: main.o espace.o
	$(MPI) -o $@ $^

espace.o: espace.c
	$(MPI) -o $@ -c $<
	
main.o:	main.c espace.h
	$(MPI) -o $@ -c $<
		
clean:
	rm -rf *.o
	
mrproper: clean
	rm -rf $(EXEC)
	
exec:
	mpirun -np 6 $(EXEC)



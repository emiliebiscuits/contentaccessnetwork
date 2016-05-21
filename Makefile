EXEC=out
MPI=mpicc
CC=gcc

all: $(EXEC)

out: main.o espace.o voisin.o donnee.o
	$(MPI) `pkg-config --cflags --libs glib-2.0` -o $@ $^

espace.o: espace.c
	$(MPI) -o $@ -c $<
	
donnee.o: donnee.c
	$(MPI) `pkg-config --cflags --libs glib-2.0` -o $@ -c $<
	
voisin.o: voisin.c
	$(MPI) `pkg-config --cflags --libs glib-2.0` -o $@ -c $<
	
main.o:	main.c espace.h voisin.h donnee.h
	$(MPI)  `pkg-config --cflags --libs glib-2.0` -o $@ -c $<
		
clean:
	rm -rf *.o
	
mrproper: clean
	rm -rf $(EXEC)
	
exec:
	mpirun -np 6 $(EXEC)


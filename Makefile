EXEC=main
CC=mpicc

all: $(EXEC)

main: main.c
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o
	
mrproper: clean
	rm -rf $(EXEC)
	
exec:
	mpirun -np 6 $(EXEC)



CC=mpicc

###################################

SRC=src

EXE=vector_mpi

OBJECT_FILES = $(SRC)/main.c

###################################

$(EXE): $(OBJECT_FILES)
	$(CC) $< -o $@

clean:
	rm $(EXE)
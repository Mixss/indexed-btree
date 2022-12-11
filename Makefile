ODIR=./build
IDIR=./include
EXEC=program

_OBJ = main.o record.o page.o btree.o file_reader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
FLAGS = -Wall -I$(IDIR) -g 

_DEPS = record.h page.h btree.h data.h file_reader.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: src/%.c $(DEPS)
	gcc -c $< -o $@  $(FLAGS)

all: $(OBJ)
	gcc $(OBJ) -o $(EXEC) $(FLAGS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*
	rm $(EXEC)


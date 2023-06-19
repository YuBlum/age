EXE=sandbox
CC=cc
LIBS=-L. -lm -lmoonlib
FLAGS=-I /usr/include/GLFW/ -I /usr/include/AL/ -I../dev/include/
CFILES=$(wildcard *.c)

.PHONY: all clean res

all: $(EXE)

$(EXE): main.c
	$(CC) $(FLAGS) -o $(EXE) main.c $(LIBS)

run:
	./$(EXE)

clean:
	@rm $(EXE)

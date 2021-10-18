CC=gcc
SHELL=/bin/sh
CFLAGS=-lstdc++ -ggdb3 -Wno-deprecated -Wall -Wextra -pedantic -pie -pedantic -static-libasan # -fsanitize=address
SRC=./src
OBJ=./obj
INC=./include


FILES=main.cpp
SOURCES=$(patsubst %,$(SRC)/%,$(FILES))
OBJECTS=$(patsubst %.cpp,$(OBJ)/%.o,$(FILES))

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

cpu: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

delobj:
	rm -f $(OBJ)/*.o

clean:
	rm -f $(OBJ)/*.o ./cpu

.PHONY: delobj clean

CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = functions.h
OBJ = functions.o tremios.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tremios: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^


TARGET=teste
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
CCFLAGS=$(DEBUG) $(OPT) $(WARN)
LD=g++
OBJS= main.o 
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) 
 
main.o: main.cpp
	$(CC) -c $(CCFLAGS) main.cpp -o main.o

clean:
	@rm -f *.o

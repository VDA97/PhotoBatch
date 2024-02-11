TARGET=teste
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
CCFLAGS=$(DEBUG) $(OPT) $(WARN)
LD=g++
OBJS= main.o argumentparser.o utils.o
GTEST=-lgtest -lgmock
GTEST_MAIN=-lgtest_main
PTHREAD= -lpthread  
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(GTEST) $(GTEST_MAIN) $(PTHREAD)
 
main.o: main.cpp
	$(CC) -c $(CCFLAGS) $(GTEST) $(GTEST_MAIN) $(PTHREAD) main.cpp -o main.o

argumentparser.o: ArgumentParser.cpp
	$(CC) -c $(CCFLAGS) $(GTEST) $(GTEST_MAIN) $(PTHREAD) ArgumentParser.cpp -o argumentparser.o

utils.o: Utils.cpp
	$(CC) -c $(CCFLAGS) $(GTEST) $(GTEST_MAIN) $(PTHREAD) Utils.cpp -o utils.o 

clean:
	@rm -f *.o

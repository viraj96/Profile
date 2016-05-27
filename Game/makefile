CC = g++
CFLAGS = -c -O3
CLINK_FLAGS = -lSDL
all : game

game : main.o quickcg.o
	$(CC) main.o quickcg.o -o game $(CLINK_FLAGS)

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

quickcg.o : quickcg.cpp
	$(CC) $(CFLAGS) quickcg.cpp

clean :
	rm *.o game
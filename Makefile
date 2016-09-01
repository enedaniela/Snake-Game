CC = gcc
FLAGS = -Wall
PROGS = snake
all: $(PROGS)

snake: snake.c
	$(CC) snake.c -o snake $(FLAGS) -lncurses -lm
run: 
	./snake

clean:
	rm -f *~ *.out $(PROGS)

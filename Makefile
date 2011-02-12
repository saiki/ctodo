CC = gcc

.c.o:
	$(CC) -g -c $<
.PHONY : all
all: todo
todo: main.o todo.o
	@if [ ! -d runtime ]; then \
		echo ";; mkdir runtime"; mkdir runtime; \
	fi
	$(CC) -Wall -o runtime/$@ $^
main.o: todo.o
todo.o: todo.h
.PHONY : clean
clean: 
	rm -rf *.o runtime

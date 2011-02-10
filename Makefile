CC = gcc

.c.o:
	$(CC) -c $<
.PHONY : all
all: todo
todo: main.o
	@if [ ! -d runtime ]; then \
		echo ";; mkdir runtime"; mkdir runtime; \
	fi
	$(CC) -Wall -o runtime/$@ $^
.PHONY : clean
clean: 
	rm -rf *.o runtime

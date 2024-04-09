CC=clang

all: hello

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c
	./hello
	rm -f hello
clean:
	rm -f hello
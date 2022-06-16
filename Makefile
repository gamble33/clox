CC=gcc
flags=-g -Wall -lm -ldl -fPIC -rdynamic
exec=a.out
sources=$(wildcard src/*.c)
objects=$(sources:.c=.o)

%.o: %.c include/%.h
	$(CC) -c $(flags) $< -o $@

$(exec): $(objects)
	$(CC) $(objects) $(flags) -o $(exec)

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o

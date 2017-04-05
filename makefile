target = lc3
libraries = -lm
compiler = gcc
flags = -g -std=c99 -Wall

.PHONY: default all

default: $(target)
all: default

objs = $(patsubst %.c, %.o, $(wildcard *.c))
heads = $(wildcard *.h)

%.o: %.c $(heads)
	$(compiler) $(flags) -c $< -o $@

.PRECIOUS: $(target) $(objs)

$(target): $(objs)
	$(compiler) $(objs) -Wall $(libraries) -o $@

clean:
	-rm -f *.o
	-rm -f $(target)
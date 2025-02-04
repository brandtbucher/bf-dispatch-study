.PHONY: all clean time

CFLAGS = -O3 -Wall
TARGETS = bf-loop bf-goto bf-tail

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

time: $(TARGETS)
	for bf in $^; do \
	    echo \\n$$bf: >&2; \
		time ./$$bf mandelbrot.bf; \
	done

$(TARGETS): %: %.o common.o

common.o: common.c common.h

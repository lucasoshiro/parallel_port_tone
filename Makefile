CFLAGS = -Wall -O2
LDFLAGS = -lpthread

.PHONY: all
all: tone

.PHONY:clean
clean:
	rm -f tone

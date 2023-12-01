CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG) --libs gtk4) -lnotify

SRC = main.c 

OBJS = $(SRC:.c=.o)

all: notifytest


%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<


notifytest: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f notifytest


# program executable name
TARGET = sem_test

# Xenomai stuff
XENO_DESTDIR:=
XENO_CONFIG:=$(XENO_DESTDIR)/usr/bin/xeno-config
XENO_CFLAGS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --cflags)
XENO_LIBS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --ldflags)


# compiler flags
CFLAGS = $(XENO_CFLAGS) $(XENO_LIBS) -g -Wall -D_GNU_SOURCE 

# linker flags
LDFLAGS = $(XENO_LIBS) $(XENO_CFLAGS) -lrt -pthread -lcomedi -lm

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)

# compile
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<

# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)

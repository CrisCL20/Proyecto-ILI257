TARGET = SGP
CC = g++

cflags = -std=c++20 -Wall -Iinclude -pg

SRCDIR = src
INCLUDEDIR = include
SOURCES := $(wildcard $(SRCDIR)/*.cc)
OBJECTS := $(SOURCES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(cflags) -o $@ $^

%.o: %.cc
	$(CC) $(cflags) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
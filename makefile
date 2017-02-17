EXEC = Game
CFLAGS = -Wall -Iinclude
CC = gcc $(CFLAGS)
LIBS = -lSDL2main -lSDL2 -lSDL2_image
SOURCES = $(wildcard src/*.c)
HEADERS = $(SOURCES:src/%.c=include/%.h)
OBJECTS = $(SOURCES:src/%.c=%.o)

all: debug

debug: CFLAGS += -DDEBUG -g -pg
debug: release

release: $(OBJECTS)
	$(CC) $^ $(LIBS) -o $(EXEC)

%.o: src/%.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC) gmon.out

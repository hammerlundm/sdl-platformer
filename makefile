EXEC = Game
CFLAGS = -Wall -Iinclude
WFLAGS = $(CFLAGS) -mwindows
CC = gcc $(CFLAGS)
WCC = x86_64-w64-mingw32-gcc $(WFLAGS)
LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm
WLIBS = -lmingw32 $(LIBS)
SOURCES = $(wildcard src/*.c)
HEADERS = $(SOURCES:src/%.c=include/%.h)
OBJECTS = $(SOURCES:src/%.c=%.o)
WOBJECTS = $(SOURCES:src/%.c=%.wo)

all: debug

run: debug
	./$(EXEC)

debug: CFLAGS += -DDEBUG -g -pg
debug: release

release: $(OBJECTS)
	$(CC) $^ $(LIBS) -o $(EXEC)

windows: $(WOBJECTS)
	$(WCC) $^ $(WLIBS) -fstack-protector -o $(EXEC).exe

%.o: src/%.c $(HEADERS)
	$(CC) -c $< -o $@

%.wo: src/%.c $(HEADERS)
	$(WCC) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(WOBJECTS) $(EXEC) $(EXEC).exe gmon.out

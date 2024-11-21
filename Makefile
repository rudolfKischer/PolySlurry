CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -flto -funroll-loops



SRCS = $(wildcard src/*.c)

OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

TARGET = build/main

$(TARGET): $(OBJS)
	mkdir -p build
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -framework OpenGL -lSDL2

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

clean:
	rm -f build/*.o $(TARGET)

run: clean all
	./$(TARGET)

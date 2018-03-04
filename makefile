CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -I.
LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lm


presa: main.o presa.o presa_init.o graphics_sdl.o graphics_init_sdl.o kunbus.o
	$(CC) -o presa main.o presa.o presa_init.o graphics_sdl.o graphics_init_sdl.o kunbus.o $(LIBS) $(DEPS)

.PHONY:clean

clean:
	find . -name "*.o" -type f -delete

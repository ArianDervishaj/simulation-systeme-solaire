#The compiler
CC:=gcc
#The flags passed to the compiler
CFLAGS:=-g -Ofast -Wall -Wextra -fsanitize=address -fsanitize=leak -std=gnu11
#The flags passed to the linker
LDFLAGS:=-lm -lSDL2
#Path to the lib Vec2
VPATH:=vec2 gfx celestial_body



main: main.o vec2.o gfx.o celestial_body.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run_tests: tests
	./$<

tests: vec_tests.o vec2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

celestial_body.o: celestial_body.h

vec2.o: vec2.h

gfx.o: gfx.h

rebuild : clean main

clean:
	rm -f *.o main tests

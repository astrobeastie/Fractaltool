CC := gcc
FT_OBJECTS :=$(addprefix obj/, fet.o fractaltool.o helpers.o julia.o mandelbrot.o)
FV_OBJECTS := $(addprefix obj/, fet.o viewer.o gui.o helpers.o juliav.o mandelbrotv.o)
fractaltool: CFLAGS := -DFRACTALTOOL `pkg-config --cflags cairo` -O2 -Wall
fractalviewer: CFLAGS := `pkg-config --cflags gtk+-3.0` -O2 -Wall

all: clean fractaltool fractalviewer

fractaltool: $(FT_OBJECTS)
	$(CC) $(FT_OBJECTS) -lm `pkg-config --cflags --libs cairo` -Ofast -rdynamic -o bin/fractaltool
	
fractalviewer: $(FV_OBJECTS)
	gcc $(FV_OBJECTS) -lm `pkg-config --cflags --libs gtk+-3.0` -Ofast -rdynamic -o bin/fractalviewer

obj/juliav.o:
	$(CC) `pkg-config --cflags gtk+-3.0` -O2 -Wall -c -o obj/juliav.o src/julia.c

obj/mandelbrotv.o:
	$(CC) `pkg-config --cflags gtk+-3.0` -O2 -Wall -c -o obj/mandelbrotv.o src/mandelbrot.c

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.s
	nasm -f elf64 $< -o $@

clean:
	rm -rf bin/fractaltool
	rm -rf bin/fractalviewer
	rm -rf obj/*.o

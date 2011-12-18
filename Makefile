all: build

build:
	gcc -o mandelbrot mandelbrot.c

test: build
	./mandelbrot > mandelbrot.ppm
	convert mandelbrot.ppm mandelbrot.png
	open mandelbrot.png
	rm mandelbrot.ppm

clean:
	rm mandelbrot

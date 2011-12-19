all: build

build:
	gcc -o mandelbrot mandelbrot.c

test: build
	./mandelbrot 800 600 1000 > mandelbrot.ppm
	convert mandelbrot.ppm mandelbrot.png
	open mandelbrot.png

clean:
	rm mandelbrot mandelbrot.ppm

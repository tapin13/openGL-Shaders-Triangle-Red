LDLIBS=-lglut  -lGLEW -lGL
all: triangle-red
clean:
	rm -f *.o triangle-red
.PHONY: all clean

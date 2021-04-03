CC = g++

CFLAGS = -Wall -Wextra -Wnarrowing -Wfloat-equal -Wundef -Wshadow 
CFLAGS +=-Wconversion -Wunreachable-code -Wpointer-arith -Wcast-align
CFLAGS += -Wwrite-strings -Wswitch-default -Wswitch-enum -Winit-self -Wcast-qual -Wlogical-op

LFLAGS = -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi
LFLAGS += -lglfw -ldl -lm

all: example

example: sphere.o camera.o camera_example.o
	$(CC) $(LFLAGS) sphere.o camera.o camera_example.o -o example.exe

sphere.o: sphere.cpp
	$(CC) $(CFLAGS) $(LFLAGS) -c sphere.cpp

camera_example.o: camera_example.cpp shader_source
	$(CC) $(CFLAGS) $(LFLAGS) -c camera_example.cpp

camera.o: ../camera/camera.cpp
	$(CC) $(CFLAGS) $(LFLAGS) -c ../camera/camera.cpp -o ./camera.o

clean:
	rm -rf *.o

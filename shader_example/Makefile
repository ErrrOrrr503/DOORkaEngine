CC = g++

CFLAGS = -Wall -Wextra -Wnarrowing -Wfloat-equal -Wundef -Wshadow 
CFLAGS +=-Wconversion -Wunreachable-code -Wpointer-arith -Wcast-align
CFLAGS += -Wwrite-strings -Wswitch-default -Wswitch-enum -Winit-self -Wcast-qual -Wlogical-op

LFLAGS = -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi
LFLAGS += -lglfw -ldl -lm

all: oshposhmak

oshposhmak: sphere.o shader_example.o
	$(CC) $(LFLAGS) sphere.o shader_example.o -o shader_example.exe

sphere.o: sphere.cpp
	$(CC) $(CFLAGS) $(LFLAGS) -c sphere.cpp $(LFLAGS)

shader_example.o: shader_example.cpp
	$(CC) $(CFLAGS) $(LFLAGS) -c shader_example.cpp $(LFLAGS)

clean:
	rm -rf *.o

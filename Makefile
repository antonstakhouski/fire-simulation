CC=g++

CXX_FLAGS=-c -std=c++11 -Wall \
	  # -g -O0 \
	  # -pg
LD_FLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW \
	 # -pg

SOURCES=main.cpp \
	game.cpp \
	shader.cpp \
	texture.cpp \
	resource_manager.cpp \
   	particle_generator.cpp \
	camera.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=fire

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LD_FLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CXX_FLAGS) $< -o $@

clean:
	rm -rf a.out *.o

.PHONY: clean

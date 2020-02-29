CC=g++

CXX_FLAGS=-c -std=c++17 -Wall \
	  # -g -O0 \
	  # -pg
LD_FLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW \
	 # -pg

SOURCES=main.cpp \
	game.cpp \
	shader.cpp \
	texture.cpp \
	resource_manager.cpp \
   	emitter.cpp \
	camera.cpp \
	particle.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=fire

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LD_FLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CXX_FLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o

.PHONY: clean

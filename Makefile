SOURCES=main.cpp \
	game.cpp \
	shader.cpp \
	texture.cpp \
	resource_manager.cpp

LD_FLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW


all:
	g++ $(SOURCES) $(LD_FLAGS)
clean:
	rm -rf a.out

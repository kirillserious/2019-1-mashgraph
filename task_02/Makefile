# Опции компилятора
CC=g++
CFLAGS=-c -g -std=c++17 -Wall
LDFLAGS=-lglfw -lGLEW -lGL -lassimp -lm

# Исходный код
SOURCES=src/main.cpp src/shader.cpp src/window.cpp src/texture2d.cpp src/vao.cpp src/Objects/mesh_object.cpp src/Objects/asteroid.cpp src/skybox.cpp src/mesh.cpp src/model.cpp src/Objects/laser.cpp src/Frame/aim.cpp src/Frame/healthbar.cpp src/Objects/spaceship.cpp src/fs.cpp
OBJECTS=$(SOURCES:.cpp=.o)

# Программа
EXECUTABLE=build/task.out

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $< -o $@ $(CFLAGS)

clean:
	rm -r $(OBJECTS) $(EXECUTABLE)
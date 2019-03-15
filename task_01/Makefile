# Опции компилятора
CC=g++
CFLAGS=-c -std=c++11 -Wall
LDFLAGS=-lglfw -lGLEW -lGL -lm

# Исходный код
SOURCES=src/main.cpp src/shader.cpp
OBJECTS=$(SOURCES:.cpp=.o)

# Папка с шейдерами
SHADER_SRC=src/shaders
SHADER_BUILD=build/shaders

# Программа
EXECUTABLE=build/task.out

all: $(SOURCES) $(EXECUTABLE) $(SHADER_BUILD)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $< -o $@ $(CFLAGS)

$(SHADER_BUILD):
	cp -r $(SHADER_SRC) $@
clean:
	rm -r $(OBJECTS) $(SHADER_BUILD) $(EXECUTABLE)

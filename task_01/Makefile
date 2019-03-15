CC=g++
CFLAGS=-c -std=c++11 -Wall
LDFLAGS=-lglfw -lGLEW -lGL -lm
SOURCES=src/main.cpp src/shader.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=task_01.out

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o build/$@ $(LDFLAGS)

.cpp.o:
	$(CC) $< -o $@ $(CFLAGS) 

clean:
	rm -r $(OBJECTS)

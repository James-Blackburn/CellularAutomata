CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lGL -lGLEW -lglfw -lm

TARGET = cellular_automata
SOURCES = main.cpp Simulator.cpp PlayingState.cpp ResourceManager.cpp Shader.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean

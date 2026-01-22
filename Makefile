CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iexternal/glad/include
LDFLAGS = -lGL -lglfw -lm -ldl

TARGET = cellular_automata
SOURCES = main.cpp Simulator.cpp PlayingState.cpp ResourceManager.cpp Shader.cpp
OBJECTS = $(SOURCES:.cpp=.o)

GLAD_SRC = external/glad/src/glad.c
GLAD_OBJ = external/glad/src/glad.o

all: $(TARGET)

$(TARGET): $(OBJECTS) $(GLAD_OBJ)
	$(CXX) $(OBJECTS) $(GLAD_OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GLAD_OBJ): $(GLAD_SRC)
	$(CXX) $(CXXFLAGS) -c $(GLAD_SRC) -o $(GLAD_OBJ)

clean:
	rm -f $(OBJECTS) $(GLAD_OBJ) $(TARGET)

.PHONY: all clean

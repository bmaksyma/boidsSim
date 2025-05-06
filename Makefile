# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# SFML configuration
SFML_INCLUDE = -I/usr/include -I.
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Project files
SRCS = main.cpp Fish.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = fish_simulation

# Main target
all: $(EXEC)

# Linking
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(SFML_LIBS)

# Compilation rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INCLUDE) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
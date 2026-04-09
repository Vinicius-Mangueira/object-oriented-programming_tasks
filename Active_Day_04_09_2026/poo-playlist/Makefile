CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
SRCS     = src/main.cpp src/GerenciadorReproducao.cpp
TARGET   = playlist

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

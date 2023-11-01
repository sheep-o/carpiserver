CC := g++
CFLAGS := -Wall -g -Iinclude/ -I/opt/homebrew/Cellar/boost/1.83.0/include -I/opt/homebrew/Cellar/boost/1.81.0_1/include -I/System/Volumes/Data/opt/homebrew/include/ -I/opt/homebrew/opt/opencv/include/opencv4 -std=c++20 $(shell pkg-config --cflags opencv4)

TARGET := server

LIBPATHS :=
LIBS := $(shell pkg-config --libs opencv4)

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(LIBPATHS) $(LIBS) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) -pthread -c $<
clean:
	rm -rf $(TARGET) *.o server
all: run
run: $(TARGET)
	./$(TARGET)

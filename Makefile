CC := g++
CFLAGS := -Wall -g -Iinclude/ -I/opt/homebrew/Cellar/boost/1.83.0/include -I/opt/homebrew/Cellar/boost/1.81.0_1/include -I/System/Volumes/Data/opt/homebrew/include/ -std=c++20
TARGET := server

LIBPATHS :=
LIBS := 

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(LIBPATHS) $(LIBS) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) -lpthread -c $<
clean:
	rm -rf $(TARGET) *.o
all: run
run: $(TARGET)
	./$(TARGET)

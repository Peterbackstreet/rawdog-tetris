ifeq ($(OS),Windows_NT)
    SYSTEM := Windows
else
    SYSTEM := Linux
endif

CXX = g++

ifeq ($(SYSTEM),Windows)
    TARGET := main.exe
    LIBS := -L./lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
    RM := del
else
    TARGET := main
    LIBS := -lraylib -lGL -lm -lpthread -ldl
    RM := rm -f
endif

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) -o $@ $< $(LIBS)

clean:
	$(RM) $(TARGET)

.PHONY: all clean

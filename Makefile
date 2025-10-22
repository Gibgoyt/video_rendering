CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude $(shell pkg-config --cflags libavcodec libavformat libavutil libswscale)
LDFLAGS = $(shell pkg-config --libs libavcodec libavformat libavutil libswscale) -lm

TARGET = video_renderer
SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET) output.mp4 output_3d.mp4

run: $(TARGET)
	./$(TARGET)

email:
	./scripts/email/send.sh

.PHONY: all clean run

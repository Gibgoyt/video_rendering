CC = gcc
CFLAGS = -Wall -O2 -Iinclude $(shell pkg-config --cflags librsvg-2.0 cairo libavcodec libavformat libavutil libswscale)
LDFLAGS = $(shell pkg-config --libs librsvg-2.0 cairo libavcodec libavformat libavutil libswscale) -lm

TARGET = video_renderer
SRCDIR = src
SOURCES = $(SRCDIR)/main.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET) output.mp4

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
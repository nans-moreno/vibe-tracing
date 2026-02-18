# Makefile - Vibe Tracing
# Simple makefile for quick builds (alternative to CMake)

CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11
LDFLAGS := -lm -lglfw -lGL

SOURCES := main.c math.c raytracer.c camera.c scene.c ui.c graphics.c
OBJECTS := $(SOURCES:.c=.o)
TARGET := vibe_tracing

# OpenMP support (optional)
OPENMP ?= 0
ifeq ($(OPENMP), 1)
    CFLAGS += -fopenmp
    LDFLAGS += -fopenmp
endif

# Debug build
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS := -Wall -Wextra -g -std=c11 -O0
endif

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LDFLAGS += -lX11
endif
ifeq ($(UNAME_S), Darwin)
    # macOS
    LDFLAGS := -lm -lglfw -framework OpenGL
endif

.PHONY: all clean help

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Build successful: $@"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned"

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Usage: make [OPTION]"
	@echo ""
	@echo "Options:"
	@echo "  make              - Build project"
	@echo "  make run          - Build and run"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make DEBUG=1      - Build with debug symbols"
	@echo "  make OPENMP=1     - Build with OpenMP support"
	@echo ""
	@echo "Example:"
	@echo "  make OPENMP=1 run  # Build with OpenMP and run"

# Compiler and flags
CC = gcc
CFLAGS = -g

# Target executable
TARGET = newshell

# Source files
SRCS = main.c \
       preprocessing.c \
       redirecting.c \
       myhistory.c \
       cd.c \
       alias.c \
       path.c \
       pipelining.c \
	   exit.c \
	   signal-control.c

# Object files (automatically generated from source files)
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = preprocessing.h \
          redirecting.h \
          myhistory.h \
          cd.h \
          alias.h \
          path.h \
          pipelining.h \
		  exit.h \
		  signal-control.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target for removing compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Run target (for testing)
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run

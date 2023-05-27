# Source files
SRCS = $(wildcard src/*.cpp) 
OBJS = $(SRCS:.cpp=.o)

# Compiler
CC = g++

# Directories
INCLUDE_PATHS = -Ideps/include
LIBRARY_PATHS = -Ldeps/libs
OBJ_DIR = objs/*.o

# Compiler flags
COMPILER_FLAGS = -std=c++17 -Wall -Wextra $(INCLUDE_PATHS)

# Linker flags
LINKER_FLAGS = -lSDL2 -lGL -lGLEW -lGLU -lfreetype $(LIBRARY_PATHS)

# Output binary
BIN_NAME = bin/app

# Build target
all: $(BIN_NAME)

$(BIN_NAME): $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(OBJS) $(OBJ_DIR) -o $(BIN_NAME) $(LINKER_FLAGS)

%.o: %.cpp
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) # $(BIN_NAME)

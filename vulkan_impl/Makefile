
#OBJS specifies which files to compile as part of the project
OBJS = $(wildcard src/*.cpp) $(wildcard compiled/*.o)

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I.\deps\include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L.\deps\lib\SDL2   -L.\deps\lib\vulkan

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,console -std=c++17 -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2 -lvulkan-1

#OBJ_NAME specifies the name of our exectuable
BIN_NAME = .\bin\app

#This is the target that compiles our executable
all : $(OBJS)

	cmd /k $(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BIN_NAME)
CC = g++
CFLAGS = -Wall -std=c++11
EXEC_NAME = visibleSimCatoms2DConfigTool
INCLUDES =
LIBS = -ltinyxml
OBJ_FILES = arguments.o cell.o vector2D.o vector3D.o configuration.o lattice.o visibleSimCatoms2DConfigTool.o

all : $(EXEC_NAME)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

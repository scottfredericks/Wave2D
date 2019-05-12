OBJS = main.cpp

OBJ_NAME = RunWave

all: $(OBJS)
	g++ $(OBJS) -w -lSDL2 -o $(OBJ_NAME)

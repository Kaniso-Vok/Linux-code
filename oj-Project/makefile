LDFLAGS=-L /home/chuhailong/Linux-code/oj-Project/third_part/lib
DEBUGFLAGS=-g
BIN=./Bin/oj_server
CC=g++

.PHONY:$(BIN)
$(BIN):oj_svr.cpp
		  $(CC) $^ -o $@  $(DEBUGFLAGS) $(LDFLAGS) -lctemplate -ljsoncpp -lpthread

.PHONY:clean
clean:
		rm -f $(BIN)

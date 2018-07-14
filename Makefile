CC = g++
CFLAGS = -g -Wall -std=c++0x
OPENCV = `pkg-config --cflags --libs opencv`
SRC = *.cpp
PROG = detect

$(PROG) : $(SRC)
		$(CC) $(CFLAGS) -o $(PROG) $(SRC) $(OPENCV)


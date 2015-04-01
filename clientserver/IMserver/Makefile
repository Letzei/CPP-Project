CC = clang++
CXX = clang++
CFLAGS = -g -Wall -Wextra
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++11
LDFLAGS = -L..
LDLIBS = -lclientserver

.PHONY: all clean

all: IMserver IMclient

IMserver: IMserver.o database.o messagehandler.o newsgroup.o article.o
IMclient: IMclient.o messagehandler.o 

clean:
	rm -f *.o *.d

run: clean IMserver IMclient
	./IMserver 1337

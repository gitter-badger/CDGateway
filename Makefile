FLAGS=-I./include -L./lib -Wl,-rpath=/usr/local/lib -pthread -lboost_system -ldl -lrt -lm ./lib/libevent.a 
CXXFLAGS = -g
CFLAGS = -g
OBJS=src/main.cpp src/client.cpp src/server.cpp src/eventutil.c
TAGS=src/main.o src/client.o src/server.o src/eventutil.o

.PHONY:all clean
all:gate client

clean:
	rm -f gate *.o $(TAGS)
gate:$(TAGS)
	g++ -g $(TAGS) -o $@ $(FLAGS)
client:test/client.c
	gcc $^ -o $@ $(FLAGS)

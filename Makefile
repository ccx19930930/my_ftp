SOURCES_SERVER=$(wildcard src/server/*.c)
SOURCES_CLIENT=$(wildcard src/client/*.c)

OBJS_SERVER=$(patsubst *.c,*.o,$(SOURCES_SERVER))
OBJS_CLIENT=$(patsubst *.c,*.o,$(SOURCES_CLIENT))

ELF:server client

server:$(OBJS_SERVER)
		gcc -o $@ $^ -lpthread -I /usr/local/mysql/include/ -L /usr/local/mysql/lib/ -l mysqlclient -I./include/server/ 
		mkdir ./bin/server -p
		mv server ./bin/server/
client:$(OBJS_CLIENT)
		gcc -o $@ $^ -lpthread -I./include/client/ 
		mkdir ./bin/client -p
		mv client ./bin/client/
.PHONY:clean
clean:
	rm ./bin/server/server ./bin/client/client

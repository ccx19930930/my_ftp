SOURCES=$(wildcard *.c)

OBJS=$(patsubst *.c,*.o,$(SOURCES))

ELF:=server

$(ELF):$(OBJS)
		 gcc -o $@ $^ -lpthread -I../../include/server/

.PHONY:clean
clean:
	rm $(ELF) 

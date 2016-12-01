SOURCES=$(wildcard *.c)

OBJS=$(patsubst *.c,*.o,$(SOURCES))

ELF:=server

$(ELF):$(OBJS)
		 gcc -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm $(ELF) 

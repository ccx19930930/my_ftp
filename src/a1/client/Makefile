SOURCES=$(wildcard *.c)

OBJS=$(patsubst *.c,*.o,$(SOURCES))

ELF:=client

$(ELF):$(OBJS)
		 gcc -o $@ $^ 

.PHONY:clean
clean:
	rm $(ELF) 

OBJ=$(patsubst %.c,%.o,$(wildcard ./*.c))
TARGET=select

${TARGET}:${OBJ}
	cc $^ -o $@
%.o:%.c
	cc -c $^ -o $@	
	
.PHONY:
clean:
	$(RM)  *.o 
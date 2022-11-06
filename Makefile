CC=gcc
CPP=g++
# CFLAGS= -c -O2
CFLAGS= -c -g $(shell pkg-config --cflags opencv4)

LFLAGS=$(shell pkg-config --libs opencv4)

C_SRC=$(wildcard *.c)
CPP_SRC=$(wildcard *.cpp)
C_OBJ=$(C_SRC:%.c=%.o)
CPP_OBJ=$(CPP_SRC:%.cpp=%.o)
PROG=image_corners

all: ${PROG}

${PROG}: ${C_OBJ} ${CPP_OBJ}
	${CPP} -o $@ $^ ${LFLAGS} 

${C_OBJ}: ${C_SRC}
	${CC} -fPIC ${CFLAGS} $^

${CPP_OBJ}: ${CPP_SRC}
	${CPP} -fPIC ${CFLAGS} $^

clean:
	rm -f *.o
	rm -f ${PROG}

SRCS = chess_framework.c
OBJS = ${SRCS:.c=.o}
HDS = ${SRCS:.c=.h}

all: main

main: main.c ${OBJS}
	gcc main.c ${OBJS} -o main

.c.o: ${SRCS} ${HDS}
	gcc -c $<

clean:
	rm -f main ${OBJS}

BB_generate: capture_BB_generator.c quiet_BB_generator.c ${OBJS}
	gcc capture_BB_generator.c ${OBJS} -o capture_BB_generator
	gcc quiet_BB_generator.c ${OBJS} -o quiet_BB_generator

	./capture_BB_generator
	./quiet_BB_generator
	mkdir -p captureBB/
	mkdir -p quietBB/
	mv *capture.txt ./captureBB
	mv *quiet.txt ./quietBB
	chmod 777 ./captureBB/*
	chmod 777 ./quietBB/*

BB_clean:
	rm -f capture_BB_generator quiet_BB_generator captureBB/* quietBB/*
	rm -d -f captureBB/ quietBB/

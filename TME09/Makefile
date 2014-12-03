CC=gcc
CFLAGS=-W -Wall
LDFLAGS= -lrt -lpthread
INCFLAGS=-I$(INC)
BIN=bin
LIB=lib
INC=include
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

ifeq ($(SUM),1)
CFLAGS += -DSUM
endif

all: directories EX1 EX2 EX3 EX4 EX5 EX6 EX7

directories: ${OBJ} ${BIN} #${LIB}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}
${LIB}:
	mkdir -p ${LIB}

# =====
# OBJET
# =====
${OBJ}/ex1.o: ${SRC}/ex1.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ex2.o: ${SRC}/ex2.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ex3.o: ${SRC}/ex3.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ex4.o: ${SRC}/ex4.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ex5.o: ${SRC}/ex5.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ex6.o: ${SRC}/ex6.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ex7.o: ${SRC}/ex7.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libexample.a : ${OBJ}/example.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
EX1: ${OBJ}/ex1.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EX2: ${OBJ}/ex2.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EX3: ${OBJ}/ex3.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EX4: ${OBJ}/ex4.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EX5: ${OBJ}/ex5.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EX6: ${OBJ}/ex6.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EX7: ${OBJ}/ex7.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runEX1 runEX2 runEX3 runEX4 runEX5 runEX6 runEX7

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~ fic1 fic2 fic3 target

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runEX1:
	@echo "EX1"
	./$(BIN)/EX1 fic1 "test exercice1" 
	@echo

runEX2:
	@echo "EX2"
	./$(BIN)/EX2 fic2 "test exercice2" 
	@echo

runEX3:
	@echo "EX3"
	./$(BIN)/EX3 fic3
	@echo

runEX4:
	@echo "EX4"
	./$(BIN)/EX4 source target
	@echo

runEX5:
	@echo "EX5"
	./$(BIN)/EX5
	@echo

runEX6:
	@echo "EX6"
	./$(BIN)/EX6
	@echo

runEX7:
	@echo "EX7"
	./$(BIN)/EX7
	@echo

bench: runEX1 runEX2 runEX3 runEX4 runEX5 runEX6 runEX7

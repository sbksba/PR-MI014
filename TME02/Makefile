CC=gcc
CFLAGS=-Wall -ansi
LDFLAGS=-lrt -lpthread
INCFLAGS= -I$(INC)
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

all: directories CREATE EXCLUSION SYNCHRO DAEMON DAEMON2 BROADCAST

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}
${LIB}:
	mkdir -p ${LIB}

# =====
# OBJET
# =====
${OBJ}/create.o: ${SRC}/create.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/exclusion.o: ${SRC}/exclusion.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/synchro.o: ${SRC}/synchro.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/daemon.o: ${SRC}/daemon.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/daemonBIS.o: ${SRC}/daemonBIS.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/broadcast.o: ${SRC}/broadcast.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

# ============
# BIBLIOTHEQUE
# ============

# ==========
# EXECUTABLE
# ==========
CREATE: ${OBJ}/create.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EXCLUSION: ${OBJ}/exclusion.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SYNCHRO: ${OBJ}/synchro.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

DAEMON: ${OBJ}/daemon.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

DAEMON2: ${OBJ}/daemonBIS.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

BROADCAST: ${OBJ}/broadcast.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runCREATE runEXCLU runDAEMON runDAEMON2 runBROADCAST

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runCREATE:
	@echo "CREATE"
	./$(BIN)/CREATE
	@echo

runEXCLU:
	@echo "EXCLUSION"
	./$(BIN)/EXCLUSION
	@echo

runSYNCHRO:
	@echo "SYNCHRO"
	./$(BIN)/SYNCHRO
	@echo

runDAEMON:
	@echo "DAEMON"
	./$(BIN)/DAEMON
	@echo

runDAEMON2:
	@echo "DAEMON2"
	./$(BIN)/DAEMON2
	@echo

runBROADCAST:
	@echo "BROADCAST"
	./$(BIN)/BROADCAST
	@echo

bench: runCREATE runEXCLU runSYNCHRO runDAEMON runDAEMON2 runBROADCAST

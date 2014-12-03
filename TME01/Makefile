CC=gcc
CFLAGS=-Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

all: directories MAX_CALCUL NFORK SLEEP CHFORK SYNCHRO KILL SIGNAUX

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
${OBJ}/max_func.o: ${SRC}/max_func.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/max_calcul.o: ${SRC}/max_calcul.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/nfork.o: ${SRC}/nfork.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/sleep.o: ${SRC}/sleep.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/chfork.o: ${SRC}/chfork.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/synchro.o: ${SRC}/synchro.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/kill.o: ${SRC}/kill.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/signaux.o: ${SRC}/signaux.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

# ============
# BIBLIOTHEQUE
# ============

# ==========
# EXECUTABLE
# ==========
MAX_CALCUL: ${OBJ}/max_func.o ${OBJ}/max_calcul.o
	${CC} -o ${BIN}/$@ $^

NFORK: ${OBJ}/nfork.o
	${CC} -o ${BIN}/$@ $^

SLEEP: ${OBJ}/sleep.o
	${CC} -o ${BIN}/$@ $^

CHFORK: ${OBJ}/chfork.o
	${CC} -o ${BIN}/$@ $^

SYNCHRO: ${OBJ}/synchro.o
	${CC} -o ${BIN}/$@ $^

KILL: ${OBJ}/kill.o
	${CC} -o ${BIN}/$@ $^

SIGNAUX: ${OBJ}/signaux.o
	${CC} -o ${BIN}/$@ $^

.PHONY: all proper clean cleanall runCALCUL runNFORK runSLEEP runCHFORK runSYNCHRO runKILL runSIGNAUX

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runCALCUL:
	@echo "MAX_CALCUL"
	./$(BIN)/MAX_CALCUL 4 8 12 7
	@echo

runNFORK:
	@echo "NFORK"
	./$(BIN)/NFORK
	@echo

runSLEEP:
	@echo "SLEEP"
	./$(BIN)/SLEEP
	@echo

runCHFORK:
	@echo "CHFORK"
	./$(BIN)/CHFORK 3
	@echo

runSYNCHRO:
	@echo "SYNCHRO"
	./$(BIN)/SYNCHRO
	@echo

runKILL:
	@echo "KILL"
	./$(BIN)/KILL
	@echo

runSIGNAUX:
	@echo "SIGNAUX"
	./$(BIN)/SIGNAUX
	@echo

bench: runCALCUL runNFORK runSLEEP runCHFORK runSYNCHRO runKILL runSIGNAUX

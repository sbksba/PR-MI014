CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-L $(LIB) -lrt -lpthread -lthread
INCFLAGS=-I$(INC)
BIN=bin
LIB=lib
INC=include
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

all: directories UNLIMITED POOL PRODUC PRODUCS CHAINE

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
${OBJ}/common.o: ${SRC}/common.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/thread_stack.o: ${SRC}/thread_stack.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/unlimited.o: ${SRC}/unlimited.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/pool.o: ${SRC}/pool.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/produc.o: ${SRC}/produc.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/producS.o: ${SRC}/producS.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/chaine.o: ${SRC}/chaine.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libthread.a : ${OBJ}/common.o ${OBJ}/thread_stack.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
UNLIMITED: $(LIB)/libthread.a ${OBJ}/unlimited.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

POOL: $(LIB)/libthread.a ${OBJ}/pool.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

PRODUC: $(LIB)/libthread.a ${OBJ}/produc.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

PRODUCS: $(LIB)/libthread.a ${OBJ}/producS.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

CHAINE: $(LIB)/libthread.a ${OBJ}/chaine.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runUNLIMITED runPOOL runPRODUC runPRODUCS runCHAINE

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runUNLIMITED:
	@echo "UNLIMITED"
	./$(BIN)/UNLIMITED test/toto test/tata test/titi
	@echo

runPOOL:
	@echo "POOL"
	./$(BIN)/POOL test/toto test/tata test/titi test/tutu
	@echo

runPRODUC:
	@echo "PRODUC"
	./$(BIN)/PRODUC
	@echo

runPRODUCS:
	@echo "PRODUCS"
	./$(BIN)/PRODUCS
	@echo

runCHAINE:
	@echo "CHAINE"
	./$(BIN)/CHAINE
	@echo

bench: runUNLIMITED runPOOL runPRODUC runPRODUCS runCHAINE

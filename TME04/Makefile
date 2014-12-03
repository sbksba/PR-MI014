CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-L $(LIB) -lrt -lpthread -lproduc
INCFLAGS=-I$(INC)
BIN=bin
LIB=lib
INC=include
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

all: directories FILEMSG MEMORYMSG SERVERMULTI SERVERUNIQ RDVSEM PRODUC

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
${OBJ}/thread_stack.o: ${SRC}/thread_stack.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/fileMSG.o: ${SRC}/fileMSG.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/memoryMSG.o: ${SRC}/memoryMSG.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/serverMultiFile.o: ${SRC}/serverMultiFile.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/serverUniq.o: ${SRC}/serverUniq.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/rdvSem.o: ${SRC}/rdvSem.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/produc.o: ${SRC}/produc.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libproduc.a : ${OBJ}/thread_stack.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
FILEMSG: $(LIB)/libproduc.a ${OBJ}/fileMSG.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

MEMORYMSG: $(LIB)/libproduc.a ${OBJ}/memoryMSG.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SERVERMULTI: $(LIB)/libproduc.a ${OBJ}/serverMultiFile.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SERVERUNIQ: $(LIB)/libproduc.a ${OBJ}/serverUniq.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

RDVSEM: $(LIB)/libproduc.a ${OBJ}/rdvSem.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

PRODUC: $(LIB)/libproduc.a ${OBJ}/produc.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runFILEMSG runMEMORYMSG runSERVERMULTI runSERVERUNIQ runRDVSEM runPRODUC

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runFILEMSG:
	@echo "FILEMSG"
	./$(BIN)/FILEMSG
	@echo

runMEMORYMSG:
	@echo "MEMORYMSG"
	./$(BIN)/MEMORYMSG
	@echo

runSERVERMULTI:
	@echo "SERVERMULTI"
	./$(BIN)/SERVERMULTI
	@echo

runSERVERUNIQ:
	@echo "SERVERUNIQ"
	./$(BIN)/SERVERUNIQ
	@echo

runRDVSEM:
	@echo "RDVSEM"
	./$(BIN)/RDVSEM
	@echo

runPRODUC:
	@echo "PRODUC"
	./$(BIN)/PRODUC
	@echo

bench: runFILEMSG runMEMORYMSG runSERVERMULTI runSERVERUNIQ runRDVSEM runPRODUC

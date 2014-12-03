CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-lrt
INCFLAGS=-I$(INC)
BIN=bin
LIB=lib
INC=include
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

all: directories MQEXAMPLE SHAREMEM RDVSEM SKIPSHARE SKIPCLIENT NIETZSCHEBROT

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
${OBJ}/mq_example.o: ${SRC}/mq_example.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/shareMemory.o: ${SRC}/shareMemory.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/rdvSem.o: ${SRC}/rdvSem.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/skipShare.o: ${SRC}/skipShare.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/skipClient.o: ${SRC}/skipClient.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/nietzscheBrot.o: ${SRC}/nietzscheBrot.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAG}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libexample.a : ${OBJ}/example.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
MQEXAMPLE: ${OBJ}/mq_example.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SHAREMEM: ${OBJ}/shareMemory.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

RDVSEM: ${OBJ}/rdvSem.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SKIPSHARE: ${OBJ}/skipShare.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SKIPCLIENT: ${OBJ}/skipClient.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

NIETZSCHEBROT: ${OBJ}/nietzscheBrot.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runMQEXAMPLE runSHAREMEM runRDVSEM runSKIPSHARE runSKIPCLIENT1 runSKIPCLIENT2 runNIETZSCHEBROT

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runMQEXAMPLE:
	@echo "MQEXAMPLE"
	./$(BIN)/MQEXAMPLE
	@echo

runSHAREMEM:
	@echo "SHAREMEM"
	./$(BIN)/SHAREMEM
	@echo

runRDVSEM:
	@echo "RDVSEM"
	./$(BIN)/RDVSEM
	@echo

runSKIPSHARE:
	@echo "SKIPSHARE"
	./$(BIN)/SKIPSHARE skip1
	@echo

runSKIPCLIENT1:
	@echo "SKIPCLIENT1"
	./$(BIN)/SKIPCLIENT cl1 skip1
	@echo

runSKIPCLIENT2:
	@echo "SKIPCLIENT2"
	./$(BIN)/SKIPCLIENT cl2 skip1
	@echo

runNIETZSCHEBROT:
	@echo "NIETZSCHEBROT"
	./$(BIN)/NIETZSCHEBROT
	@echo

bench: runMQEXAMPLE runSHAREMEM runRDVSEM runNIETZSCHEBROT

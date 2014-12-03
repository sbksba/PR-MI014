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

all: directories FTPSERVERTHREAD FTPCLIENT WARMCHAT SONAR PONGER CONNECTSERVER CONNECTCLIENT

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
${OBJ}/ftpServerThread.o: ${SRC}/ftpServerThread.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ftpClient.o: ${SRC}/ftpClient.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/warmChat.o: ${SRC}/warmChat.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/sonar.o: ${SRC}/sonar.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ponger.o: ${SRC}/ponger.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/connectServer.o: ${SRC}/connectServer.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/connectClient.o: ${SRC}/connectClient.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libexample.a : ${OBJ}/example.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
FTPSERVERTHREAD: ${OBJ}/ftpServerThread.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

FTPCLIENT: ${OBJ}/ftpClient.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

WARMCHAT: ${OBJ}/warmChat.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SONAR: ${OBJ}/sonar.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

PONGER: ${OBJ}/ponger.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

CONNECTSERVER: ${OBJ}/connectServer.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

CONNECTCLIENT: ${OBJ}/connectClient.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runFTPSERVERTHREAD runFTPCLIENT runWARMCHAT1 runWARMCHAT2 runSONAR runPONGER runCONNECTSERVER runCONNECTCLIENT1 runCONNECTCLIENT2

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~ ic1

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runFTPSERVERTHREAD:
	@echo "FTP SERVER THREAD"
	./$(BIN)/FTPSERVERTHREAD
	@echo

runFTPCLIENT:
	@echo "FTP CLIENT"
	./$(BIN)/FTPCLIENT
	@echo

runWARMCHAT1:
	@echo "WARM CHAT I"
	./$(BIN)/WARMCHAT 225.0.0.10 1234 toto
	@echo

runWARMCHAT2:
	@echo "WARM CHAT II"
	./$(BIN)/WARMCHAT 225.0.0.10 1234 titi
	@echo

runSONAR:
	@echo "SONAR"
	./$(BIN)/SONAR
	@echo

runPONGER:
	@echo "PONGER"
	./$(BIN)/PONGER
	@echo

runCONNECTSERVER:
	@echo "CONNECT SERVER"
	./$(BIN)/CONNECTSERVER 1234 5678
	@echo

runCONNECTCLIENT1:
	@echo "CONNECT CLIENT I"
	./$(BIN)/CONNECTCLIENT localhost 1234
	@echo

runCONNECTCLIENT2:
	@echo "CONNECT CLIENT II"
	./$(BIN)/CONNECTCLIENT localhost 5678
	@echo

bench:

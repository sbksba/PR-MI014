CC=gcc
CFLAGS=-W -Wall
LDFLAGS=
INCFLAGS=-I$(INC)
BIN=bin
LIB=lib
INC=include
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

all: directories EX1 SENDFILE RECVFILE SERVERENV CLIENTENV FTPSERVER FTPCLIENT

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

${OBJ}/sendfile.o: ${SRC}/sendfile.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/recvfile.o: ${SRC}/recvfile.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/serverENV.o: ${SRC}/serverENV.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/clientENV.o: ${SRC}/clientENV.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ftpServer.o: ${SRC}/ftpServer.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ftpClient.o: ${SRC}/ftpClient.c
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

SENDFILE: ${OBJ}/sendfile.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

RECVFILE: ${OBJ}/recvfile.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

SERVERENV: ${OBJ}/serverENV.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

CLIENTENV: ${OBJ}/clientENV.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

FTPSERVER: ${OBJ}/ftpServer.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

FTPCLIENT: ${OBJ}/ftpClient.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runEX1 runSENDFILE runRECVFILE runSERVERENV runCLIENTENV runFTPSERVER runFTPCLIENT

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~ ic1

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}

runEX1:
	@echo "EX1"
	./$(BIN)/EX1
	@echo

runSENDFILE:
	@echo "SENFILE"
	./$(BIN)/SENDFILE localhost 1234 FTP_SERVER/fic1
	@echo

runRECVFILE:
	@echo "RECVFILE"
	./$(BIN)/RECVFILE 1234
	@echo

runSERVERENV:
	@echo "SERVER ENV"
	./$(BIN)/SERVERENV
	@echo

runCLIENTENV:
	@echo "CLIENT ENV"
	./$(BIN)/CLIENTENV localhost
	@echo

runFTPSERVER:
	@echo "FTP SERVER"
	./$(BIN)/FTPSERVER
	@echo

runFTPCLIENT:
	@echo "FTP CLIENT"
	./$(BIN)/FTPCLIENT
	@echo

bench: runEX1

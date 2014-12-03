CC=gcc
CFLAGS=-W -Wall
LDFLAGS=
INCFLAGS=-I$(INC)
BIN=bin
LIB=lib
INC=include
OBJ=obj
SRC=src
TEST=test

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
endif

all: directories EX1 FEEDBACKES FEEDBACKMAP ASCENTFILE VERSUSPOSIX VERSUSC EXTENDGREP LSEEKINV PREADINV

directories: ${OBJ} ${BIN} ${TEST} #${LIB}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}
${LIB}:
	mkdir -p ${LIB}

${TEST}:
	mkdir -p ${TEST}
	chmod -R 755 ${TEST}
	touch ${TEST}/search
	echo " Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam scelerisque eu dolor in moltie, commodo nisi accumsan, eleifend ligula. search viverra. Quisque eu bibendum libero. Aliquam in lacus urna. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Ut porttitor justo non ex mattis porttitor id id dui. Aenean in velit nisi. Etiam consectetur lacus ante, ut blandit turpis sodales nec. Curabitur sed posuere elit. " > ${TEST}/search
	echo "fezpoifhzeouifh ezofhzeoufehzfo zeofhzefozeh z search eoifhzeoufhzeoiufzhe" > ${TEST}/toto
	echo "fezpoifhzeouifh ezofhzeoufehzfo zeofhzefozeh z eoifhzeoufhzeoiufzhe" > ${TEST}/tata

# =====
# OBJET
# =====
${OBJ}/ex1.o: ${SRC}/ex1.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/feedbackES.o: ${SRC}/feedbackES.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/feedbackMAP.o: ${SRC}/feedbackMAP.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/ascentFile.o: ${SRC}/ascentFile.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/versusPOSIX.o: ${SRC}/versusPOSIX.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/versusC.o: ${SRC}/versusC.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/extendGrep.o: ${SRC}/extendGrep.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/lseekInv.o: ${SRC}/lseekInv.c
	${CC} -c -o $@ $< $(CFLAGS) ${INCFLAGS}

${OBJ}/preadInv.o: ${SRC}/preadInv.c
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

FEEDBACKES: ${OBJ}/feedbackES.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

FEEDBACKMAP: ${OBJ}/feedbackMAP.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

ASCENTFILE: ${OBJ}/ascentFile.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

VERSUSPOSIX: ${OBJ}/versusPOSIX.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

VERSUSC: ${OBJ}/versusC.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

EXTENDGREP: ${OBJ}/extendGrep.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

LSEEKINV: ${OBJ}/lseekInv.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

PREADINV: ${OBJ}/preadInv.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall runEX1 runFEEDBACKES runFEEDBACKMAP runASCENTFILE runVERSUSPOSIX runVERSUSC runEXTENDGREP runLSEEKINV runPREADINV

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~ fich1 fic2 fic3 share fic1.inv

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/* ${TEST}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB} ${TEST}

runEX1:
	@echo "EX1"
	./$(BIN)/EX1
	@echo

runFEEDBACKES:
	@echo "FEEDBACKES"
	./$(BIN)/FEEDBACKES fic1 fic2
	@echo

runFEEDBACKMAP:
	@echo "FEEDBACKMAP"
	./$(BIN)/FEEDBACKMAP fic1 fic3
	@echo

runASCENTFILE:
	@echo "ASCENTFILE"
	./$(BIN)/ASCENTFILE share
	@echo

runVERSUSPOSIX:
	@echo "VERSUSPOSIX"
	./$(BIN)/VERSUSPOSIX fic1
	@echo

runVERSUSC:
	@echo "VERSUSC"
	./$(BIN)/VERSUSC fic1
	@echo

runEXTENDGREP:
	@echo "EXTENDGREP"
	./$(BIN)/EXTENDGREP "search" test
	@echo

runLSEEKINV:
	@echo "LSEEKINV"
	./$(BIN)/LSEEKINV fic1
	@echo

runPREADINV:
	@echo "PREADINV"
	./$(BIN)/PREADINV fic1
	@echo

bench: runFEEDBACKES runFEEDBACKMAP runASCENTFILE runVERSUSPOSIX runVERSUSC runEXTENDGREP runLSEEKINV runPREADINV

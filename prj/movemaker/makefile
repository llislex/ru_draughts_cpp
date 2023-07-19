#
CC=g++
CFLAGS=-Wall

INC += \
    -I../../src/utils \
    -I../../src/ut \
    -I../../src/rules \
    -I../../src/ai

SRC += \
    ../../src/rules/board.cpp \
    ../../src/rules/rules.cpp \
    ../../src/ut/ut.cpp \
    ../../src/utils/bit_op.cpp \
    main.cpp
	
BIN = ./bin/


EXE=move_maker

all: clean $(SRC) $(EXE)
	$(BIN)$(EXE)
	
$(EXE): $(SRC) 
	$(CC) $(INC) $(CFLAGS) $^ -o $(BIN)$@
	
clean:
	rm -rf $(BIN)
	mkdir -p $(BIN)
# Copyright 2020-2021, Hadrien Barral, Arthur Leonard, Samuel Vivien and the CrazySums contributors
# SPDX-License-Identifier: GPL-3.0-only

MAKEFLAGS += --no-builtin-rules --no-builtin-variables
.SUFFIXES:

BIN := crazysums
#GPROF := -pg
OPT := -O3 -flto
#OPT := -O2

ifndef CXX_TOOL
CXX_TOOL := g++
endif
ifeq ($(CXX_TOOL), g++)
	LFLAGS += -lstdc++fs
endif
ifeq ($(CXX_TOOL), clang)
	LFLAGS += -lstdc++
endif

all: build run

build: $(BIN)

-include $(BIN).d

$(BIN): main.cpp Makefile
	$(CXX_TOOL) -o "$@" $< -Wall -Wextra -std=c++17 $(OPT) -march=native $(LFLAGS) -lpthread -MMD -g \
	    $(GPROF) -DHAS_COLOR ${EXTRA} \

run:
	./$(BIN)
	@@echo Generating pdf from LaTeX logs...
	@(cd tex && pdflatex --interaction=batchmode -halt-on-error logs.tex \
	    | { grep -v "This is pdfTeX, " || true; } \
	    | { grep -v "restricted \\\\write18 enabled" || true; } \
	    | { grep -v "entering extended mode" || true; } \
	)

timer: build runT

runT:
	time ./$(BIN)

clean:
	rm -rf $(BIN) $(BIN).d tex

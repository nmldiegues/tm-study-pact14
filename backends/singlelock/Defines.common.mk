CC       := gcc
CFLAGS   += -g -Wall -w -pthread
CFLAGS   += -O3
CFLAGS   += -I$(LIB) -I../rapl-power/
CPP      := g++
CPPFLAGS += $(CFLAGS)
LD       := g++
LIBS     += -lpthread

LIB := ../lib

STM := ../tl2

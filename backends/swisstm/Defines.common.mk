CC       := gcc
CFLAGS   += -g -Wall -pthread -fno-strict-aliasing
CFLAGS   += -O3
CFLAGS   += -I$(LIB) -I../rapl-power/
CFLAGS   += -static
CPP      := g++
CPPFLAGS += $(CFLAGS)
LD       := g++
LIBS     += -pthread

LIB := ../lib

STM := ../swissTM

CFLAGS += -DOUTPUT_TO_STDOUT

PROG := kmeans-locks

SRCS += \
	cluster.c \
	common.c \
	kmeans-locks.c \
	normal.c \
	$(LIB)/mt19937ar.c \
	$(LIB)/random.c \
	$(LIB)/thread.c \
#
OBJS := ${SRCS:.c=.o}

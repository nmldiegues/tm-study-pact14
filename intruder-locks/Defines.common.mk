PROG := intruder-locks

SRCS += \
	decoder.c \
	detector.c \
	dictionary.c \
	intruder-locks.c \
	packet.c \
	preprocessor.c \
	stream.c \
	$(LIB)/list.c \
	$(LIB)/mt19937ar.c \
	$(LIB)/pair.c \
	$(LIB)/queue.c \
	$(LIB)/random.c \
        $(LIB)/hash.c \
	$(LIB)/hashtable.c \
	$(LIB)/thread.c \
	$(LIB)/vector.c \
#
OBJS := ${SRCS:.c=.o}

CFLAGS += -DMAP_USE_HASHTABLE

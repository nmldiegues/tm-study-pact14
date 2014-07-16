PROG := redblacktree

SRCS += \
	redblacktree.c \
	rbtree.c \
	$(LIB)/mt19937ar.c \
	$(LIB)/random.c \
	$(LIB)/thread.c \
#
OBJS := ${SRCS:.c=.o}

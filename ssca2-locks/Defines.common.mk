PROG := ssca2-locks

SRCS += \
	alg_radix_smp.c \
	computeGraph.c \
	createPartition.c \
	cutClusters.c \
	findSubGraphs.c \
	genScalData.c \
	getStartLists.c \
	getUserParameters.c \
	globals.c \
	ssca2-locks.c \
	$(LIB)/mt19937ar.c \
	$(LIB)/random.c \
	$(LIB)/thread.c \
#
OBJS := ${SRCS:.c=.o}

#CFLAGS += -DUSE_PARALLEL_DATA_GENERATION
#CFLAGS += -DWRITE_RESULT_FILES
CFLAGS += -DENABLE_KERNEL1
#CFLAGS += -DENABLE_KERNEL2 -DENABLE_KERNEL3
#CFLAGS += -DENABLE_KERNEL4

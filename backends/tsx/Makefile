ifndef LOCK_VERSION
  LOCK_VERSION=-DUSE_TTAS_LOCKS
endif

ifndef PROB
  PROB=-DFALLBACK_PROB=10
endif

CFLAGS   += -w -DHTM -I$(STM)/include $(LOCK_VERSION) $(PROB) 

CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM)
LIBS     += -lsync -lrt -lpthread -lnuma -mrtm -L../rapl-power -lrapl

# ==============================================================================
# Rules
# ==============================================================================

.PHONY: default
default: $(PROG)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG) $(OUTPUT)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(PROG) -static

include ../common/Makefile.common


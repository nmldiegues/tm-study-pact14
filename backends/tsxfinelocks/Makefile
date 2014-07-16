ifndef LOCK_VERSION
  LOCK_VERSION=-DUSE_TTAS_LOCKS
endif

CFLAGS   += -DHTM $(LOCKS) -I$(STM)/include $(LOCK_VERSION) # We substitute our RTM implementation for HTM.                                                                                                                                                
CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM)
LIBS     += -lsync -lrt -lpthread -lnuma -mrtm -L../rapl-power -lrapl

.PHONY: default
default: $(PROG)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG) $(OUTPUT)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(PROG) -static

include ../common/Makefile.common

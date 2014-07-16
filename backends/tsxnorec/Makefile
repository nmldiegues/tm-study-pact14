ifndef PROB
  PROB=-DFALLBACK_PROB=10
endif

CFLAGS   += -w -DHTM -I$(STM) $(PROB) # We substitute our RTM implementation for HTM.   

CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM)
LIBS     += -lnorec -lrt -lpthread -mrtm -L../rapl-power -lrapl

.PHONY: default
default: $(PROG)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG) $(OUTPUT)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(PROG) -static

include ../common/Makefile.common

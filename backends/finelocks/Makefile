CFLAGS   += -w -DHTM $(LOCKS) -I$(STM)
CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM)
LIBS     += -ltl2 -L../rapl-power -lrapl

.PHONY: default
default: $(PROG)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG) $(OUTPUT)

include ../common/Makefile.common

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(PROG)

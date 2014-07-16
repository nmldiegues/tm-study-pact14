CFLAGS   += -w -DSTM -I$(STM)
CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM)
LIBS     += -lnorec -L../rapl-power -lrapl

.PHONY: default
default: $(PROG)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG) $(OUTPUT)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(PROG)

include ../common/Makefile.common

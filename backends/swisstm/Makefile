CFLAGS   += -w -DSTM -DWLPDSTM -I$(STM)/include
CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM)/lib -static
LIBS     += -lwlpdstm -L../rapl-power -lrapl


.PHONY: default
default: $(PROG)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG) $(OUTPUT)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(PROG)

include ../common/Makefile.common

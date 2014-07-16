.PHONY: default
default: $(PROG)

LIBS += -L../rapl-power -lrapl

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG) $(OUTPUT)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(PROG)

include ../common/Makefile.common

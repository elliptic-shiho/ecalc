SRCDIR = src
INCDIR = include
BINDIR = bin

include src/Makefile

CC = gcc
LD = gcc
RM = rm
CD = cd
AS = nasm
CFLAGS = -g -O0 -Wall -Wextra -I$(INCDIR) -D_FORTIFY_SOURCE=2 -masm=intel
#CFLAGS = -Wall -Wextra -I$(INCDIR) -D_FORTIFY_SOURCE=2 -masm=intel -DNDEBUG
LDFLAGS = -lm
ASFLAGS = -f elf

.PHONY: all
all: ec

ec: $(OBJS:p/%=$(BINDIR)/%)
	$(LD) -o $(BINDIR)/$@ $(LDFLAGS) $(OBJS:p/%=$(BINDIR)/%)

bin/%.o: src/%.c $(INCDIR)/*.h
	$(CC) -o $@ -c $(CFLAGS) $<

clean:
	$(RM) -f $(BINDIR)/*.o
	$(RM) -f $(BINDIR)/ec

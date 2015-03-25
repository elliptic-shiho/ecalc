SRCDIR = src
INCDIR = include
BINDIR = bin

include src/Makefile

CFLAGS           = -Wall -Wextra -I$(INCDIR) -D_FORTIFY_SOURCE=2 -masm=intel -std=c99
PKG-CONFIG      := pkg-config
CC              := gcc
LD              := gcc
RM              := rm
CD              := cd
AS              := nasm
MAKE            := make
MKDIR           := mkdir -p
DEBUG_FLAGS     := -O0 -g
RELEASE_FLAGS   := -O3 -DNDEBUG
MODE            := DEBUG
LDFLAGS         := -lm -lgmp
ASFLAGS         := -f elf
TARGET          := ec
VPATH           := $(SRCDIR)
EXTRA_FLAGS      = $(DEBUG_FLAGS)

.PHONY: all
all: $(BINDIR) release

.PHONY: debug
debug: EXTRA_FLAGS = $(DEBUG_FLAGS)
debug: $(TARGET)

.PHONY: release
release: EXTRA_FLAGS = $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(foreach O,$(OBJS),$(BINDIR)/$O)
	$(LD) -o $(BINDIR)/$@ $(LDFLAGS) $(EXTRA_FLAGS) $^

$(BINDIR)/%.o: %.c $(INCDIR)/*.h
	$(CC) -o "$@" -c $(CFLAGS) $(EXTRA_FLAGS) "$<"

$(BINDIR):
	$(MKDIR) $(BINDIR)

.PHONY: clean
clean:
	$(RM) -f $(BINDIR)/*.o
	$(RM) -f $(BINDIR)/$(TARGET)

.PHONY: distclean
distclean:
	$(RM) -rf $(BINDIR)

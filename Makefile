CC = gcc

SRCDIR = src
BINDIR = build
BINARY = gameboy

INCS = -I./includes 
LIBS = `sdl2-config --cflags --libs` -lSDL2_ttf

CFLAGS = -Wall -Wextra -g $(INCS)
LDLIBS = $(LIBS)

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SRCS))

all: $(BINDIR)/$(BINARY)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/$(BINARY): $(OBJS)
	@echo "Linking $@"
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(BINDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BINDIR)/$(BINARY)

clean:
	rm  -rf $(BINDIR)

.PHONY: all run clean

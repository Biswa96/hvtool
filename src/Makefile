#Makefile for hvtool project

NAME = hvtool
BINDIR = ../bin
CFLAGS = -std=c99 -Os -m64 -Wall -Wextra
LIBS = -lole32 -lshell32 -lcomputecore -lcomputenetwork
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.obj)
BINS = $(patsubst %.obj,$(BINDIR)/%.obj,$(OBJECTS))

EXECUTABLE=$(BINDIR)/$(NAME)

all: $(BINDIR) $(EXECUTABLE)

$(EXECUTABLE): $(BINS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(BINS): $(BINDIR)/%.obj: %.c
	$(CC) -c $(CFLAGS) $< -o $(BINDIR)/$*.obj

$(BINDIR):
	@mkdir ..\bin

#END
C = gcc
IDIR = ../include
ODIR = ../obj
BDIR = ../bin

_DEPS = caesar_modules.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = caesar_cipher.o caesar_modules.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(BDIR)/caesar_cipher.exe: $(OBJ)
	$(CC) -o $@ $^

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*.exe

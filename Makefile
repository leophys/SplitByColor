all: split

OBJ:=split.o lib/fillwithblack.o lib/manppm.o
CFLAGS=-Wall

split: $(OBJ)
	$(CC) $(CFLAGS) -lm $(OBJ) -o $@

clean:
	rm -f $(OBJ)

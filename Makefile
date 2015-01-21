images_names=kodim01.png kodim02.png

all: split
images:$(addprefix testImage/,$(images_names))
.PHONY: clean test

OBJ:=split.o lib/fillwithblack.o lib/manppm.o
EXTRACFLAGS=
CFLAGS=-Wall $(EXTRACFLAGS)

split: $(OBJ)
	$(CC) $(CFLAGS) -lm $(OBJ) -o $@

testImage/kodim%.png:
	wget --quiet http://r0k.us/graphics/kodak/kodak/$(notdir $@) -O $@

clean:
	rm -f $(OBJ) split tests/createBlankImage


tests/createBlankImage: tests/createBlankImage.o lib/fillwithblack.o lib/manppm.o
	$(CC) $(CFLAGS) -lm -o $@ $^

test: tests/cuttest tests/createBlankImage split
	cd $(dir $<) && bash $(notdir $<)

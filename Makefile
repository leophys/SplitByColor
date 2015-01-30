images_names=kodim01.png kodim02.png

PREFIX ?= /usr/local
PROG=split
BINS=${PROG} tests/createBlankImage
OBJ:=split.o lib/fillwithblack.o lib/manppm.o
EXTRACFLAGS=
CFLAGS=-Wall $(EXTRACFLAGS)

all: ${PROG}
images:$(addprefix testImage/,$(images_names))
.PHONY: clean test

$(BINS):
	$(CC) $(CFLAGS) -lm -o $@ $^

${PROG}: $(OBJ)
tests/createBlankImage: tests/createBlankImage.o lib/fillwithblack.o lib/manppm.o


testImage/kodim%.png:
	wget --quiet http://r0k.us/graphics/kodak/kodak/$(notdir $@) -O $@

clean:
	rm -f $(OBJ) ${PROG} tests/createBlankImage

test: tests/cuttest tests/createBlankImage ${PROG}
	cd $(dir $<) && bash $(notdir $<)

install:
	@install -Dm755 ${PROG} ${DESTDIR}/${PREFIX}/bin/${PROG}

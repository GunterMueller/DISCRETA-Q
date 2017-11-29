
all: ../../bin/dance ../../bin/danceone ../../bin/stripcolumnsdance

../../bin/dance: dance.c
	cc -g -o ../../bin/dance dance.c

../../bin/danceone: danceone.c
	cc -o ../../bin/danceone danceone.c

../../bin/stripcolumnsdance: stripcolumnsdance.c
	cc -o ../../bin/stripcolumnsdance stripcolumnsdance.c

clean:
	rm -f ../../bin/dance ../../bin/danceone \
	../../bin/stripcolumnsdance makefile~ *.c~


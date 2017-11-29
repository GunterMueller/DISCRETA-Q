
all: ../../bin/solvediophant \
	../../bin/discretaqsolve \
	../../bin/stripcolumns \
	../../bin/solvedesign \


../../obj/diophant.o: diophant.C diophant.h
	g++ -c diophant.C -o ../../obj/diophant.o -O3

../../bin/solvediophant: solvediophant.C ../../obj/diophant.o 
	g++ -o ../../bin/solvediophant solvediophant.C ../../obj/diophant.o -lm -O3

../../bin/discretaqsolve: discretaqsolve.C ../../obj/diophant.o 
	g++ -o ../../bin/discretaqsolve discretaqsolve.C ../../obj/diophant.o -lm -O3

../../bin/stripcolumns: stripcolumns.C
	g++ -o ../../bin/stripcolumns stripcolumns.C -O3

../../bin/solvedesign: solvedesign.C
	g++ -o ../../bin/solvedesign solvedesign.C -O3

clean:
	rm -f ../../bin/solvediophant \
	../../bin/discretaqsolve \
	../../bin/stripcolumns \
	../../bin/solvedesign \
	../../obj/diophant.o



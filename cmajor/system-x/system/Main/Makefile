all: Main.o Main.o.txt

clean:
	del Main.o
	del Main.o.txt

Main.o: 
	sxas -v Main.s

Main.o.txt: 
	sxdump -v Main.o

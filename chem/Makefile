

# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I .

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I .

# fully optimized
## this may not be a good idea for chem as unrolling loops etc
# will maake code huge and this is not critical... 
#CFLAGS = -Wall -O3 -I ../ -I .
CFLAGS = -Wall  -I ../ -I .


CCXX=g++
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu

all: mylibs
	rm -f *.a
	ar cr libmychem.a *.o
	ranlib libmychem.a
#	rm -f *.o

tidy:
	rm -f *.o
	rm -f *.bak	
clean:
	rm -f *.o
	rm -f *.a
	

	
residue_properties.o : residue_properties.cpp residue_properties.h 
	rm -f residue_properties.o
	$(CCXX) $(CFLAGS) -c residue_properties.cpp
	
mylibs :  residue_properties.o


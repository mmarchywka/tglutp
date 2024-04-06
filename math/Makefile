
# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I .

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I .

# fully optimized
CFLAGS = -Wall -O3 -I ../ -I .

#;	$(CCXX) $(CFLAGS)  -nostartfiles -static -o math.lib FastNorm3.o -lm -lc

CCXX=g++
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu

all: mylibs
	ar crv libmymath.a *.o
	ranlib libmymath.a
#	rm -f *.o

tidy:
	rm -f *.o
	rm -f *.bak	
clean:
	rm -f *.o
	rm -f *.a

#modelling_matrix.o : nonuniform_wavelet_model.h modelling_matrix.cpp data_display.o
#	$(CCXX) $(CFLAGS) -c modelling_matrix.cpp
	
linear_regression_matrix.o : linear_regression_matrix.h linear_regression_matrix.cpp
	$(CCXX) $(CFLAGS) -c linear_regression_matrix.cpp

#FastNorm3.o : FastNorm3.h FastNorm3.c
#	$(CCCC) $(CFLAGS) -c FastNorm3.c
mylibs	: linear_regression_matrix.o
	
#mylibs : FastNorm3.o
	
	
	
	

	
	


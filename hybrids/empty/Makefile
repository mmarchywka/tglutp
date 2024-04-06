
# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I . -I ./view/

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I . -I ./view/

# fully optimized
CFLAGS = -Wall -O3 -I ../ -I . -I ./view/ -I /cygdrive/c/MyDocs/scripts/cc/affx/


CCXX=g++
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu
LIBS=-lm
VIEWLIBS=-L./view -lmyview  -L./atom -lmyatom  -L./ext -lmyext -L./base -lmybase -ljpeg -L./math $(LIBS) 

AFFXINC=/cygdrive/c/mydocs/scripts/cc/affx/

all: 
	
	make pdb_proc
clean:
	rm -f *.o

sterile:
	rm -f `find -name "*.o" `
	rm -f `find -name "*.a" `
	rm -f `find -name "*.bak" `
dev:
	rm -f main.o
	


pdb_proc.o : pdb_proc.cpp  
	$(CCXX) $(CFLAGS) -DSTANDALONE -c pdb_proc.cpp
	
pdb_proc :  pdb_proc.o 
	$(CCXX) $(CFLAGS) -o pdb_proc  \
	pdb_proc.o  \
	$(LIBS)

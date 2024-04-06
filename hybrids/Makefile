
# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I . -I ./view/

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I . -I ./view/

# fully optimized
CFLAGS = -Wall -O3 -I ../ -I . -I ./view/ -I /cygdrive/c/MyDocs/scripts/cc/affx/

#CFLAGS = -Wall -fpermissive -O3 -I ../ -I . -I ./view/ -I ../affx/
CFLAGS = -g -Wall -Wno-misleading-indentation  -O3 -I ../ -I . -I ./view/ -I ../affx/


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
	
../atom/libmyatom.a : 
	cd ../atom ; make  ;cd ..
	


pdb_proc.o : global.h point_list_stuff.h hybrid_expression.h neighbor_probe_classes.h find_surface_molecules.h hybrid_pdb_reader.h pdb_hybrid_typedefs.h ../atom/pdb_atom.h pdb_proc.h pdb_proc.cpp  
	$(CCXX) $(CFLAGS) -DSTANDALONE -c pdb_proc.cpp
	
pdb_proc :  pdb_proc.o ../atom/libmyatom.a 
	$(CCXX) $(CFLAGS) -o pdb_proc  \
	pdb_proc.o  \
	-L../atom -lmyatom $(LIBS)

recursive_nay_index.o : global.h enclave_base_classes.h pdb_hybrid_typedefs.h recursive_nay_index.h recursive_nay_index.cpp
	$(CCXX) $(CFLAGS) -DSTANDALONE -c recursive_nay_index.cpp

nay_db_driver : recursive_nay_index.o 
	$(CCXX) $(CFLAGS) -o nay_db_driver  \
	recursive_nay_index.o  \
	$(LIBS)

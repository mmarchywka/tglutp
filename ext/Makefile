

# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I .

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I .

# fully optimized
CFLAGS = -Wall -O3 -I ../ -I .
CFLAGS = -Wall -g -O0 -I ../ -I . -Wno-misleading-indentation 


CCXX=g++
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu
#g++ avimake.cpp -shared -I ./include -ljpeg -lmsvfw32 -lavifil32

all: mylibs
	rm -f *.a
	ar cr libmyext.a *.o
	ranlib libmyext.a
#	rm -f *.o

tidy:
	rm -f *.o
	rm -f *.bak	
clean:
	rm -f *.o
	rm -f *.a
	

iso_energy_contour.o : iso_energy_contour.cpp iso_energy_contour.h ../base/cheap_greps.h \
	atom_lists.h pdb_atom.h energy_tr.h \
	potential_param.h potential_follow.h ../math/distance_to_segments.h \
	../math/sorted_potentials.h
	rm -f iso_energy_contour.o
	$(CCXX) $(CFLAGS) -c iso_energy_contour.cpp
		
jpeg_writer.o : jpeg_writer.cpp jpeg_writer.h
	$(CCXX) $(CFLAGS) -c jpeg_writer.cpp
	
bmp_writer.o : bmp_writer.cpp bmp_header.h bmp_writer.h
	$(CCXX) $(CFLAGS) -c bmp_writer.cpp
	
bmp_reader.o : bmp_reader.cpp bmp_header.h  bmp_reader.h
	$(CCXX) $(CFLAGS) -c bmp_reader.cpp
	
mylibs :  jpeg_writer.o bmp_writer.o bmp_reader.o



# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I . -I ./view/

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I . -I ./view/

# fully optimized
CFLAGS = -Wall -O3 -I ../ -I . -I ./view/
CFLAGS = -Wall -O0 -I ../ -I . -I ./view/ -Wno-misleading-indentation -Wno-parentheses -Wno-reorder -Wno-unused-variable -Wno-write-strings
CFLAGS = -MMD -MF tglutp_main.deps -Wall -g  -O0 -I ../ -I . -I ./view/ -Wno-misleading-indentation -Wno-parentheses -Wno-reorder -Wno-unused-variable 


//CCXX=g++ -fpermissive 
CCXX=g++  
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu
LIBS=-lglut  -lGLU -L/usr/lib/compiz -lopengl -lm -L/usr/X11R6/lib -lXi -lXmu
LIBS=-lglut  -lGLU -L/usr/lib/compiz -lOpenGL -lm -L/usr/X11R6/lib -lXi -lXmu

VIEWLIBS=-L./view -lmyview  -L./atom -lmyatom  -L./ext -lmyext -L./base -lmybase -ljpeg -L./math $(LIBS) 

AFFXINC=/cygdrive/c/mydocs/scripts/cc/affx/
AFFXINC=./affx/

all: 
	cd base ; make ;cd ..
	cd view ; make  ;cd ..
#	cd math ; make  ;cd ..
	cd atom ; make  ;cd ..
	cd ext ; make  ;cd ..
	cd chem ; make ; cd ..
	rm -f main.exe
	make main
clean:
	rm -f *.o main

sterile:
	rm -f `find -name "*.o" `
	rm -f `find -name "*.a" `
dev:
	rm -f main.o
	make main
	
	
dcdlib.o : dcdlib.c dcdlib.h
	$(CCXX) $(CFLAGS) -c dcdlib.c
data_and_config.o : data_and_config.cpp data_and_config.h debug_and_support.h 
	$(CCXX) $(CFLAGS) -c data_and_config.cpp
	
data_display.o : data_display.cpp data_display.h debug_and_support.h \
         data_and_config.h 
	$(CCXX) $(CFLAGS) -c data_display.cpp

#atom_display.o : atom_display.cpp atom_display.h debug_and_support.h \
#         data_and_config.h atoms.h atom_lists.h nurbs_test.h
#	$(CCXX) $(CFLAGS) -c atom_display.cpp

./view/libmyview.a : 
	cd view ; make  ;cd ..
	
./math/libmymath.a : 
	cd math ; make  ;cd ..	
		
./atom/libmyatom.a : 
	cd atom ; make  ;cd ..

main.o : main.cpp  ./view/libmyview.a ./atom/libmyatom.a \
         data_display.h tess_display.h
	$(CCXX) $(CFLAGS) -c main.cpp
	
main :  data_and_config.o data_display.o main.o 
	$(CCXX) $(CFLAGS) -o main  \
	data_and_config.o data_display.o main.o  \
	-L./view -lmyview -L./atom -lmyatom  -L./ext -lmyext \
	-L./chem -lmychem -L./base -lmybase -ljpeg $(LIBS)

modelling_matrix.o : modelling_matrix.h modelling_matrix.cpp data_display.o
	$(CCXX) $(CFLAGS) -c modelling_matrix.cpp

simumain.o : simumain.cpp data_display.o
	cd math ; make  ;cd ..
	$(CCXX) $(CFLAGS) -c simumain.cpp
	
# I think this is obsolete, but I can 't remember
regress : simumain.cpp  data_display.o data_and_config.o
	$(CCXX) $(CFLAGS) -o simumain.o  data_display.o \
	data_and_config.o   \
	-L./view -lmyview  -L./atom -lmyatom  -L./ext -lmyext \
	-L./base -lmybase -ljpeg -L./math $(LIBS) 
	
model : modelling_matrix.o data_display.o
	cd math ; make  ;cd ..
	$(CCXX) $(CFLAGS) -o model modelling_matrix.o data_display.o \
	data_and_config.o   \
	-L./view -lmyview -L./atom -lmyatom  -L./ext -lmyext \
	-L./base -lmybase -ljpeg -L./math $(LIBS) 

simumain : simumain.o data_display.o data_and_config.o
	cd math ; make  ;cd ..
	$(CCXX) $(CFLAGS) -o simumain simumain.o data_display.o \
	data_and_config.o   \
	-L./view -lmyview -L./atom -lmyatom  -L./ext -lmyext \
	-L./base -lmybase -lgsl -ljpeg -L./math -lmymath $(LIBS) 

streaming_data.o : streaming_data.cpp data_display_header.h background_loader.h dd_display_options.h data_display.o
	cd math ; make  ;cd ..
	$(CCXX) $(CFLAGS) -I $(AFFXINC)  -c streaming_data.cpp

streaming_data : streaming_data.o data_display.o data_and_config.o
	cd math ; make  ;cd ..
	$(CCXX) $(CFLAGS) -o streaming_data streaming_data.o data_display.o \
	data_and_config.o  $(VIEWLIBS)
	
	
stats_test.o : stats_test.cpp wavelet_separation.h dcdlib.h stat_calc_classes.h stats_base_classes.h data_display.o
	cd math ; make  ;cd ..
	$(CCXX) $(CFLAGS) -I $(AFFXINC)  -c stats_test.cpp

stats_test : stats_test.o  dcdlib.o data_and_config.o
	cd math ; make  ;cd ..
	$(CCXX) $(CFLAGS) -o stats_test stats_test.o  dcdlib.o \
	data_and_config.o  $(VIEWLIBS)
	

#FastNorm3.o : ./math/FastNorm3.h ./math/FastNorm3.c
#	$(CCCC) $(CFLAGS) -c ./math/FastNorm3.c $(LIBS)
#	ar rc libmymath.a ./math/FastNorm3.o
	
#mylibs : ./math/FastNorm3.o
#	$(CCXX) $(CFLAGS)  -nostartfiles -static -o math.lib ./math/FastNorm3.o -lm -lc
	
	


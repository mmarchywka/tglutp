

# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I .

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I .

# fully optimized
CFLAGS = -Wall -O3 -I ../ -I .
CFLAGS = -Wall -g  -O0 -I ../ -I . -Wno-misleading-indentation -Wno-reorder
#CFLAGS = -Wall -O0 -I ../ -I .  -Wno-reorder


CCXX=g++
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu

all: mylibs
#	ar crv libmyview.a *.o
	ar cr libmyview.a *.o
	ranlib libmyview.a
#	rm -f *.o

tidy:
	rm -f *.o
	rm -f *.bak	
clean:
	rm -f *.o
	rm -f *.a
	
glut_commons.o : glut_commons.cpp glut_commons.h
	$(CCXX) $(CFLAGS) -c glut_commons.cpp
	
#fontdata.o : fontdata.cpp fontdata.h fontDefinition.h global.h
#	$(CCXX) $(CFLAGS) -c fontdata.cpp

mathutil.o : mathutil.cpp mathutil.h global.h
	$(CCXX) $(CFLAGS) -c mathutil.cpp

annotation_handler.o : annotation_handler.cpp annotation_handler.h global.h ../ext/bmp_reader.h
	$(CCXX) $(CFLAGS) -c annotation_handler.cpp

#drawutil.o : drawutil.cpp drawutil.h mathutil.h fontdata.h global.h
#	$(CCXX) $(CFLAGS) -c drawutil.cpp

Camera.o : Camera.cpp Camera.h mathutil.h global.h
	$(CCXX) $(CFLAGS) -c Camera.cpp

glut_generic.o : annotation_handler.h glut_generic.cpp glut_generic.h camera_and_lights.h glut_mol_gui.h \
	../base/basic_pseudo_threading.h 
	$(CCXX) $(CFLAGS) -c glut_generic.cpp
	
text_commands.o : text_commands.cpp text_commands.h camera_and_lights.h glut_mol_gui.h \
	../base/points_base_classes.h ../base/cmd_line_input.h
	$(CCXX) $(CFLAGS) -c text_commands.cpp
	
camera_and_lights.o : camera_and_lights.cpp camera_and_lights.h Camera.o 
	$(CCXX) $(CFLAGS) -c camera_and_lights.cpp
	
mouse_commands.o : mouse_commands.cpp mouse_commands.h  \
         camera_and_lights.h glut_generic.h
	$(CCXX) $(CFLAGS) -c mouse_commands.cpp
	
mylibs : mathutil.o Camera.o annotation_handler.o \
	glut_generic.o text_commands.o camera_and_lights.o \
	mouse_commands.o glut_commons.o
	

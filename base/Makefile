

# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I .
CFLAGS = -Wall -g -DDEBUG -I ../ -I . -O0 -Wno-misleading-indentation

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I .

# fully optimized
#CFLAGS = -Wall -O3 -I ../ -I .
CFLAGS = -Wall -g -DDEBUG -I ../ -I . -O0 -Wno-misleading-indentation


CCXX=g++
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu

all: mybase
	rm -f *.a
	ar cr libmybase.a *.o
	ranlib libmybase.a
#	rm -f *.o

tidy:
	rm -f *.o
	rm -f *.bak	
clean:
	rm -f *.o
	rm -f *.a

basic_command_shell.o : basic_command_shell.h basic_command_shell.cpp \
	../base/cheap_greps.h  ../base/basic_symbol_store.h  \
	../base/basic_expression.h
	$(CCXX) $(CFLAGS) -c basic_command_shell.cpp
		
mybase :  basic_command_shell.o


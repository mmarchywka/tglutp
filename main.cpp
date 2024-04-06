
#define MM_GLOB_ALLOC
#include "mjm_globals.h"

#include "debug_and_support.h"
#include "data_and_config.h"

#include "./view/glut_generic.h"
#include "./atom/atom_display.h"
#include "data_display.h"
#include "tess_display.h"

config_type global_config;
glut_generic global_generic;

glut_generic_base* ggb=NULL;


typedef data_and_config::string_type string_type;

template <class Ty> void banner(Ty & os)
{	os<<" Molecule and Data Viewer - Marchywka with source from various places \n";
	os<<"	http://math.nist.gov/\n ";
	os<<"	http://www.dgp.toronto.edu/~mjmcguff/learn/opengl/ \n";
//	os<<"	ftp://ftp.csse.monash.edu.au/users/csw/FastNorm3.c\n";
//	os<<"   http://www.codeproject.com/audio/avigenerator.asp\n";
	os<<"	http://avifile.sourceforge.net/\n";
	os<<"	http://www.gnu.org/software/gsl/\n";
//	http://www.agner.org/random/stocc.htm
	os<<"	http://www.expasy.org/cgi-bin/protscale.pl\n";
	os<<"	Contact: marchywka@hotmail.com \n";
	os<<"         Supporting scripts available for pdb download and analysis.\n";
}

template <class Ty> void rhelp(Ty & os)
{	os<<"Usage: name [atom,data,tess,poly] config_file\n\n       ---->>> All Others obsolete\n";
	os<<"                Put 'DEBUG' as first entry in file and get list of requests\n";
	os<<"      Sample usage with file name override from cmd line:\n";
	os<<" $ ./main atom ./cf/config_quick.dat -pdb_file =./pdb/2it5.pdb\n";

}
	
int main( int argc, char *argv[] ) {
enum {EXIT_OK=0, WRONG_ARGS=65};		

banner(std::cout);
if (argc<2) {rhelp(std::cout); return WRONG_ARGS;}

//	783  ./main radii.dat a.pdb color.dat
	string_type huh=string_type(argv[1]);	
if (huh=="atom") 
	{if (atom_display::display(argc,argv,global_config)) {rhelp(std::cout); return WRONG_ARGS;}; }	
if (huh=="data") 
	{data_display::display(argc,argv,global_config); }	
if (huh=="tess") 
	{tess_display::display(argc,argv); }
if (huh=="poly") 
	{poly_display::display(argc,argv); }

if (!ggb->valid()) { rhelp(std::cout); return WRONG_ARGS; }
if (global_config.has("width")||global_config.has("height"))
{	int w=512; int h=512;
	global_config.get("width",w);
	global_config.get("height",h);
	glut_generic::set_ini_dim(w,h); 	
} 
glut_generic::start_glut(&argc,argv);

return EXIT_OK;
}


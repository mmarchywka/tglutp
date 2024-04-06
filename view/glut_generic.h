#ifndef GLUT_GENERIC_H
#define GLUT_GENERIC_H

//#include "../view/drawutil.h"
#include "mjm_globals.h"
#include "../view/glut_commons.h"
#include <math.h>
#include <GL/glut.h>
#include "../view/glut_mol_gui.h"
#include "../view/camera_and_lights.h"




#include "glut_picking.h"
#include "mouse_commands.h"
#include "text_commands.h"
#include "menu_commands.h"


class glut_generic : public glut_generic_base {
typedef glut_generic Myt;
typedef glut_generic_base Super; 
static Myt * r_one; 
public:
glut_generic(): Super(),text_commander((Super*)(this)),m_w_w(512),m_w_h(512) {
	r_one=this;  
	::ggb=(Super*)(this); 
	// not used yet, just added for testing... 
	// this should work now...
	m_idles.add_task("script",&text_commander,&TextCmdTy::run_script);
	m_timed.add_task("cin", &text_commander, &TextCmdTy::cin_problem);
	}
typedef text_commands TextCmdTy;
TextCmdTy text_commander;
mouse_commands mouse_commander;
// want automated features, added 2008-04-24
int m_w_w, m_w_h;
static void set_ini_dim(const int w, const int h) {r_one->m_w_w=w; r_one->m_w_h=h; }
static void reshapeCallback( int width, int height ) ;
void ReshapeCallback( int width, int height ); 
static void timerCallback( int id);
void TimerCallback( int  id  );
static void idleCallback( )
{ r_one-> IdleCallback();}
void IdleCallback(   ) { 
//cout<<"idle callback invoked\n";
// this appears to bnomb.... 
//  if (m_idles.run()==0) glutIdleFunc(0);

//cout<<"idle callback Ignored \n";

 }
void drawFinish();

void modelinit();

static void drawCallback();
void DrawCallback();

  
static void start_glut( int* argc, char *argv[]); 
static void almost_start_glut( int* argc, char *argv[]);
static void set_panic_location(int * p);
static void start_glut_main_loop( );
void AlmostStartGlut( int* argc, char *argv[]);

void StartGlut( int* argc, char *argv[]);


};



#endif



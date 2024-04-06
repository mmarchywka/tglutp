#ifndef MOUSE_COMMANDS_H
#define MOUSE_COMMANDS_H
//#include "debug_and_support.h"
//#include "data_and_config.h"
#include "camera_and_lights.h"
#include "glut_mol_gui.h"
//#include "glut_picking.h"
class mouse_commands
{
typedef mouse_commands Myt;
static Myt * r_one;

bool LMB, MMB,RMB,CTRL, ALT, SHIFT;
int mouse_state; 
int mouse_x, mouse_y;
public:
mouse_commands ():LMB(false),MMB(false),RMB(false),CTRL(false),ALT(false),SHIFT(false)
 { r_one=this; }
static void mouseCallback( int button, int state, int x, int y ) 
{ r_one->MouseCallback(button,state,x,y); }
void MouseCallback( int button, int state, int x, int y ) ;


static void passiveMotionCallback( int x, int y ) {
r_one->PassiveMotionCallback(x,y); 	}
  void PassiveMotionCallback( int x, int y );
  
static void motionCallback( int x, int y ) {
r_one->MotionCallback(x,y); 	}
void MotionCallback( int x, int y ) ;

};

//mouse_commands * mouse_commands::r_one=NULL;
#endif


//No guard here- know what you are doing!!!
// Include this at **top** at global scope and forward declare
// this function to push model points: 
// fwd_ declare for display
//void push_from_model(float *);
// glut idle calls YOUR function run_model,
// float points to : radius, x,y,z,r,g,b a float[7]


////////////////////////////////////////////////////
#include "../debug_and_support.h"
#include "../data_and_config.h"

#include "../view/glut_generic.h"
#include "../atom/atom_display.h"
#include "../data_display.h"
#include "../tess_display.h"

config_type global_config;
data_display dd(global_config);

glut_generic global_generic;

glut_generic_base* ggb=NULL;
typedef list<float*> IdleListTy;
IdleListTy p_list;

extern void run_model();
void model_runnable() {ggb->idle_enable(true); }
void model_blocked() {ggb->idle_enable(false); }
void model_done() {ggb->idle_enable(false); }
void push_from_model(float *x)
{	
	 float * own = new float[7];	
	 for ( int ii=0; ii<7 ; ii++) own[ii]=x[ii];
	p_list.push_back(own);
	
	}
void _run_model() {
//	std::cout<<"model call made\n\n"; std::cout.flush();
IdleListTy::iterator ii=p_list.begin(),ie=p_list.end();
while (ii!=ie) delete *(ii++); p_list.clear();
run_model();  data_display::display_stl(p_list);glutPostRedisplay();}

int main(int argc, char **argv) 
{
	global_config.load(argc,argv);
	dd.configure();
run_model();
data_display::display_stl(p_list);
ggb->set_idle(_run_model);
ggb->idle_enable(true); 
glut_generic::start_glut(&argc,argv);
	return 0;}	
	
	



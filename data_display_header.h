#ifndef DATA_DISPLAY_HEADER_H__
#define DATA_DISPLAY_HEADER_H__



#include "../debug_and_support.h"
#include "../data_and_config.h"

#include "../view/glut_generic.h"
// This only is needed AFAIK for the core_cmd thing
#include "../atom/atom_display.h"
#include "../data_display.h"
#include "../tess_display.h"


config_type global_config;
data_display dd(global_config);
glut_generic global_generic;
glut_generic_base* ggb=NULL;

class run_shell
{
	public:
	int model() {  (*tgt)();    return 2;  }
	typedef void (* Tt)();
	Tt tgt;
};

typedef run_shell ShellType;
ShellType task_shell;
int panic=0;

void model_runnable() {

std::cout<<MM_MARK<<" MODEL ENABLE SET TREYHYYH \n";

ggb->idle_enable(true); }

void model_blocked() {
std::cout<<MM_MARK<<" disable model disable \n";
//ggb->idle_enable(false);
ggb->idle_enable(true);
 }

void model_done() {ggb->idle_enable(false); }
// this is overriding the set_idle thingy... 
void model_init( void (_run_model)(), int * argc, char ** argv )
{
std::cout<<"calling start_glut early COMMENT OUT MOVE TO START see if that works\n";
//	glut_generic::almost_start_glut(argc,argv);
//glutInit(argc,argv);
	
// 
//	glut_generic::set_panic_location(&panic); 


		// This is needed to pickup the scripting logic which really should
		// be moved but it would be nice to generalize the hierarchial data
		// things and pickup the pick logic too. 
		// who deletes this? the commander has multiple pointers to same object?
	core_cmd_handler *  cch= new core_cmd_handler(0,global_config);
	// this does call add_idle_task
// confusing run
	ggb->set_idle(_run_model);




	// this updates the screen at least once, but hard to tell
	// what is going on... ok, this doesn't shut off per the 
	// user's handshaking so it probably gets data starved... 
//	task_shell.tgt=_run_model;
//	ggb->add_idle_task("auser model",&task_shell,&ShellType::model); 

// shoudl set this too 
	ggb->idle_enable(true); 


std::cout<<"calling start_glut late commented out \n";
	//glut_generic::start_glut(argc,argv);
	glut_generic::start_glut_main_loop();

}


#endif

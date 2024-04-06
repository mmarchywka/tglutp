#ifndef GLUT_MOL_GUI_H
#define GLUT_MOL_GUI_H

#include "annotation_handler.h"

#include "mathutil.h" // To pickup "Ray"

#include "../base/basic_pseudo_threading.h"
#include <math.h>
#include <GL/glut.h>
#include <iostream>

class glut_generic_base {
typedef glut_generic_base Myt;
typedef annotation_handler AnoteTy; 
public:

typedef GLfloat coord_type;
typedef int flag_type;
typedef int index_type;
//mytype (*f1)(mytype , mytype) =linP;

typedef void ( *draw_model_ptr)();
typedef void ( *draw_select_ptr)(int,int);
enum DrawMode { WIREFRAME,FLAT_SHADED,SMOOTH_SHADED, NUM_DRAW_MODES } ;

glut_generic_base()
	:drawMode(FLAT_SHADED),
	 timerInterval(1000), frameCount(0), 
	pickingRay(),cullBackfaces( true),
	//drawPickingRay( true),
	 drawWorldAxes(true),need_init(true),
	draw_func( NULL), draw_select_func(NULL),  //timer_func(NULL),
	idle_func(NULL),idle_dispatch(NULL), m_message_once(false)
	{ 
	}

bool valid() { return (draw_func!=NULL); }
int local_idle() {if ( !idle_func) return 0;   idle_func(); return 1;  }
bool m_message_once;
void idle_enable(bool enable) {     
if ( !m_message_once) 
std::cout<<"idle_enable commneted ouf until things work \n";
m_message_once=true;
	m_idles.enable(enable,(const char*)("user idle")); 


if (enable) glutIdleFunc(idle_dispatch);

}
bool idle_enable() const {return  m_idles.enabled((const char*)("user idle"));}
bool  idle_toggle() {idle_enable(!idle_enable()); return idle_enable();  }
void set_idle(draw_model_ptr id)
{ 	idle_func=id;
	m_idles.add_task("user idle",this,&Myt::local_idle); 
}
template <class Ty> void dump_q(Ty & os)
{   m_idles.dump(os); m_timed.dump(os);  }
template <class Ts, class Ty,typename Tf > void add_draw_task( const Ts & nm,
  Ty * thing, const Tf f )
{  m_draw.add_task(nm,thing,f); }
template <class Ts, class Ty,typename Tf > void add_idle_task( const Ts & nm,
  Ty * thing, const Tf f )
{  m_idles.add_task(nm,thing,f); }
template <class Ts, class Ty,typename Tf > void add_timed_task( const Ts & nm,
  Ty * thing, const Tf f )
{  m_timed.add_task(nm,thing,f); }

	DrawMode drawMode;
	int timerInterval,frameCount;
	Ray pickingRay;
	bool cullBackfaces,//drawPickingRay, 
	drawWorldAxes, need_init;
public:
	basic_pseudo_threading m_idles,m_timed,m_draw;

void set_df(draw_model_ptr df) { draw_func=df;}
void set_ds(draw_select_ptr ds) { draw_select_func=ds; }
template < class Tx> int annotate(Tx & cmd ) { 
	std::cout<<MM_MARK<<"\n";
	return m_anot.annotate(cmd); }
protected:
	draw_model_ptr draw_func;
public:
	draw_select_ptr draw_select_func;
	//draw_model_ptr  timer_func;
protected:
	draw_model_ptr idle_func,idle_dispatch;

	AnoteTy m_anot;

};

extern glut_generic_base* ggb;

#endif



#ifndef GLUT_COMMONS_H__
#define GLUT_COMMONS_H__
#include <GL/glut.h>

namespace myglutil {	
	typedef int index_type;
	  
	extern  GLfloat br[],no_mat[],black[],white[],green[],clear_green[];

	void shiny_material(bool hl);
	void generic_light( int x); 
	void generic_lighting();
	void lights_on(int flags);
	void lights_off(int flags);
	void generic_mat_1();
	void sphere_start(float x);			
	void blend_start();
	void point_start(int ps) ;
	void point_end();
	void disk_start() ;
	
	void set_blending(int blend); 
	};

#endif


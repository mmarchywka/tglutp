#include "glut_commons.h"
namespace myglutil {
	  
 // glColor4f(-1.0f,-1.0f,-1.0f,0.9f);			// Full Brightness, 50% Alpha ( NEW )
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// Blending Function For Translucency Based On Source Alpha Value ( NEW )
//glBlendFunc(GL_SRC_ALPHA,1-GL_SRC_ALPHA);
//  glEnable(GL_BLEND);		// Turn Blending On
		//	glDisable(GL_DEPTH_TEST);
  //Point3 lightPosition = camera->getPosition();
  // glLightfv( GL_LIGHT0, GL_POSITION, lightPosition.get() );
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1};
     GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1 };
     GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1 };
    
     GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1 };
     GLfloat no_shininess[] = { 0.0 };
     GLfloat low_shininess[] = { 5.0 };
     GLfloat high_shininess[] = { 100.0 };
     GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

	GLfloat black[] = { 0.0, 0.0, 0.0, 1};
	GLfloat white[] = { 1,1,1, 1};
	GLfloat green[] = { 0.0, 1.0, 0.0, 1};
	GLfloat clear_green[] = { 0,1,0,1};
	
	GLfloat br[] = { 1, 1.0f, 1, 1.0f };
	
	void shiny_material(bool hl)
       {
          glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, hl?high_shininess:low_shininess);
       }
void generic_light(int x)
{
	glLightfv(x,GL_AMBIENT,br); // 0 0 0 1
	glLightfv(x,GL_SPECULAR,br); // 1 1 1 1 
	glLightfv(x,GL_DIFFUSE,br); // 1 1 1 1 
}
void lights_on(int flag)
{
	glEnable( GL_LIGHTING );
	// ever hear of an array?????
	if (flag&1) glEnable( GL_LIGHT0 );
	if (flag&2) glEnable( GL_LIGHT1 );
	if (flag&4) glEnable( GL_LIGHT2 );	
}
		
void lights_off(int flag)
{
	glDisable( GL_LIGHTING );
	// ever hear of an array?????
	if (flag&1) glDisable( GL_LIGHT0 );
	if (flag&2) glDisable( GL_LIGHT1 );
	if (flag&4) glDisable( GL_LIGHT2 );	
}
	
void generic_lighting()
{
	// GLfloat br[] = { 1, 1.0f, 1, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,br);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,1); //  <<<<<<<
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1);     //  <<<<<<<
	glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0);
	glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0);
	generic_light(GL_LIGHT0);
	generic_light(GL_LIGHT1);
	generic_light(GL_LIGHT2);
	//	glLightfv(GL_LIGHT0,GL_AMBIENT,br); // 0 0 0 1
	//	glLightfv(GL_LIGHT0,GL_SPECULAR,br); // 1 1 1 1 
	//	glLightfv(GL_LIGHT0,GL_DIFFUSE,br); // 1 1 1 1 
	glDisable(GL_FOG);
}
void generic_mat_1()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	}	
void sphere_start(float x)
{ 	no_mat[3]=x;
	generic_mat_1();           
        generic_lighting();		
}
	
void blend_start()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);	 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// Blending Function For Translucency Based On Source Alpha Value ( NEW )
	glBlendFunc(GL_SRC_ALPHA,1-GL_SRC_ALPHA);
	glEnable(GL_BLEND); 
}
//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/blendfunc.html
void set_blending(int blend)
{
	if (blend!=0)
	{
	GLenum src[]={GL_ZERO, GL_ONE,GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,
			  GL_SRC_ALPHA_SATURATE};
	GLenum dest[]={	  GL_ZERO, GL_ONE,
		   GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA,
		   GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
		   GL_ONE_MINUS_DST_ALPHA};	
	index_type ii=(blend&15), ij=(blend>>4)&15;
	if ( ii>8) ii=8; if (ij>7) ij=7; 
	glBlendFunc(src[ii],dest[ij]);glEnable(GL_BLEND); 
	}	
	else {glDisable(GL_BLEND);}	
	
	}


void point_start(int ps) {
//	glDisable( GL_LIGHTING );
	glDisable(GL_POINT_SMOOTH);
	glDisable( GL_CULL_FACE );
	glPointSize(ps);
	glBegin(GL_POINTS);
}


void point_end() { glEnd(); }
void disk_start() {
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, no_mat);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
//          glMaterialfv(GL_FRONT, GL_SHININESS, m_highlight?high_shininess:low_shininess);
	glDisable( GL_CULL_FACE );
	//glDisable( GL_LIGHTING );
	//glDisable( GL_LIGHT0 ); }
	}
};

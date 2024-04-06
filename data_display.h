#ifndef DATA_DISPLAY_H
#define DATA_DISPLAY_H

#include "debug_and_support.h"
#include "data_and_config.h"
#include "camera_and_lights.h"
#include "glut_generic.h"
#include "glut_commons.h"
#include <list>
#include <map>

#include <string>
class display_generic_text
{	typedef display_generic_text Myt;
	typedef float F;
	typedef std::string Str;
public:	
	const char * c_str() const { return text.c_str(); }
F r,g,b,x,y,z,scale;
Str text;
	
};

class data_display {

public:
typedef data_display Myt;
typedef data_and_config DaCoTy;
	typedef DaCoTy::string_type string_type;
	typedef DaCoTy::name_type name_type;
typedef DaCoTy::count_type count_type;
typedef char Ch;
// This is easier than having to create a static variable somewhere else... 
static Myt*& r_one() { static Myt * myone=NULL; return myone; }
// it knows about the config stuff anyway...
data_display(config_type & cf):m_cf(&cf)
{ r_one()=this; }
config_type * m_cf; 

typedef map<string_type,string_type> TextMapTy;
typedef TextMapTy::const_iterator TextMapItorTy;
TextMapTy m_texts;

void configure() {}//

void add_text(const string_type & nm, const string_type & val)
{ m_texts[nm]=val;  }

static void labels() { 
	//std::cout<<"Trying to call could be null...\n"; 
	if (r_one()) r_one()->Labels(); }

void Labels() const 
{glPushMatrix();
	TextMapItorTy ii=m_texts.begin(), ie=m_texts.end();
	glTranslatef( 100,30,0 );
	while (ii!=ie)
	{
	// std::cout<<" label "<<(*ii).second<<" ";
	  const Ch * c= (*(ii++)).second.c_str();
	while (*c){
	 glPushMatrix();
	 glScaled(1, 1, 0.1f);
	float stroke_scale=0.1f;
	glColor4f(0.0, 1.0, 0.0, 1.0f);
	
	glLineWidth(1);
	 glScalef(stroke_scale, stroke_scale, stroke_scale);
	 while (*c) {if (*c=='\n'){c++; break;} 
	 glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);c++;
	 }
	glPopMatrix();
	glTranslatef( 0,-20,0 );
	}
	}

glPopMatrix();
}

static void generic_lighting() { myglutil::generic_lighting(); }
static void drawBoxList();
static void draw_a_thing();
static void axes();
static void make_disks ();
static void  display_nurb(void);
static int  main_nurb(int argc, char *argv[]);
typedef float * thing_item_type;
typedef list<thing_item_type> ThingTy;
typedef display_generic_text TextTy;
typedef std::vector<TextTy> TeLiTy;
static ThingTy thing_list,line_list,norm_list;
static TeLiTy label_list; 
template <class Ty>  static int display_stl(Ty & blah)
{
	thing_list.clear(); 
	typedef typename Ty::const_iterator Ti;
	Ti ii=blah.begin(), ie=blah.end();
	while (ii!=ie ) {   thing_list.push_back(*(ii++));}
	//ggb->draw_func=draw_a_thing;
	ggb->set_df(draw_a_thing);

std::cout<<MM_MARK<<" display_stl called ok\n";
	return 0;
}
template <class Ty>  static int display_lines(Ty & blah)
{	// this is not robust. The called owns this memory but it is possible 
	// that the screen could get updated or otherwise this list is used after the
	// owner deleted the underlying arrays. 
	line_list.clear(); 
	typedef typename Ty::const_iterator Ti;
	Ti ii=blah.begin(), ie=blah.end();
	while (ii!=ie ) {   line_list.push_back(*(ii++));}
	ggb->set_df(draw_a_thing);
	return 0;
}
template <class Ty>  static int display_normals(Ty & blah)
{	// this is not robust. The called owns this memory but it is possible 
	// that the screen could get updated or otherwise this list is used after the
	// owner deleted the underlying arrays. 
	norm_list.clear(); 
	typedef typename Ty::const_iterator Ti;
	Ti ii=blah.begin(), ie=blah.end();
	while (ii!=ie ) {   norm_list.push_back(*(ii++));}
	ggb->set_df(draw_a_thing);
	return 0;
}
// This is called by the disply option from the program main.cpp

template <class Ty>  static int display(int argc, char *argv[], Ty & cf)
{	
	if (argc>=3) {
	DaCoTy::input_type * is=DaCoTy::open(name_type(argv[2]));
	if (!is->good()) {  cout<<"Problem with config file "<<name_type(argv[2])
	<<"\n"; cout.flush(); 
	return 65;}
	cf.load(argc,argv);
	cf.load(*is);delete is;
 
}
	
//	ggb->draw_func=drawBoxList;
	ggb->set_df(drawBoxList);
	if (cf.has("nurb"))    {
std::cout<<MM_MARK<<" starting nurb loop\n";
		//ggb->draw_func=display_nurb;
		ggb->set_df(display_nurb);
		main_nurb(argc,argv); 
		glut_generic::start_glut_main_loop();
		}//ggb->draw_func=display_nurb;
	
	if (cf.has("disks"))            ggb->set_df(make_disks); //ggb->draw_func=make_disks;
	return 0; 
	
}

template <class Ty > static void labels(Ty & li ) 
{	
	typedef typename Ty::value_type Tt;
	typedef typename Ty::iterator Ii;
	Ii ii=li.begin(), ie=li.end();
	//glTranslatef( 100,30,0 );
	while (ii!=ie)
	{
		Tt & s=(*ii);
		glPushMatrix();
		// std::cout<<" label "<<(*ii).second<<" ";
	  	const Ch * c= s.c_str();
	  	glColor4f(s.r,s.g,s.b, 1.0f);
	  	glTranslatef( s.x,s.y,s.z );
	  	glScaled(s.scale, s.scale, 0.1f);
			float stroke_scale=0.1f;
		while (*c)
		{
	 		glPushMatrix();
	 		
			//glColor4f(0.0, 1.0, 0.0, 1.0f);
			glLineWidth(1);
	 		glScalef(stroke_scale, stroke_scale, stroke_scale);
	 		while (*c) 
	 		{
	 			if (*c=='\n'){c++; break;} 
	 			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);c++;
	 		}
			glPopMatrix();
			glTranslatef( 0,-20,0 );
		}
		glPopMatrix();
		++ii;
	}

}
static void norm(float *x); 
// x,y,z,nx,ny,nz,r,g,b,sz
template <class Ty> static  void norms(Ty & li)
{
	typedef typename Ty::iterator Ii;
	Ii ii=li.begin(), ie=li.end();
	glDisable( GL_CULL_FACE );
//	 glEnable( GL_CULL_FACE );
//	glBegin(GL_POINTS);
	while (ii!=ie)
	{
		float * x=(*ii);
		norm(x);
	
   		
   		
  // 		glVertex3f(x[3],x[4],x[5]);
		++ii;
	}
//	glEnd();
	
}

// this is used 
template <class Ty> static  void lines(Ty & li)
{
	typedef typename Ty::iterator Ii;
	Ii ii=li.begin(), ie=li.end();
	glDisable( GL_CULL_FACE );
	glBegin(GL_LINES);
	//std::cerr<<"asdfasdfasdfasdfasfadsf\n";
	while (ii!=ie)
	{
		float * x=(*ii);
		GLfloat mat_diffuse[] = { x[6],x[7],x[8],1.0 };
      	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diffuse);
     	glColor3f(x[6],x[7],x[8] );
     	
   		glVertex3f(x[0],x[1],x[2]); glVertex3f(x[3],x[4],x[5]);
		++ii;
	}
	glEnd();
	
}

};

////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
#endif



// marchywka@hotmail.com ca May 2008
// I finally got tired of rebuilding everything for small changes
// so I hid most of the junk and exposed interface only- Doh.

#include "annotation_handler.h"
#include "../ext/bmp_reader.h"
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include <sstream>


typedef annotation_handler MyClass;

namespace
{

class drawable
{
public:
	typedef MyClass::P3 P3;
	typedef MyClass::P4 P4; 
	typedef MyClass::junk_type Jt;
	typedef MyClass::coord_type Fl; 
	
	virtual void draw() =0; 
	drawable(): m_enabled(true) {}
	void draw_if() {if ( m_enabled) draw();   }
	void show() { m_enabled=true; }
	void hide() { m_enabled=false; } 
	void showhide(const bool e) { m_enabled=e; } 
	virtual ~drawable() {}
	// end user fills this in for human readable ID.
	template <class Os> void dump(Os& os) { os<<m_os.str();  }
	
protected:
	// most of these are supposed to be small, not sure if this defaults to
	// large buffers etc but doesn't matter right now. 
	std::ostringstream m_os;
private:
	bool m_enabled; 

};

}; 


class annotation_junk
{
	typedef annotation_junk Myt;
	typedef drawable Dt;
	typedef std::vector<Dt*> Dv;
	
public:

typedef MyClass::P3 P3;
typedef MyClass::P4 P4; 
annotation_junk()
:m_default_color(1,1,1,1), m_default_scale(1)
{}

// These can be private and put on a stack... 
P4 m_default_color;
float m_default_scale;



template <class Ty> void set_color(const Ty & x) { glColor4f(x[0],x[1],x[2],x[3]); }

template <class Ty> void set_mat(const Ty & x)
{ 	
	GLfloat diffuse[] = { x[0],x[1],x[2],x[3] };
	//	float am=.35f;
	//	float off=.05f;
	//	GLfloat mat_diffusef[] = { diffuse[0]*am+off,diffuse[1]*am+off,diffuse[2]*am+off,diffuse[3] };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuse);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
}

template <class Ty> void set_loc(const Ty & x)  { glTranslatef(x[0],x[1],x[2]); }

~annotation_junk()
{
	Dv::iterator ii=m_dv.begin(),ie=m_dv.end();
	while (ii!=ie) { delete (*ii); ++ii; }
	ii=m_sv.begin();ie=m_sv.end();
	while (ii!=ie) { delete (*ii); ++ii; }
}

template <class Os> void dump( Os& os )
{
	os<<" world coords\n";
	Dv::iterator ii=m_dv.begin(),ie=m_dv.end();
	int i=0; 
	while (ii!=ie) { os<<i<<" "; ++i; (**ii).dump(os); os<<"\n"; ++ii; }
	os<<" screen coords\n"; i=0;
	ii=m_sv.begin();ie=m_sv.end();
	while (ii!=ie) { os<<i<<" "; ++i; (**ii).dump(os); os<<"\n";   ++ii; }
}
void push_world( Dt* dt)  { m_dv.push_back(dt); }
void push_screen( Dt* dt) { m_sv.push_back(dt); }
void showhide(const int & v, const int & i, const bool e)
{	if ( i<0) return; 
	if (v==0) { const int sz=m_dv.size(); if ( i<sz) m_dv[i]->showhide(e);  }	
	if (v==1) { const int sz=m_sv.size(); if ( i<sz) m_sv[i]->showhide(e); }
}
//m_ptr->set_screen_coords();
//m_ptr->paint_marchywka();
//m_ptr->reset_model_coords();

void draw_all()
{
	draw_a_vector(m_dv);
	set_screen_coords();
	// push/pop didn't seem to maintain a identity xform after first pass
	// through loop, probably a latent bug here ...
	//draw_a_vector(m_sv);
	Dv::iterator ii=m_sv.begin(),ie=m_sv.end();
	while (ii!=ie) 
	{  	glLoadIdentity(); //glPushMatrix();
		(*ii)->draw_if(); 
		//glPopMatrix();	
		++ii; 
	}
	
	reset_model_coords();	
}
void draw_a_vector(Dv & v)
{
	Dv::iterator ii=v.begin(),ie=v.end();
	while (ii!=ie) 
	{  	glPushMatrix();
		(*ii)->draw_if(); 
		glPopMatrix();	
		++ii; 
	}
	
}


// this always exits with last char and last line stat position on 
// stack for anyone who wants to append etd. Presumably you know where
// youy started... 

template <class Ch,class Tx > void draw_string( Ch * const cx, const Tx & pitch)
{	glPushMatrix(); // extra, to areturn "cursor" location 
	glLineWidth(1);
	float stroke_scale=0.1f*3;
	glScalef(stroke_scale, stroke_scale, stroke_scale);
	Ch  const *c=cx;
	while (true)//(*c)
	{	glPushMatrix();
	 	while (true) // (*c) 
	 	{
	 		if (*c==0){  return; } 
	 		if (*c=='\n'){c++; break;} 
	 		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);c++;
	 	}
		glPopMatrix();
		set_loc(pitch);
	}
	
}

// This has to be reversible, I;m not sure I'm saving entire
// context here but it seems to work right now. 
void set_screen_coords() 
{
	//int vp[4];
	//glGetIntegerv(GL_VIEWPORT,vp);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity(); // no transformations
	// need to load viewport and try to figure it out from there. 
	int w=512; int h=512; 
    // Set the viewport to be the entire window
  	// glViewport(0, 0, w, h);

	gluOrtho2D(-w,w,-h,h);
	glMatrixMode(GL_MODELVIEW);	
	
	glLoadIdentity();
}

void reset_model_coords() 
{
	// really should push/pop and not assume what others did here... 
	glEnable(GL_LIGHTING);
	//glViewport(vp[0],vp[1],vp[2],vp[3]);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// Branding could be important :)
void paint_marchywka()
{
	int j=0; 
	for (int i=0; i<10; ++i)
	{
		glPushMatrix();
		glLoadIdentity();
		float fukkk=1.0/(1.0+i*1.0);
		set_color(P4(1,1,fukkk,1.0) );
		set_mat(P4( 1.0/(i*1.0+1.0) ,1.0/(j+1),1.0,1.0 ));
		set_loc(P3(20*j-100,20*i-100,0)); 
		draw_string("Marchywka",P3(i*20-100,20*i-100,0));
		glPopMatrix();
		glPopMatrix();
		glPopMatrix(); 
	}
}

private:
	Dv m_dv,m_sv;


};
namespace
{
class origin : public drawable
{	// draw a short xyz axis
	typedef GLfloat Gf;
public:
	origin(const P3 & o , const Fl & l , const P4 & c = P4(1,1,1,1) )
		:m_origin(o),m_color(c),m_length(l) 
	{
		m_os<<"origin ";
		m_os<<m_origin;
		// ambiguous overload? 
		m_os<<" "<<m_color[0]<<" "<<m_color[1]<<" "<<m_color[2]<<" "<<m_color[3];    
	}
	void draw() 
	{	glLineWidth(1);
		// this is going to be another  figuring out why
		// the back sides dis-a--pear
		// I think it had to do with lighting and normal, but turning off lights fixed it
		glDisable( GL_LIGHTING );
		glDisable( GL_CULL_FACE );
		m_junk.set_color(m_color);
		m_junk.set_loc(m_origin);
		glBegin(GL_LINES);
		m_junk.set_mat(m_color);
  		const Gf l=m_length;
  		glVertex3f(-l, 0, 0);  glVertex3f(l, 0, 0);
  		glVertex3f(0, -l, 0);  glVertex3f(0, l, 0);
  		glVertex3f(0, 0, -l);  glVertex3f(0, 0, l);
		glEnd();
		//	glEnable( GL_LIGHTING );
	}
private:
	Jt m_junk; 
	P3 m_origin;
	P4 m_color;
	Fl m_length;
};

class bitmap_image : public drawable
{ // load a bitmap image 
public:
	bitmap_image(const P3 & o , const char * nm )
		:m_origin(o),m_image(nm),m_nm(nm)
	{
		m_os<<"image "<<m_nm<<" "<<m_origin; 
		if ( m_image.data()!=0)
			std::cout<<MM_MARK<<" loaded image with "<<m_image.w()<<" "<<m_image.h()<<'\n';
		else std::cout<<MM_MARK<<" coud not load data for "<<nm<<'\n';
	}
	
	void draw() 
	{
		if ( m_image.data()==0) return; 
		m_junk.set_loc(m_origin);
  		glRasterPos2f(m_origin[0],m_origin[1]);
    //    glPixelZoom(xscale, yscale);
        glDrawPixels(m_image.w(), m_image.h(),
          	GL_RGB, GL_UNSIGNED_BYTE, m_image.data());		
	}
private:
	Jt m_junk; 
	P3 m_origin;
	bmp_reader m_image;
	std::string m_nm;
};
class plain_text : public drawable
{	// plain text. 
public:
	plain_text(const P3 & o , const char * nm , const P4 & co=P4(1,1,1,1),const float & scale=1 )
		:m_origin(o),m_color(co), m_scale(scale),m_string(nm) 
		{
			m_os<<"string ";
			m_os<<m_origin;
		// ambiguous overload? 
			m_os<<" "<<m_color[0]<<" "<<m_color[1]<<" "<<m_color[2]<<" "<<m_color[3];    
			m_os<<" "<<m_string;
		}
	
	void draw() 
	{	glDisable( GL_LIGHTING );
		glDisable( GL_CULL_FACE );
		m_junk.set_loc(m_origin);
		m_junk.set_color(m_color);
		float stroke_scale=m_scale;
		glScalef(stroke_scale, stroke_scale, stroke_scale);
 		m_junk.draw_string(m_string.c_str(), P3(0,10,0) );
 		// The last line and char are stored for appending 
 		glPopMatrix(); glPopMatrix(); 
		if (false)
		{
			m_junk.set_loc(m_origin+P3(3,0,0));
			m_junk.set_color(P4(1-m_color[0],1-m_color[1],1-m_color[2],1));
			//float stroke_scale=m_scale;
			//glScalef(stroke_scale, stroke_scale, stroke_scale);
 			m_junk.draw_string(m_string.c_str(), P3(0,10,0) );
 			// The last line and char are stored for appending 
 			glPopMatrix(); glPopMatrix(); 
		}
	}
private:
	Jt m_junk; 
	P3 m_origin;
	P4 m_color;
	float m_scale;
	std::string m_string;
};

}; //name space

MyClass::api_type MyClass::redraw() 
{
	// yahoo, this code seems to work and may be usable once I get back to it.
	//return api_type(OK); 
	m_ptr->draw_all(); 
	return api_type(OK); 
}

MyClass::handle_type MyClass::define_area(const name_type & name, const name_type & type )
{
	return 0;
}
// This gets called with thing.
MyClass::api_type MyClass::op(const StrTy & opcode)
{
		return 0;
}
// these build thing 
MyClass::api_type MyClass::operand(const StrTy & opnd)
{
		return 0;
}
MyClass::api_type MyClass::op(const op_code_type & opcode )
{
		return 0;
}

// command is now ready to run. 
MyClass::api_type MyClass::exec()
{	const int sz=m_s.size();
	
	const StrTy & cmd=m_s[0];
	if (cmd=="dump")
	{
		m_ptr->dump(std::cout);
		return 0;
	}
	if (cmd=="scale")
	{
		if (sz<2) {        return 0; }	
		m_ptr->m_default_scale=atof(m_s[1]);
		return 0; 
	}
	if (cmd=="color")
	{
		if (sz<4) {        return 0; }	
		m_ptr->m_default_color=P4(atof(m_s[1]),atof(m_s[2]),atof(m_s[3]),atof(m_s[4]));
		return 0;
	}
	if (cmd=="showworld")
	{	for ( int i=1; i<sz; ++i)
		{	
			int s=int(fabs(atof(m_s[i])));
			const bool e=(m_s[i].substr(0,1)!="-");
			m_ptr->showhide(0,s,e);			
		}
		return 0;
	}
	if (cmd=="showscreen")
	{	for ( int i=1; i<sz; ++i)
		{	
			int s=int(fabs(atof(m_s[i])));
			const bool e=(m_s[i].substr(0,1)!="-");
			m_ptr->showhide(1,s,e);			
		}
		return 0;
	}
	if (cmd=="text") 
	{ if ( sz<4) return 0;
		const P3 po=P3(atof(m_s[1]),atof(m_s[2]),0 );
		StrTy co="",sp=" ";
		int p=3;
		while (p<sz) co=co+m_s[p++]+sp;
		drawable * ptr=new plain_text(po,co.c_str(),m_ptr->m_default_color,m_ptr->m_default_scale);
		m_ptr-> push_screen(ptr);        
		return 0; 
	}
	if (cmd=="label") 
	{ if ( sz<5) return 0;
		const P3 po=P3(atof(m_s[1]),atof(m_s[2]),atof(m_s[3]) );
		StrTy co="",sp=" ";
		int p=4;
		while (p<sz) co=co+m_s[p++]+sp;
		drawable * ptr=new plain_text(po,co.c_str(),m_ptr->m_default_color,m_ptr->m_default_scale);
		m_ptr-> push_world(ptr);        
		return 0; 
	}
	if ( cmd=="image")
	{	if ( sz<4) return 0;
		const P3 po=P3(atof(m_s[1]),atof(m_s[2]),0 );
		drawable * ptr=new bitmap_image(po,m_s[3].c_str());
		m_ptr-> push_screen(ptr);  
		return 0; 
	}
	if ( cmd=="ax")
	{	if ( sz<4) return 0;
		float l=10;
		const P3 po=P3(atof(m_s[1]),atof(m_s[2]), atof(m_s[3]));
		P4 co=P4(1,1,1,1);
		if (sz>4) l=atof(m_s[4]);
		if (sz>8) co=P4(atof(m_s[5]),atof(m_s[6]),atof(m_s[7]),atof(m_s[8]));
		drawable * ptr=new origin(po,l,co);
		m_ptr-> push_world(ptr);  
		std::cout<<" make ax at "<<po<<'\n';
		return 0;
	}
	
	std::cout<< " didnt find anything to make for "<<cmd<<'\n';
	return 0;
}
MyClass::annotation_handler(): m_ptr(new annotation_junk()) {}
MyClass::~annotation_handler( )
{	delete m_ptr; 
}

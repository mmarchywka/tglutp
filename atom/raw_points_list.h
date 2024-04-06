#ifndef RAW_POINTS_LIST_H__
#define RAW_POINTS_LIST_H__

#include "../atom/own_vector.h"
#include "../base/points_base_classes.h"
#include "../view/display_tree.h"
#include "../view/glut_commons.h"
#include "../debug_and_support.h"
#include <list>
#include <sstream>
// temp until we move the xproduct
#include "math.h"
#include <GL/glut.h>




class raw_points_list
{	typedef raw_points_list Myt;
	typedef float coord_type;
	typedef point3_template<coord_type> PointTy;
	typedef point3_array<coord_type,3> PairTy;
	typedef list<PairTy> ListTy;
	typedef ListTy::iterator ItorTy;
	typedef basic_string<char> string_type;
	typedef display_tree DispTy;
	typedef std::ostringstream OsStreamTy;
	
	typedef int count_type;
	
	static string_type serial_name()
	{ static int serial=0; serial++; OsStreamTy r; r<<serial; return r.str(); }
	static GLUquadricObj*  xxx() { static GLUquadricObj *xxx=gluNewQuadric(); return xxx;}
public:
	// yes this is confusing..
	typedef PointTy position_type;
	typedef PairTy point_type;
	typedef ListTy::const_iterator const_iterator;
	const_iterator  begin() const { return m_p.begin(); }
	const_iterator  end() const {return m_p.end(); }
	typedef ListTy::iterator iterator;
	iterator  begin() { return m_p.begin(); }
	iterator  end() {return m_p.end(); }
	//const index_type size() const { return m_p.size(); }
	// doh- int isn't used ANYWHERE else here.. 
//	PairTy & operator[](const int i) { return m_p[i];} 
	void create_display_tree(DispTy * root)
	{root=root->get_child(name()+serial_name());}
	raw_points_list(const string_type & nm):m_name(nm),m_p(){}
	string_type name() const {return m_name;}
	void xpush_back(const PairTy &  p)
	{m_p.push_back(p); } 
	// tried to avoid this, slow
	int size() const { return m_p.size(); } 
	void draw(DispTy & root)
	{
		if (root.hidden()) return; 
		display_settings ds=root.settings();
	//	DrawDisks(ds);
	//	return;
	//	mmdd::dout(MMDDLOC)<<"\n";
		DrawBegin(ds);
		glEnd(); //DrawEnd(); 
		DrawDisks(root);
		return;
	//	mmdd::dout(MMDDLOC)<<"\n";
		if (ds.picking()) DrawPicking(ds);
		else DrawThings(ds); 
	//	mmdd::dout(MMDDLOC)<<"\n";
		DrawEnd(); 
	//	mmdd::dout(MMDDLOC)<<"\n";
	}
	
	
	void DrawDisks(DispTy & root)
	{
	const display_settings & ds=root.settings();
	const bool picking=ds.picking();
	count_type  draw_serial=root.m_idx_max;
	//index_type  draw_serial=disp.m_idx_max;
//	const bool move_n=((ds.substyle()&1)!=0);
	coord_type disk_size=1;
	ds.set_from(&display_settings::m_dia,disk_size);
	myglutil::disk_start();
	myglutil::lights_off(7); 
	myglutil::lights_on(ds.light()); 
		if (picking) glPushName(draw_serial++);
		ItorTy ii=m_p.begin(), ie=m_p.end();
		while (ii!=ie)
		{
			PairTy& p=(*ii);
	GLfloat crap[] = { 1,1,1,1};
			myPoint4<float> diffuse=
				myPoint4<float>(p[2].x(),p[2].y(),p[2].z(),1); 
			/////////////////////////////////////////////////////////	
		//myPoint4<float> diffuse=myPoint4<float>(crap);
		// if this is true, it inhibits the use of the atom color.
		// if false, the atom color is used.
		// to use the chain color in "crap" we need a substyle bit
		m_use_color=ds.set_from(&display_settings::m_color,diffuse);
		// zero alpha and color doesn't show up, hide some otherway
		 if (diffuse==0)diffuse=crap;
	////////////////////////////////
			
			ds.offset_always(&display_settings::m_tint,diffuse);
			
		
			
			
			glColor4fv(diffuse);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
			
			
			
			///////////////////////////////////////////////////
		//	GLfloat mat_diffuse[] = { x[6],x[7],x[8],1.0 };
		GLfloat mat_diffusex[] = { 0,0,0,1.0 };
		float am=.35f;
		float off=.05f;
		GLfloat mat_diffusef[] = { diffuse[0]*am+off,diffuse[1]*am+off,diffuse[2]*am+off,diffuse[3] };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_diffusex);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diffusef);
		
			////////////////////////////////////////////////////
			
			glPushMatrix(); 
			glTranslatef(p[0].x(),p[0].y(),p[0].z());			
 			p[1].do_twirl();
 			
 			/////////////////////////
 			//	float ju[3]={1.0f,0.0f,0.0f};
  	   	//float jun[3]={-1.0f,0.0f,0.0f};
  	   	//float juz[3]={0.0f,0.0f,1.0f};
  	   	float junz[3]={0.0f,0.0f,-1.0f};
  	   	//float ss=x[9];
  	   	float *nn=junz;//x+3; //juz;//x+3;
  	   //	float * nnz=jun;
  	 //  	glNormal3fv(nn); 
  	   // glBegin(GL_QUADS);
  	    //gluDisk( xxxx,0,x[9],4,2); 
			glNormal3fv(nn);
 			/////////////////////////
			gluDisk( xxx(),0,disk_size,4,2);
			if (picking) glLoadName(draw_serial++);
  			glPopMatrix(); 	
  	
		++ii;
	}
	
if (picking)	{
	glPopName();
root.m_idx_max=draw_serial;
//std::cout<<"setting max to "<<root.m_idx_max<<"\n";
	root.sprout_max(); 
	}
	
	}
	
	void DrawBetterThings(display_settings & ds)
	{
		ItorTy ii=m_p.begin(), ie=m_p.end();
		while (ii!=ie)
		{
			PairTy& p=(*ii);
			glNormal3f(p[1].x(),p[1].y(),p[1].z());
			myPoint4<float> diffuse=myPoint4<float>(p[2].x(),p[2].y(),p[2].z(),1); 
//	ds.offset_always(&display_settings::m_tint,diffuse);
	glColor4fv(diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);			
			glVertex3f(p[0].x(),p[0].y(),p[0].z());				
		++ii;
		}
	}
	
	
	void DrawThings(display_settings & ds)
	{
		ItorTy ii=m_p.begin(), ie=m_p.end();
		while (ii!=ie)
		{
			PairTy& p=(*ii);
			glNormal3f(p[1].x(),p[1].y(),p[1].z());
			myPoint4<float> diffuse=myPoint4<float>(1,1,1,1); 
//	ds.offset_always(&display_settings::m_tint,diffuse);
	glColor4fv(diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);			
			glVertex3f(p[0].x(),p[0].y(),p[0].z());				
		++ii;
		}
	}
	void DrawPicking(display_settings & ds)
	{
		ItorTy ii=m_p.begin(), ie=m_p.end();
		while (ii!=ie)
		{
			PairTy& p=(*ii);
			glNormal3f(p[1].x(),p[1].y(),p[1].z());
			myPoint4<float> diffuse=myPoint4<float>(1,1,1,1); 
//	ds.offset_always(&display_settings::m_tint,diffuse);
	glColor4fv(diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);			
			glVertex3f(p[0].x(),p[0].y(),p[0].z());				
		++ii;
		}
	}
	
	void DrawBegin(const display_settings & ds)// const
{//	glDisable(GL_POINT_SMOOTH);glDisable( GL_CULL_FACE );
	GLfloat crap[] = { 1,1,1,1};
	myPoint4<float> diffuse=myPoint4<float>(crap);
	m_use_color=ds.set_from(&display_settings::m_color,diffuse);
	// zero alpha and color doesn't show up, hide some otherway
	if (diffuse==0)diffuse=crap;
		
	glDisable(GL_CLIP_PLANE0); myPoint4<float> clip_plane;
	if (ds.set_from(&display_settings::m_clip_plane,clip_plane))
	{ 	glClipPlane(GL_CLIP_PLANE0,myPoint4<double>(clip_plane)); 
		glEnable(GL_CLIP_PLANE0); }
	myglutil::generic_mat_1();
	myglutil::generic_lighting();
	int mode=GL_LINES;
	//if (col.m_verticies==3) mode=GL_TRIANGLES;
	if ( mode==GL_TRIANGLES){myglutil::lights_on(ds.light()); }else   myglutil::lights_off(7); 

	float width=3.0f;
	ds.set_from(&display_settings::m_thick,width);  glLineWidth(width); 
	ds.set_from(&display_settings::m_size,width);  glPointSize(width);	
	
	glColor4fv(diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, myglutil::black); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, myglutil::black);  
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuse);
	
	//glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
//        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
      //  glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, myglutil::black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, myglutil::black);
	
	
	myglutil::set_blending(ds.blend()); 
//	if (ds.blend()!=0)
//	{glBlendFunc(GL_SRC_ALPHA,1-GL_SRC_ALPHA);glEnable(GL_BLEND); }
//	else {glDisable(GL_BLEND);}
	
	if (mode==GL_TRIANGLES) {  myglutil::lights_on(ds.light());  }
	else   myglutil::lights_off(7); 
//	myglutil::lights_on(ds.light()); 
	// above commands may not work after moding...
	glBegin(mode); 
}

void DrawEnd() const{glEnd();glDisable(GL_CLIP_PLANE0); } 
	
	
template < typename Thing, typename Ts,typename Ta,typename count_type > 
	bool model_find( Ta & root,const count_type target, Thing & thing, Ts& is )
{
	typedef count_type index_type;
//	ItorTy ii=begin();
// 2024
	//ItorTy ii=m_p.begin(), ie=m_p.end();
	ItorTy ii=m_p.begin();
	index_type ver=1; //atr().m_verticies;
	if (ver<1) ver=1;
	index_type roff=ver*(target-root.m_idx_min);
	if ((roff<0)||(((unsigned int)(roff))>=m_p.size()))
	{	std::cout<<"mol view list find error :"<<roff<<" from target="
	<<target<<" and min="<<root.m_idx_min<<"\n"; root.lineage(std::cout,0); 
	return false;  }
	
	typedef typename Thing::value_type rcast;
	while (roff) {roff--; ++ii;}
	
	while (ver) {PairTy& p=(*ii); 
		thing.push_back(rcast(p[0].x(),p[0].y(),p[0].z(),1)); 
		++ii; --ver;  }
	return true;
	
	
	}
private:
	string_type m_name;
	ListTy m_p;
	bool m_use_color;
};


typedef raw_points_list RawListTy;
typedef own_vector<  RawListTy * > RawLiSTy;

#endif

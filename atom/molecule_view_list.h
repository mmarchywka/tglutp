#ifndef MOLECULE_VIEW_LIST_H__
#define MOLECULE_VIEW_LIST_H__



#include "../view/glut_commons.h"
#include "../view/display_tree.h"
#include "../base/points_base_classes.h"

#include <GL/glut.h>
#include <sstream>
#include <list>

class chain_attributes {public: 
typedef float Ct;
typedef int count_type;
chain_attributes() : m_verticies(1),m_r(1),m_g(1),m_b(1),m_alpha(1){} 
chain_attributes(const count_type v, const Ct r, const Ct g, const Ct b) 
: m_verticies(v),m_r(r),m_g(g),m_b(b),m_alpha(1){}
chain_attributes(const count_type v, const Ct* x) 
: m_verticies(v),m_r(x[0]),m_g(x[1]),m_b(x[2]),m_alpha(x[3]){} 
count_type m_verticies;
Ct m_r,m_g,m_b,m_alpha; };

typedef chain_attributes ChainAttrTy;


template <class Ty > class annotated_list : public    list<const Ty* >
{
	
	typedef const Ty* TgTPtr;
	typedef list<const Ty*> Super;
	typedef chain_attributes AtrTy; 
	typedef float coord_type;
	typedef point3_template<coord_type> normal_type;
	typedef list<normal_type> NormalListTy;
	typedef display_tree DispTy;
	typedef int index_type;
	typedef index_type IdxTy;
	typedef Ty atom_type;
public: annotated_list(const AtrTy & a=AtrTy() ): Super(),m_atr(a),m_disp(),m_use_normals(true) {}
AtrTy & atr(){ return m_atr; }
const AtrTy & atr() const { return m_atr; }
// redundant typedef now disalloed? 
//typedef chain_attributes AtrTy;
	
AtrTy m_atr;	
DispTy m_disp;

NormalListTy m_norm;
// This odd thing needs to move
mutable bool m_use_normals,m_use_color;

typedef std::ostringstream OsStreamTy;

// 2008-03-18
index_type size() const { return Super::size(); } 
//template <class Tx> string_type  mprintf(const Tx & v) const{Tx cc=v; OsStreamTy os; os<<cc;	return os.str(); } 
void create_display_tree(DispTy * root)
{
//	root=root->get_child(name()+mprintf(mangled_list));
//	for (index_type i=0; i<NCHAINS; ++i)
//	if (m_all[i].size()) root->get_child(string_type(chain_names[i])); 
}

// overloading push_back confused the compiler...

void xpush_back( const TgTPtr * pts, const index_type n)
{
	normal_type vec[n];
	for ( index_type i=0; i<n; ++i)
// 2024 need "this" wtf 
	{this-> push_back(pts[i]); vec[i]=pts[i]->loc();}
	//{ push_back(pts[i]); vec[i]=pts[i]->loc();}
	if (n<2) return; 
	for ( index_type i=0; i<n; ++i)
	{
		index_type next=(i+1)%n, last=(i-1+n)%n;
		normal_type nor=vec[i].cross(vec[next],vec[last]).normalize();
	        if ((nor.length()>1.01)||(nor.length()<.99))
	        	{std::cout<<" nx "<<nor.x()<<" "<<nor.y()<<" "<<nor.z()<<"\n";}
		m_norm.push_back(nor);
	}
}

void draw(DispTy & root)
{
	if (root.hidden()) return; 
//	if (root.use_instance_settings()) { draw(); return;}
	DrawBegin(root.settings(),atr()); 
	if(root.settings().picking()) DrawBondPick(root); else DrawBond(root); 
	DrawEnd(); 
}
template < class Thing, class Ts > 
	bool model_find( display_tree & root,const index_type target, Thing & thing, Ts& is ) const
{
	typename Super::const_iterator ii=Super::begin();
	index_type ver=atr().m_verticies;
	if (ver<1) ver=1;
// 2024 sign issue looks like bug lol 
	//index_type roff=ver*(target-root.m_idx_min);
	int roff=ver*(int(target)-int(root.m_idx_min));
// 2024 wtf index_type is an int ... 
	//if ((roff<0)||(((unsigned int)(roff))>=size()))
	if ((roff<0)||(((roff))>=size()))
	{	std::cout<<"mol view list find error :"<<roff<<" from target="
	<<target<<" and min="<<root.m_idx_min<<"\n"; root.lineage(std::cout,0); 
	return false;  }
	while (roff) {roff--; ++ii;}
	while (ver) {thing.push_back(**ii); ++ii; --ver;  }
	return true;
	}
index_type DrawBond(DispTy & root) const 
{
	const display_settings & ds=root.settings();
//	const bool picking=ds.picking();
//	if (picking) {std::cout<<"picking is set with "<<root.m_idx_max<<"\n";}
//	index_type  draw_serial=root.m_idx_max;
	index_type ver=atr().m_verticies;
	index_type ilim=size();
	index_type nlim=m_norm.size();
	if (ilim%ver) 
	{ std::cout<<"Odd number of verticies "<<ilim<<" "<<atr().m_verticies <<"\n";}
//	std::cout<<" drawbond :"<<v<<" sz="<<i.size()<<"\n";
	//ChainListTy::
	typename Super::const_iterator ii=Super::begin();
	if (nlim&&m_use_normals||!m_use_color)

	{ typename NormalListTy::const_iterator ni=m_norm.begin(); 
		normal_type n;
		while (ilim) {
//		if (picking) {glPushName(draw_serial++);}
		if (nlim&&m_use_normals){
		n=(*(ni++));
		float x[]={n.x(),n.y(),n.z()};
		glNormal3fv(x);
		}
		const atom_type & a=*(*ii); 
		if (!m_use_color){
		n=a.get_color();
	
		myPoint4<float> diffuse=myPoint4<float>(n.x(),n.y(),n.z(),1); 
	ds.offset_always(&display_settings::m_tint,diffuse);
	glColor4fv(diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		
	//	float diffuse[]={n.x(),n.y(),n.z(),1};
		//myPoint4<float> diffusefk=myPoint4<float>(diffuse);
		//glColor4fv(diffusefk);
		// the fking operators and ctors are fked up
		//ds.offset_always(&display_settings::m_tint,diffuse);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		}
		glVertex3f(a.x(),a.y(),a.z());++ii; --ilim;}
//		if (picking) {glFlush(); glPopName();}	
		}else
{	//if(!picking)
	while (ilim) {const atom_type & a=*(*ii); glVertex3f(a.x(),a.y(),a.z()); ++ii; --ilim;}
//else while (ilim) {glPushName(draw_serial++);const atom_type & a=*(*ii);
 //glVertex3f(a.x(),a.y(),a.z()); glPopName();++ii; --ilim;}
}
//root.m_idx_max=draw_serial;
//	root.sprout_max(); 
return size();	
}
index_type DrawBondPick(DispTy & root) const 
{
//	const display_settings & ds=root.settings();
//	const bool picking=ds.picking();
//	if (picking) {std::cout<<"picking is set with "<<root.m_idx_max<<"\n";}
	index_type  draw_serial=root.m_idx_max;
	index_type ver=atr().m_verticies;
	int modes[]={GL_POINTS,GL_LINES,GL_TRIANGLES};
	int mode=modes[ver-1];
	index_type ilim=size();
	index_type nlim=m_norm.size();
	glEnd();
	glPushName(draw_serial++);
	if (ilim%ver) 
	{ std::cout<<"Odd number of verticies "<<ilim<<" "<<atr().m_verticies <<"\n";}
//	std::cout<<" drawbond :"<<v<<" sz="<<i.size()<<"\n";
	//ChainListTy::
	typename Super::const_iterator ii=Super::begin();
	const bool unorm=(nlim&&m_use_normals);

	 typename NormalListTy::const_iterator ni=m_norm.begin(); 
		
		while (ilim) {
			glBegin(mode); 
		index_type inner=ver;
		while (inner) {
		//glPushName(draw_serial++);
		const atom_type & a=*(*ii); 
		if (unorm) {
			
			// Don'trely on piuxel values- colors not right
			normal_type n=(*(ni++));
			float x[]={n.x(),n.y(),n.z()};
			glNormal3fv(x);
			n=a.get_color();
			float diffuse[]={n.x(),n.y(),n.z(),1};
			glColor4fv(diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);}
		
		glVertex3f(a.x(),a.y(),a.z());++ii; --ilim;
		--inner;
		} //inner
		//}
		glEnd(); glLoadName(draw_serial++);
		//if (picking) {glFlush(); glPopName();}	
		}
glPopName();
root.m_idx_max=draw_serial;
//std::cout<<"setting max to "<<root.m_idx_max<<"\n";
	root.sprout_max(); 
return size();	
}

void DrawBegin(const display_settings & ds,const ChainAttrTy &col) const
{//	glDisable(GL_POINT_SMOOTH);glDisable( GL_CULL_FACE );
	GLfloat crap[] = { col.m_r,col.m_g,col.m_b,1};
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
	if (col.m_verticies==3) mode=GL_TRIANGLES;
	if ( mode==GL_TRIANGLES){myglutil::lights_on(ds.light()); }else   myglutil::lights_off(0); 

	float width=3.0f;
	ds.set_from(&display_settings::m_thick,width);  glLineWidth(width); 
	ds.set_from(&display_settings::m_size,width);  glPointSize(width);	
	
	glColor4fv(diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, myglutil::black); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, myglutil::black);  
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuse);
	
	myglutil::set_blending(ds.blend()); 
//	if (ds.blend()!=0)
//	{glBlendFunc(GL_SRC_ALPHA,1-GL_SRC_ALPHA);glEnable(GL_BLEND); }
//	else {glDisable(GL_BLEND);}
	
	if (mode==GL_TRIANGLES) {  myglutil::lights_on(ds.light());  }
	else   myglutil::lights_off(0); 
	// above commands may not work after moding...
	glBegin(mode); 
}

void DrawEnd() const{glEnd();glDisable(GL_CLIP_PLANE0); } 


};

#endif

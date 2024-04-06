#include "atom_list.h"

typedef atom_container MyClass;

void MyClass::create_display_tree(DispTy * root)
{
	root=root->get_child(name());
	MyItorTy ii=begin(),ie=end();
	while(ii!=ie)
	{	// This is a low order process so I coded it slow + lazy
		atom_type & p=(*ii);
		flag_type cl=p.naive_class();
		if (cl&1) {root->get_child("atom"); m_all[ATOM].push_back(&p); }
		else if (cl&2) {root->get_child("hetero"); m_all[HETERO].push_back(&p); }
		else {root->get_child("other"); m_all[OTHER].push_back(&p); }
	++ii;
	}
}

bool MyClass::topo(const atom_type & p, const flag_type flag) const
{
	// one means outside of this condition. 
	flag_type state=((p.x()>m_X)?1:0)|((p.y()>m_Y)?2:0)|((p.z()>m_Z)?4:0);
	state|=((p.x()<m_x)?8:0)|((p.y()<m_y)?16:0)|((p.z()<m_z)?32:0);
	state=state&flag;
	// return false if all conditions "inside" 
	return (state!=0);
}


void MyClass::find_hull()
{
	coord_type MAXXX=100000; coord_type MINNN=-100000;
	m_x=m_y=m_z=MAXXX;
	m_X=m_Y=m_Z=MINNN;
for (MyItorTy i=begin(); i!=end(); ++i) 
{
	atom_type & x=(*i);
	m_cg+=MyPtTy(x);
	if (x.x()>m_X) m_X=x.x();if (x.x()<m_x) m_x=x.x();	
	if (x.y()>m_Y) m_Y=x.y();if (x.y()<m_y) m_y=x.y();
	if (x.z()>m_Z) m_Z=x.z();if (x.z()<m_z) m_z=x.z();
	}
	
	m_cg/=float(size()); 
	
	cout<<"hull m_x="<<m_x<<" m_y="<<m_y<<" m_z="<<m_z<<" m_X="<<m_X<<" m_Y="<<m_Y
	<<" m_Z="<<m_Z<<"\n";
	
	}
	void MyClass::draw() {}
	
	
	
	void MyClass::draw(DispTy & root)
	{
		if (root.hidden()) return; 
	//	if (root.use_instance_settings()) { draw(); return;}
 	myglutil::generic_lighting();
 	index_type i=0;
 	for ( index_type j=0; j<LTYPES; ++j)
 	{
 	if (m_all[j].size()!=0)
 	{	
 	 DispTy & child =*(root[i++]);	
 	 const display_settings & ds=child.settings();
 	 if( !child.hidden())
 	 {
 	 //	DrawList(child,m_all[i]);
 	 //	DrawBegin(root.settings()); 
 	 	DrawBegin(ds);  // wtf???
 	 	if (ds.style()==~0)
 	 	{ std::cout<<" style: 0=disks{move},1=sphere{solid.large,move}.2=points{move}\n";  } 
 	 	if (ds.style()==1)DrawSpheres(child,m_all[j]);
 	 	else if (ds.style()==2)DrawPoints(child,m_all[j]);
 	 	else DrawDisks(child,m_all[j]);
 	 }
 	}	
 	}

	DrawEnd();
}

void MyClass::DrawSpheres( DispTy & disp, ChildIndTy & li)
{
	const display_settings & ds=disp.settings();
	const bool solid_sphere=((ds.substyle()&1)!=0);
	coord_type shrink=((ds.substyle()&2)==0)?.25f:1.0f;
	shrink=((ds.substyle()&4)!=0)?ds.dia():shrink;
	/// Artistic effect only- really doesn't make sense...
	const bool move_n=((ds.substyle()&4)!=0);
	
	const bool picking=disp.settings().picking();
	 index_type  draw_serial=disp.m_idx_max;
	myglutil::lights_on(ds.light()); 
	ChildItorTy ii=li.begin(), ie=li.end();
	while(ii!=ie)
	{
		atom_type & p=*(*ii);
		coord_type rad=p.radius()*shrink;
		set_to_atom(p,ds);
		if (picking) {glPushName(draw_serial++);}
       myglutil::shiny_material(true||false); 
  	glPushMatrix(); 
  	//glTranslatef( p.x(),p.y(),p.z() ); 
  	if ( !move_n) glTranslatef( p.x(),p.y(),p.z() ); 
  	else glTranslatef(p.xn(),p.yn(),p.zn());
  	
  	if (solid_sphere)glutSolidSphere( rad, 10, 10 );
  	else glutWireSphere( rad, 4, 4 );
  	glPopMatrix(); 		
	if (picking) {glPopName();}	
		++ii;
	}
	disp.m_idx_max=draw_serial;
	disp.sprout_max(); 
}
void MyClass::DrawDisks( DispTy & disp, ChildIndTy & li)
{
	const display_settings & ds=disp.settings();
	const bool picking=ds.picking();
	index_type  draw_serial=disp.m_idx_max;
	const bool move_n=((ds.substyle()&1)!=0);
	coord_type disk_size=1;
	ds.set_from(&display_settings::m_dia,disk_size);
	myglutil::disk_start();
	myglutil::lights_on(ds.light()); 
//	 glDisable( GL_CULL_FACE );
	ChildItorTy ii=li.begin(), ie=li.end();
	while(ii!=ie)
	{
		atom_type & p=*(*ii);
	//	coord_type rad=p.radius();
		set_to_atom(p,ds);
       myglutil::shiny_material(true||false); 
       if (picking) {glPushName(draw_serial++);}
  	glPushMatrix(); 
  	if ( !move_n) glTranslatef( p.x(),p.y(),p.z() ); else glTranslatef(p.xn(),p.yn(),p.zn());
  	if (p.m_t.m_dot_z!=0) {  glRotatef(p.m_t.m_dot_z,p.m_t.m_curl_x,p.m_t.m_curl_y,0); }
  	gluDisk( xxx,0,disk_size,4,2);
  	//if (p.m_t.m_dot_z!=0) {  glRotatef(p.m_t.m_dot_z,p.m_t.m_curl_x,p.m_t.m_curl_y,0); }
  	//gluDisk( xxx,0,10,4,2);
  	glPopMatrix(); 	
  	if (picking) {glPopName();}		
		++ii;
	}
	disp.m_idx_max=draw_serial;
	disp.sprout_max(); 
}
void MyClass::set_to_atom(const atom_type & p,const display_settings& ds) 
{	myPoint4<float> diffuse=p.get_color_v(); 
	ds.offset_always(&display_settings::m_tint,diffuse);
	glColor4fv(diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		GLfloat mat_diffusex[] = { 0,0,0,1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_diffusex);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diffusex);
		/*
		GLfloat mat_diffuse[] = { x[4],x[5],x[6],1.0 };
		GLfloat mat_diffusex[] = { 0,0,0,1.0 };
		GLfloat mat_diffusef[] = { x[4]*.1f+.05f,x[5]*.1f+.05f,x[6]*.1f+.05f,1.0 };
		 	glColor3f(x[6],x[7],x[8] );
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_diffusex);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diffusef);
		*/
		
		
		
		}
void MyClass::DrawPoints( DispTy & disp, ChildIndTy & li)
{
	const display_settings & ds=disp.settings();
	const bool picking=ds.picking();
	index_type  draw_serial=disp.m_idx_max;
	const bool move_n=((ds.substyle()&1)!=0);
		glBegin(GL_POINTS);
//	 myglutil::lights_off(7); 
	
	ChildItorTy ii=li.begin(), ie=li.end();
	while(ii!=ie)
	{
		atom_type & p=*(*ii);
	//	coord_type rad=p.radius();
		set_to_atom(p,ds);
		 if (picking) {glPushName(draw_serial++);}
       myglutil::shiny_material(true||false); 
 
  	if ( !move_n) glVertex3f( p.x(),p.y(),p.z() ); 
		else glVertex3f(p.xn(),p.yn(),p.zn());
  	
  	if (picking) {glPopName();}		
		++ii;
	}
	myglutil::point_end(); 
	disp.m_idx_max=draw_serial;
	disp.sprout_max(); 
	
}


void MyClass::DrawEnd() {glDisable(GL_CLIP_PLANE0); }
void MyClass::DrawBegin(const display_settings & ds) 
{
	myPoint4<float> clip_plane;
	glDisable(GL_CLIP_PLANE0); 
	if (ds.set_from(&display_settings::m_clip_plane,clip_plane))
	{ 	glClipPlane(GL_CLIP_PLANE0,myPoint4<double>(clip_plane)); 
	 	glEnable(GL_CLIP_PLANE0);
	}
	float width=3.0f;
	ds.set_from(&display_settings::m_thick,width);  glLineWidth(width); 
	ds.set_from(&display_settings::m_size,width);  glPointSize(width);
//	http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/blendfunc.html
//	if (ds.blend()!=0)
//	{glBlendFunc(GL_SRC_ALPHA,1-GL_SRC_ALPHA);glEnable(GL_BLEND); }
//	else {glDisable(GL_BLEND);}
	myglutil::set_blending(ds.blend()); 
	glDisable( GL_CULL_FACE ); myglutil::generic_mat_1(); myglutil::lights_off(7); 
}


#include "molecule_view_list.cpp"
typedef molecule_view_list MyClass;




void MyClass::xpush_back( const TgTPtr * pts, const index_type n)
{
	normal_type vec[n];
	for ( index_type i=0; i<n; ++i)
	{ push_back(pts[i]); vec[i]=pts[i]->loc();}
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

void MyClass::draw(DispTy & root)
{
	if (root.hidden()) return; 
//	if (root.use_instance_settings()) { draw(); return;}
	DrawBegin(root.settings(),atr()); 
	if(root.settings().picking()) DrawBondPick(root); else DrawBond(root); 
	DrawEnd(); 
}

MyClass::index_type MyClass::DrawBond(DispTy & root) const 
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
	typename Super::const_iterator ii=begin();
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
		//myPoint4<float> diffusefak=myPoint4<float>(diffuse);
		//glColor4fv(diffusefak);
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
MyClass::index_type MyClass::DrawBondPick(DispTy & root) const 
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
	typename Super::const_iterator ii=begin();
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

void MyClass::DrawBegin(const display_settings & ds,const ChainAttrTy &col) const
{//	glDisable(GL_POINT_SMOOTH);glDisable( GL_CULL_FACE );
	GLfloat crap[] = { col.m_r,col.m_g,col.m_b,1};
	myPoint4<float> diffuse=myPoint4<float>(crap);
	// if this is true, it inhibits the use of the atom color.
	// if false, the atom color is used.
	// to use the chain color in "crap" we need a substyle bit
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

void MyClass::DrawEnd() const{glEnd();glDisable(GL_CLIP_PLANE0); } 

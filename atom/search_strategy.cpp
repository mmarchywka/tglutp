#include "search_strategy.h"

typedef search_strategy MyClass;


void MyClass::LoadLists(const TgtListTy & li)
{
	for (TgtCItorTy p=li.begin(); p!=li.end(); ++p)
	{  TgtPtr pp=&(*p); m_x.push_back(pp); }
	m_y=m_x; m_z=m_x;
	PtrItorTy ii=m_x.begin(); PtrItorTy jj=m_x.end();
	mmdd::dout(MMDDLOC)<<"Sorting x list "<<m_x.size()<<"\n";
	//std::sort(ii,jj,compare_x());
	mmdd::dout(MMDDLOC)<<"Sorting y list\n";
	//ii=m_y.begin();  jj=m_y.end(); std::sort(ii,jj,compare_y());
	mmdd::dout(MMDDLOC)<<"Sorting z list\n";
	ii=m_z.begin();  jj=m_z.end(); std::sort(ii,jj,compare_z());
}

void MyClass::InitialRestrict(const coord_type z, const coord_type radius)
{   sorting_x=true; sorting_y=true;
	m_zi=m_z.begin(); m_zf=m_z.end();
//	mmdd::dout(MMDDLOC)<<" z has "<<m_z.size()<<kluge::nl;
	UpdateComposite(false,z,radius);
}
void MyClass::UpdateComposite(const bool inc,const coord_type z, const coord_type radius)
{       m_composite.clear();
	PtrItorTy z_end=m_z.end();
	const coord_type zi=z-radius; const coord_type zf=z+radius;
	//mmdd::dout(MMDDLOC)<<" z has "<<m_z.size()<<kluge::nl;
	while (m_zi!=z_end) { if((**m_zi).z()<zi) ++m_zi; else break; }
	PtrItorTy zx=inc?m_zi:m_zf;
	while (zx!=z_end) { if((**zx).z()<=zf) ++zx; else break; } m_zf=zx;
	for(zx=m_zi; zx!=m_zf; ++zx) m_composite.push_back(*zx);
	if (sorting_x)
	{m_xi=m_composite.begin(), m_xf=m_composite.end(); 
	std::sort(m_xi,m_xf,compare_x()); m_xi=m_composite.begin(); m_xf=m_composite.end();      }
	
}
void MyClass::Restricty(const bool inc, const coord_type y, const coord_type radius)
{	m_composite_y.clear();
	PtrItorTy y_end=m_composite_x.end();
	const coord_type yi=y-radius; const coord_type yf=y+radius;
	while (m_yi!=y_end) { if((**m_yi).y()<yi) ++m_yi; else break; }
	PtrItorTy yy=inc?m_yi:m_yf;
	while (yy!=y_end) { if((**yy).y()<=yf) ++yy; else break; } m_yf=yy;
	for(yy=m_yi; yy!=m_yf; ++yy) m_composite_y.push_back(*yy);
	//cout<<"x list is "<<m_composite_x.size()<<" vs z of "<<m_composite.size()<<"\n";
}
void MyClass::Restrictx(const bool inc, const coord_type x, const coord_type radius)
{	m_composite_x.clear();
	PtrItorTy x_end=m_composite.end();
	const coord_type xi=x-radius; const coord_type xf=x+radius;
	while (m_xi!=x_end) { if((**m_xi).x()<xi) ++m_xi; else break; }
	PtrItorTy xx=inc?m_xi:m_xf;
	while (xx!=x_end) { if((**xx).x()<=xf) ++xx; else break; } m_xf=xx;
	for(xx=m_xi; xx!=m_xf; ++xx) m_composite_x.push_back(*xx);
	if (sorting_y)
	{m_yi=m_composite_x.begin(), m_yf=m_composite_x.end(); 
	std::sort(m_yi,m_yf,compare_y()); m_yi=m_composite_x.begin(); m_yf=m_composite_x.end();      }
	
	
	//cout<<"x list is "<<m_composite_x.size()<<" vs z of "<<m_composite.size()<<"\n";
}
void MyClass::IncrementalRestrict(const coord_type z, const coord_type radius)
{  UpdateComposite(true,z,radius); }
//	std::cout<<" list size is "<<m_composite.size()<<" from "<<m_z.size()<<" at "<<z<<
//	" with radius "<<radius<<"\n";	
	
void MyClass::IncrementalRestrict(const coord_type x,const coord_type z, const coord_type radius)
{
	m_composite_x.clear();
	PtrItorTy ii=m_composite.begin();
	PtrItorTy en=m_composite.end();
	const coord_type rho=radius*radius;
	while (ii!=en)
	{
		const coord_type zz=(**ii).z()-z;
		const coord_type xx=(**ii).x()-x;
		if (zz*zz+xx*xx<=rho) m_composite_x.push_back(*ii);
	++ii;	
	}
	//std::cout<<
	//" X list size is "<<m_composite_x.size()<<" from "<<m_composite.size()
	//<<" at "<<z<<" with x="<<x<<" with radius "<<radius<<"\n";
	}

void MyClass::BuildPointList( pdb_atom& probe,     AtomLiTy & dest           //PointListTy & point_list 
	,const build_point_list_param &bplp ) //= build_point_list_param())
{
	coord_type neighborhood_radius,filter_radius, grid_space, shell_thickness,safe_radius;
	bplp.load(neighborhood_radius,filter_radius, grid_space, shell_thickness,safe_radius);
	coord_type cx=0, cy=0,cz=0;
	cx=probe.x(); cy=probe.y(); cz=probe.z(); 
	probe.radius(filter_radius);
	probe.highlight(true); probe.show_point(true); 
	coord_type test_grid_radius=neighborhood_radius-probe.radius()-safe_radius;
	mmdd::dout(MMDDLOC)<<"test grid radius is "<<test_grid_radius<<"\n";
	//m_test_list.clear(); 
	//MakeTestList(probe,neighborhood_radius);
	//m_test_list.find_hull();
	InitialRestrict(-test_grid_radius+cz,safe_radius+probe.radius());
	coord_type x=0; coord_type y=0; coord_type z=0;
	coord_type tgr2=test_grid_radius*test_grid_radius; 
	count_type reject=0; 
	coord_type naive_delta=grid_space;
	for (z=-test_grid_radius; z<test_grid_radius; z+=grid_space)
	{coord_type gr2=(tgr2-z*z); coord_type grid_rho=sqrt(gr2);
	cout<< "z :"<<z<<" pt: "<< //point_list.size()
	dest.size()
	<<" of "<< reject
	<<" and map="<< m_test_map.size()   <<"z-list "<<m_composite.size()<<"      \r"; cout.flush();
	//MakeZList(z+cz,shell_thickness+filter_radius);
	if (sorting_x)Restrictx(false,cx-grid_rho,safe_radius+probe.radius()); 
	for (x=-grid_rho; x<grid_rho; x+=grid_space)
	{
	if (!sorting_x)IncrementalRestrict(x+cx,z+cz+grid_space,safe_radius+probe.radius());	
	coord_type ylim=sqrt(gr2-x*x);
////////////////////////////////////////////////////////////////////////////////////
	if (!sorting_y) {
	for (y=-ylim; y<ylim;) // y-=grid_space)
	{	probe.move_to(x+cx,y+cy,z+cz); 
	if (FitCheckShell(m_composite_x,probe,shell_thickness,naive_delta)) 
	{ //probe.highlight(true); probe.show_point(true); 
	probe.oriented_toward(*m_closest_hit,probe.radius());
	probe.color(*m_closest_hit);dest.push_back(probe);
	} else {++reject;}
	 y+=grid_space;	//else {++reject;  if (naive_delta<grid_space) y-=grid_space; else y-=naive_delta;}
	} }  //y and sorting
 ////////////////////////////////////////////////////////////////////////////////////////
else { // sorting y...
Restricty(false,cy-ylim,safe_radius+probe.radius()); 
for (y=-ylim; y<ylim;) // y-=grid_space)
	{	probe.move_to(x+cx,y+cy,z+cz); 
	if (FitCheckShell(m_composite_y,probe,shell_thickness,naive_delta)) 
	{ //probe.highlight(true); probe.show_point(true); 
	probe.oriented_toward(*m_closest_hit,probe.radius());
	probe.color(*m_closest_hit);probe.incr(); dest.push_back(probe);
	m_test_map[m_closest_hit]++;
	} else {++reject;}
	 y+=grid_space;	//else {++reject;  if (naive_delta<grid_space) y-=grid_space; else y-=naive_delta;}
	Restricty(true,cy+y,safe_radius+probe.radius()); 
	} 
}
////////////////////////////////////////////////////////////////////////////////////////	
	
	if (sorting_x) Restrictx(true,cx+x+grid_space,safe_radius+probe.radius()); 
	} //x
	IncrementalRestrict(z+cz+grid_space,safe_radius+probe.radius());
	} //z
std::cout<<"\n";	
}

void MyClass::BuildPointListMap( pdb_atom& probe
	,const build_point_list_param &bplp) // = build_point_list_param())
{
	coord_type neighborhood_radius,filter_radius, grid_space, shell_thickness,safe_radius;
	bplp.load(neighborhood_radius,filter_radius, grid_space, shell_thickness,safe_radius);
	coord_type cx=0, cy=0,cz=0;
	cx=probe.x(); cy=probe.y(); cz=probe.z(); 
	probe.radius(filter_radius);
	probe.highlight(true); probe.show_point(true); 
	coord_type test_grid_radius=neighborhood_radius-probe.radius()-safe_radius;
	mmdd::dout(MMDDLOC)<<"test grid radius is "<<test_grid_radius<<"\n";
	InitialRestrict(-test_grid_radius+cz,safe_radius+probe.radius());
	coord_type x=0; coord_type y=0; coord_type z=0;
	coord_type tgr2=test_grid_radius*test_grid_radius; 
	count_type reject=0; 
	coord_type naive_delta=grid_space;
	for (z=-test_grid_radius; z<test_grid_radius; z+=grid_space)
	{coord_type gr2=(tgr2-z*z); coord_type grid_rho=sqrt(gr2);
	cout<< "z :"<<z<<" of "<< reject
	<<" map="<< m_test_map.size()   <<"z-list "<<m_composite.size()<<" \n"; cout.flush();
	Restrictx(false,cx-grid_rho,safe_radius+probe.radius()); 
	for (x=-grid_rho; x<grid_rho; x+=grid_space)
	{
	coord_type ylim=sqrt(gr2-x*x);
	Restricty(false,cy-ylim,safe_radius+probe.radius()); 
	for (y=-ylim; y<ylim;) // y-=grid_space)
	{	probe.move_to(x+cx,y+cy,z+cz); 
	if (FitCheckShell(m_composite_y,probe,shell_thickness,naive_delta)) 
	{ //probe.highlight(true); probe.show_point(true); 
	//probe.oriented_toward(*m_closest_hit,probe.radius());
	//probe.color(*m_closest_hit);dest.push_back(probe);
	m_test_map[m_closest_hit]++;
	} else {++reject;}
	 y+=grid_space;
	Restricty(true,cy+y,safe_radius+probe.radius()); 
	} 
	Restrictx(true,cx+x+grid_space,safe_radius+probe.radius()); 
	} //x
	IncrementalRestrict(z+cz+grid_space,safe_radius+probe.radius());
	} //z
	
}
